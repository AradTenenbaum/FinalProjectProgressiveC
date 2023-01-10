#include "segment.h"
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"
#include "objects.h"
#include <stdlib.h>

// Insert node to end of tnode list
void insertNodeToEndList(TNODE_LIST* lst, TNODE_LNODE* newTail)
{
	if (isEmptyList(lst) == true)
	{
		lst->head = newTail;
		lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

// Insert data to end of Tree node List
void insertDataToEndList(TNODE_LIST* lst, TNODE* data)
{
	if (data != NULL) {
		TNODE_LNODE* newTail;
		newTail = createNewListNode(data, NULL);
		insertNodeToEndList(lst, newTail);
	}
}

// Create new positon
IMG_POS* newPosition(IMG_POS current, unsigned short addRow, unsigned short addCol) {
	IMG_POS* iPos = (IMG_POS*)malloc(sizeof(IMG_POS));
	memoryAndFileValidation(iPos);
	(*iPos)[0] = current[0] + addRow;
	(*iPos)[1] = current[1] + addCol;
	return iPos;
}

// return an empty list
void makeEmptyList(TNODE_LIST* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

// Check if list is empty
bool isEmptyList(TNODE_LIST* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}

// Create a new list node
TNODE_LNODE* createNewListNode(TNODE* data, TNODE_LNODE* next)
{
	TNODE_LNODE* output;
	// Create the node
	output = (TNODE_LNODE*)malloc(sizeof(TNODE_LNODE));
	memoryAndFileValidation(output);

	// Allocate a tree node
	output->node = (TNODE*)malloc(sizeof(TNODE));
	memoryAndFileValidation(output->node);
	output->node = data;
	// Point the node to the next
	output->next = next;
	return output;
}

// Create a tree node
TNODE* CreateTreeNode(IMG_POS position, TNODE_LIST lst)
{
	TNODE* t;
	// Allocate Tree node
	t = (TNODE*)malloc(sizeof(TNODE*));
	memoryAndFileValidation(t);
	// Create children list and position in pixel array
	t->nextPossiblePositions = lst;
	t->position[0] = position[0];
	t->position[1] = position[1];
	return t;
}

// Make position list empty
void makeEmptyPosList(IMG_POS_LIST* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

// Check if position list is empty
bool isEmptyPosList(IMG_POS_LIST* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}

// Create a new position list node
IMG_POS_NODE* createNewPosListNode(IMG_POS* data, IMG_POS_NODE* next, IMG_POS_NODE* prev)
{
	IMG_POS_NODE* output;
	// Create the node
	output = (IMG_POS_NODE*)malloc(sizeof(IMG_POS_NODE));
	memoryAndFileValidation(output);

	output->position[0] = (*data)[0];
	output->position[1] = (*data)[1];
	// Point the position to the next
	output->next = next;
	// Point the previus position
	output->prev = prev;

	return output;
}

// Insert node to end of pos List
void insertNodeToEndPosList(IMG_POS_LIST* lst, IMG_POS_NODE* newTail)
{
	if (isEmptyPosList(lst) == true)
	{
		newTail->prev = NULL;
		lst->head = newTail;
		lst->tail = newTail;
	}
	else
	{
		newTail->prev = lst->tail;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

// Create a node and insert into the end of the list
void insertDataToEndPosList(IMG_POS_LIST* lst, IMG_POS_NODE* data)
{
	if (data != NULL) {
		IMG_POS_NODE* newTail;
		newTail = createNewPosListNode(data, NULL, NULL);
		insertNodeToEndPosList(lst, newTail);
	}
}

// print the position list
void printPosList(IMG_POS_LIST lst) {
	IMG_POS_NODE* curr = lst.head;
	while (curr != NULL)
	{
		printf("(%d,%d) ", curr->position[0], curr->position[1]);
		curr = curr->next;
	}
	printf("\n");
}

// Create a position list recursive
void createPositionsListHelper(TNODE* root, IMG_POS_LIST* posList) {
	TNODE_LNODE* curr = root->nextPossiblePositions.head;
	// If there are children to the tree
	if (!isEmptyList(&root->nextPossiblePositions)) {
		while (curr != NULL)
		{
			createPositionsListHelper(curr->node, posList);
			// Insert position to list
			insertDataToEndPosList(posList, &(curr->node->position));
			curr = curr->next;
		}
	}
}

IMG_POS_LIST* createPositionsList(SEGMENT segment) {
	IMG_POS_LIST* imgPosList;
	imgPosList = malloc(sizeof(IMG_POS_LIST));
	// Init list
	makeEmptyPosList(imgPosList);
	// Add root
	insertDataToEndPosList(imgPosList, segment.root->position);
	// Function that adds all positions except the root
	createPositionsListHelper(segment.root, imgPosList);
	return imgPosList;
}

// Insert position list into positions list array
void insertPosListToPosListArray(IMG_POS_LIST** segments, IMG_POS_LIST lst, int* size) {
	(*size)++;
	(*segments)[(*size) - 1] = lst;
}

// Find all the segments
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments) {
	SEGMENT segment;
	// Using a bool multi array to check which pixel was already picked
	bool** flags = memoryAllocationMultiBoolArray(img->cols, img->rows);
	IMG_POS pos;
	IMG_POS_LIST posList;
	int size = 0;
	unsigned char max;
	unsigned char min;

	// Go over the image pixels
	for (int i = 0; i < img->rows; i++)
	{
		for (int j = 0; j < img->cols; j++)
		{
			// If pixel is not already in the list
			if (flags[i][j] == false) {
				pos[0] = i;
				pos[1] = j;

				max = img->pixels[pos[0]][pos[1]] + threshold;
				min = img->pixels[pos[0]][pos[1]] - threshold;
				// If the pixel is greater than 255 or smaller than 0
				// We insert the value into a byte so the value won't be valid
				if ((img->pixels[pos[0]][pos[1]] + threshold) > 255) {
					max = 255;
				}
				if ((img->pixels[pos[0]][pos[1]] - threshold) < 0) {
					min = 0;
				}

				// Find a segment, update the flags array
				segment.root = findSingleSegmentHelper(img, pos, min, max, flags);

				// Create the position list of the segment
				posList = *(createPositionsList(segment));

				// Insert the position list into the array
				insertPosListToPosListArray(segments, posList, &size);


			}
		}
	}
	// Reallocate the segments positions array
	*segments = realloc(*segments, sizeof(IMG_POS_LIST) * size);
	memoryAndFileValidation(*segments);
	return size;
}

// Color the segments
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size) {

	GRAY_IMAGE* grayImage;
	grayImage = malloc(sizeof(GRAY_IMAGE));
	IMG_POS_NODE* curr;
	int width = 0, height = 0;

	// Go over the array and find the height and width
	for (int i = 0; i < size; i++)
	{
		curr = segments[i].head;
		// Go over each list
		while (curr != NULL) {
			// Get the biggest position
			if (curr->position[0] > height) height = curr->position[0];
			if (curr->position[1] > width) width = curr->position[1];
			curr = curr->next;
		}
	}
	grayImage->cols = width + 1;
	grayImage->rows = height + 1;
	grayImage->pixels = memoryAllocationMultiCharArray(width + 1, height + 1);

	// Insert new data into pixels array
	for (int i = 0; i < size; i++)
	{
		curr = segments[i].head;
		while (curr != NULL) {
			grayImage->pixels[curr->position[0]][curr->position[1]] = (i * (255 / (size - 1)));
			curr = curr->next;
		}
	}

	return grayImage;
}

// Find a single segment recursive
TNODE* findSingleSegmentHelper(GRAY_IMAGE* img, IMG_POS start, 
	unsigned char min, unsigned char max, bool** flags) {

	if (start[0] < 0 || start[0] >= img->rows ||
		start[1] < 0 || start[1] >= img->cols ||
		img->pixels[start[0]][start[1]] < min ||
		img->pixels[start[0]][start[1]] > max ||
		flags[start[0]][start[1]]) {
		return NULL;
	}
	else {
		flags[start[0]][start[1]] = true;
	}

	TNODE_LIST lst;
	makeEmptyList(&lst);

	// Insert data of all the positions arount the start position to the list
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, 1, 0), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, 1, 1), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, 0, 1), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, -1, 0), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, -1, -1), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, 0, -1), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, 1, -1), min, max, flags));
	insertDataToEndList(&(lst),
		findSingleSegmentHelper(img, *newPosition(start, -1, 1), min, max, flags));

	return CreateTreeNode(start, lst);

}

// Find a single segment
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold) {
	SEGMENT segment;

	// Allocate the booleans array
	bool** flags = memoryAllocationMultiBoolArray(img->cols, img->rows);

	// Find a single segment from the received location
	segment.root = findSingleSegmentHelper(img, start, img->pixels[start[0]][start[1]] - threshold,
		img->pixels[start[0]][start[1]] + threshold, flags);

	return segment;
}


// Print a list of positions
void printList(TNODE_LIST lst) {
	TNODE_LNODE* curr = lst.head;
	while (curr != NULL)
	{
		printf("(%d,%d) ", curr->node->position[0], curr->node->position[1]);
		curr = curr->next;
	}
	printf("\n");
}

// Print segment recursive
void printSegmentHelper(TNODE* root, GRAY_IMAGE grayImage) {
	TNODE_LNODE* curr = root->nextPossiblePositions.head;
	if (!isEmptyList(&(root->nextPossiblePositions))) {

		printf("root: (%d,%d)\n", root->position[0], root->position[1]);
		printf("children: ");
		printList(root->nextPossiblePositions);

		while (curr != NULL) {
			printSegmentHelper(curr->node, grayImage);
			curr = curr->next;
		}
	}
}

void printSegment(SEGMENT s, GRAY_IMAGE grayImage) {
	printSegmentHelper(s.root, grayImage);
}
#include "objects.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

RGB** memoryAllocationMultiPixelArray(int width, int height) {
	RGB** pixels;
	pixels = (RGB**)malloc(sizeof(RGB*) * height);
	for (int i = 0; i < height; i++) {
		pixels[i] = (RGB*)malloc(sizeof(RGB) * width);
	}
	return pixels;
}

void printColorImagePixels(COLOR_IMAGE * colorImage) {
	for (int i = 0; i < colorImage->rows; i++) {
		for (int j = 0; j < colorImage->cols; j++) {
			printf("%3d ", colorImage->pixels[i][j].r);
			printf("%3d ", colorImage->pixels[i][j].g);
			printf("%3d ", colorImage->pixels[i][j].b);

		}
		printf("\n");
	}
}

void printGrayImagePixels(GRAY_IMAGE* grayImage) {
	for (int i = 0; i < grayImage->rows; i++) {
		for (int j = 0; j < grayImage->cols; j++) {
			printf("%3d ", grayImage->pixels[i][j]);
		}
		printf("\n");
	}
}


COLOR_IMAGE* readPPM(char* fname) {
	FILE* fp = fopen(fname, "r");
	memoryAndFileValidation(fp);

	COLOR_IMAGE* colorImage = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE));
	memoryAndFileValidation(colorImage);

	char buffer[MAX_LINE_LENGTH];
	RGB** pixels;
	bool flag = false;

	int values[3];

	char* num;
	fgets(buffer, MAX_LINE_LENGTH, fp);
	if(buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);

	if (strlen(buffer) <= 3) {
		fgets(buffer, MAX_LINE_LENGTH, fp);
		if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
	}

	int i = 0;
	num = strtok(buffer, " ");
	while (i < 3) {
		
		while ((num != NULL) && (num[0] == '#')) {
			fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		}

		if ((num != NULL) && (strcmp(num, "P3") != 0))
			values[i++] = atoi(num);

		if (num == NULL && (i != 3))
		{
			fgets(buffer, MAX_LINE_LENGTH, fp);
			while (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		}
		else if(i != 3) num = strtok(NULL, " ");
	}

	// Allocate a pixels multi array
	pixels = memoryAllocationMultiPixelArray(values[WIDTH], values[HEIGHT]);

	int lineLength;
	char* tempValue;

	
	for (i = 0; i < values[HEIGHT]; i++) {
		lineLength = (values[WIDTH] * 3 * log10(values[MAX_VALUE]) + (values[WIDTH] * 3) + 2);
		fgets(buffer, lineLength, fp);
		tempValue = simplestrtok(buffer, ' ');
		for (int j = 0; j < values[WIDTH]; j++) {
			pixels[i][j].r = atoi(tempValue);
			tempValue = simplestrtok(NULL, ' ');
			pixels[i][j].g = atoi(tempValue);
			tempValue = simplestrtok(NULL, ' ');
			pixels[i][j].b = atoi(tempValue);
			tempValue = simplestrtok(NULL, ' ');
		}

	}

	colorImage->pixels = pixels;
	colorImage->rows = values[HEIGHT];
	colorImage->cols = values[WIDTH];
	

	fclose(fp);
	return colorImage;
}


GRAY_IMAGE* readPGM(char* fname) {
	FILE* fp = fopen(fname, "r");
	memoryAndFileValidation(fp);

	GRAY_IMAGE* grayImage = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	memoryAndFileValidation(grayImage);

	char buffer[MAX_LINE_LENGTH];
	char** pixels;

	int values[3];

	char* num;
	fgets(buffer, MAX_LINE_LENGTH, fp);
	if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);

	if (strlen(buffer) <= 3) {
		fgets(buffer, MAX_LINE_LENGTH, fp);
		if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
	}

	int i = 0;
	num = strtok(buffer, " ");
	while (i < 3) {

		while ((num != NULL) && (num[0] == '#')) {
			fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		}

		if ((num != NULL) && (strcmp(num, "P2") != 0))
			values[i++] = atoi(num);

		if (num == NULL && (i != 3))
		{
			fgets(buffer, MAX_LINE_LENGTH, fp);
			while (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		}
		else if (i != 3) num = strtok(NULL, " ");
	}

	// Allocate a pixels multi array
	pixels = memoryAllocationMultiCharArray(values[WIDTH], values[HEIGHT]);

	int lineLength;
	char* tempValue;


	for (i = 0; i < values[HEIGHT]; i++) {
		lineLength = (values[WIDTH] * 3 * log10(values[MAX_VALUE]) + (values[WIDTH] * 3) + 2);
		fgets(buffer, lineLength, fp);
		tempValue = simplestrtok(buffer, ' ');
		for (int j = 0; j < values[WIDTH]; j++) {
			pixels[i][j] = atoi(tempValue);
			tempValue = simplestrtok(NULL, ' ');
		}

	}

	grayImage->pixels = pixels;
	grayImage->rows = values[HEIGHT];
	grayImage->cols = values[WIDTH];


	fclose(fp);
	return grayImage;
}


// List general functions
void makeEmptyList(TNODE_LIST* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

bool isEmptyList(TNODE_LIST* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}

TNODE_LNODE* createNewListNode(TNODE* data, TNODE_LNODE* next)
{
	TNODE_LNODE* output;
	// Create the node
	output = (TNODE_LNODE*)malloc(sizeof(TNODE_LNODE));
	memoryAndFileValidation(output);

	output->node = (TNODE*)malloc(sizeof(TNODE));
	memoryAndFileValidation(output->node);
	output->node = data;
	// Point the node to the next
	output->next = next;
	return output;
}

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

// Create a node and insert into the end of the list
void insertDataToEndList(TNODE_LIST* lst, TNODE* data)
{
	if (data != NULL) {
		TNODE_LNODE* newTail;
		newTail = createNewListNode(data, NULL);
		insertNodeToEndList(lst, newTail);
	}
}

TNODE* CreateTreeNode(IMG_POS position, TNODE_LIST lst)
{
	TNODE* t;
	t = (TNODE*)malloc(sizeof(TNODE*));
	memoryAndFileValidation(t);
	t->nextPossiblePositions = lst;
	t->position[0] = position[0];
	t->position[1] = position[1];
	return t;
}

IMG_POS* newPosition(IMG_POS current, unsigned short addRow, unsigned short addCol) {
	IMG_POS* iPos = (IMG_POS*)malloc(sizeof(IMG_POS));
	memoryAndFileValidation(iPos);
	(*iPos)[0] = current[0] + addRow;
	(*iPos)[1] = current[1] + addCol;
	return iPos;
}

TNODE* findSingleSegmentHelper(GRAY_IMAGE* img, IMG_POS start
, char min, char max, bool** flags, SEGMENT* segment) {

	if (start[0] < 0 || start[0] > img->rows ||
		start[1] < 0 || start[1] > img->cols || 
		img->pixels[start[0]][start[1]] < min || 
		img->pixels[start[0]][start[1]] > max ||
		flags[start[0]][start[1]]) {
		return NULL;
	}
	else {
		flags[start[0]][start[1]] = true;
	}

	insertDataToEndList(&(segment->root->nextPossiblePositions), 
		findSingleSegmentHelper(img, *newPosition(start, 1, 0), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, 1, 1), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, 0, 1), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, -1, 0), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, -1, -1), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, 0, -1), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, 1, -1), min, max, flags, segment));
	insertDataToEndList(&(segment->root->nextPossiblePositions),
		findSingleSegmentHelper(img, *newPosition(start, -1, 1), min, max, flags, segment));

	return CreateTreeNode(start, segment->root->nextPossiblePositions);

}

SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold) {
	SEGMENT segment;

	bool** flags = memoryAllocationMultiBoolArray(img->cols, img->rows);

	TNODE_LIST lst;
	makeEmptyList(&lst);

	segment.root = findSingleSegmentHelper(img, start, img->pixels[start[0]][start[1]] - threshold,
		img->pixels[start[0]][start[1]] + threshold, flags, &segment);

	return segment;


}

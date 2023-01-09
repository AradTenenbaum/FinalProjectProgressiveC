#include "objects.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

// Forward declerations
bool isEmptyList(TNODE_LIST* lst);

RGB** memoryAllocationMultiPixelArray(int width, int height) {
	RGB** pixels;
	pixels = (RGB**)malloc(sizeof(RGB*) * height);
	memoryAndFileValidation(pixels);
	for (int i = 0; i < height; i++) {
		pixels[i] = (RGB*)malloc(sizeof(RGB) * width);
		memoryAndFileValidation(pixels[i]);
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

void printList(TNODE_LIST lst) {
	TNODE_LNODE* curr = lst.head;
	while (curr != NULL)
	{
		printf("(%d,%d) ", curr->node->position[0], curr->node->position[1]);
		curr = curr->next;
	}
	printf("\n");
}

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

// Segment List general functions
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
, unsigned char min, unsigned char max, bool** flags) {

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

SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold) {
	SEGMENT segment;

	bool** flags = memoryAllocationMultiBoolArray(img->cols, img->rows);

	segment.root = findSingleSegmentHelper(img, start, img->pixels[start[0]][start[1]] - threshold,
		img->pixels[start[0]][start[1]] + threshold, flags);

	return segment;
}

// Positions List general functions
void makeEmptyPosList(IMG_POS_LIST* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

bool isEmptyPosList(IMG_POS_LIST* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}

IMG_POS_NODE* createNewPosListNode(IMG_POS* data, IMG_POS_NODE* next, IMG_POS_NODE* prev)
{
	IMG_POS_NODE* output;
	// Create the node
	output = (IMG_POS_NODE*)malloc(sizeof(IMG_POS_NODE));
	memoryAndFileValidation(output);

	//memoryAndFileValidation(output->position);
	output->position[0] = (*data)[0];
	output->position[1] = (*data)[1];
	// Point the position to the next
	output->next = next;
	// Point the previus position
	output->prev = prev;

	return output;
}

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

void printPosList(IMG_POS_LIST lst) {
	IMG_POS_NODE* curr = lst.head;
	while (curr != NULL)
	{
		printf("(%d,%d) ", curr->position[0], curr->position[1]);
		curr = curr->next;
	}
	printf("\n");
}

void createPositiionsListHelper(TNODE* root, IMG_POS_LIST* posList) {
	TNODE_LNODE* curr = root->nextPossiblePositions.head;
	if (!isEmptyList(&root->nextPossiblePositions)) {
		while (curr != NULL)
		{
			createPositiionsListHelper(curr->node, posList);
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
	createPositiionsListHelper(segment.root, imgPosList);
	return imgPosList;
}

void insertPosListToPosListArray(IMG_POS_LIST** segments, IMG_POS_LIST lst, int* size) {
	(*size)++;
	//IMG_POS_LIST** newSegments = (IMG_POS_LIST**)realloc(segments, sizeof(IMG_POS_LIST*) * (*size));
	//memoryAndFileValidation(newSegments);
	(*segments)[(*size) - 1] = lst;
}

int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments) {
	SEGMENT segment;
	bool** flags = memoryAllocationMultiBoolArray(img->cols, img->rows);
	IMG_POS pos;
	IMG_POS_LIST posList;
	int size = 0;
	unsigned char max;
	unsigned char min;
	
	for (int i = 0; i < img->rows; i++)
	{
		for (int j = 0; j < img->cols; j++)
		{
			if (flags[i][j] == false) {
				pos[0] = i;
				pos[1] = j;

				max = img->pixels[pos[0]][pos[1]] + threshold;
				min = img->pixels[pos[0]][pos[1]] - threshold;
				if ((img->pixels[pos[0]][pos[1]] + threshold) > 255) {
					max = 255;
				}
				if ((img->pixels[pos[0]][pos[1]] - threshold) < 0) {
					min = 0;
				}

				segment.root = findSingleSegmentHelper(img, pos, min, max, flags);

				posList = *(createPositionsList(segment));

				insertPosListToPosListArray(segments, posList, &size);


			}
		}
	}
	*segments = realloc(*segments, sizeof(IMG_POS_LIST) * size);
	memoryAndFileValidation(*segments);
	return size;
}

GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size) {

	GRAY_IMAGE* grayImage;
	grayImage = malloc(sizeof(GRAY_IMAGE));
	IMG_POS_NODE* curr;
	int width = 0, height = 0;

	// Get rows and cols
	for (int i = 0; i < size; i++)
	{
		curr = segments[i].head;
		while (curr != NULL) {
			if (curr->position[0] > height) height = curr->position[0];
			if (curr->position[1] > width) width = curr->position[1];
			curr = curr->next;
		}
	}
	grayImage->cols = width + 1;
	grayImage->rows = height + 1;
	grayImage->pixels = memoryAllocationMultiCharArray(width + 1, height + 1);

	// Insert data into pixels array
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

void byte_to_binary(unsigned char x)
{
	unsigned char z;
	for (z = 128; z > 0; z >>= 1)
	{
		printf(((x & z) == z) ? "1" : "0");
	}
	printf("\n");
}

unsigned char convertCharTo7Bit(unsigned char c, unsigned char next, int offset) {
	unsigned char newChar = c << (offset + 1);	
	newChar = newChar + (next >> (7 - (offset + 1)));
	return newChar;
}

void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel) {
	FILE* fp = fopen(fname, "wb");
	memoryAndFileValidation(fp);
	
	unsigned char newPixel;
	unsigned char nextNewPixel;
	unsigned char combinedPixel;
	int offset = 0;
	int count = 0;
	bool isSet = true;

	fwrite(&(image->rows), sizeof(unsigned short), 1, fp);
	fwrite(&(image->cols), sizeof(unsigned short), 1, fp);

	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			offset = count % 7;

			// if count 7 next char
			if (offset == 0 && !(i == 0 && j == 0) && isSet) isSet = false;
			else isSet = true;

			if (isSet) {

				newPixel = (image->pixels[i][j] * maxGrayLevel) / 255;
				// case: last pixel
				if (i == (image->rows - 1) && j == (image->cols - 1)) nextNewPixel = 0;
				// case: last pixel in row
				else if (j == (image->cols - 1)) nextNewPixel = ((image->pixels[i + 1][0] * maxGrayLevel) / 255);
				// normal case
				else nextNewPixel = ((image->pixels[i][j + 1] * maxGrayLevel) / 255);

				combinedPixel = convertCharTo7Bit(newPixel, nextNewPixel, offset);
				fwrite(&(combinedPixel), sizeof(char), 1, fp);

				count++;
			}

		}
	}


	fclose(fp);
}

char* getNewFileNameBinToPgm(char* fname) {
	int length = strlen(fname);
	char* fnameTemp = malloc(sizeof(char) * length);
	strcpy(fnameTemp, fname);
	fnameTemp[length - 1] = 'm';
	fnameTemp[length - 2] = 'g';
	fnameTemp[length - 3] = 'p';
	return fnameTemp;
}

unsigned char convert7BitToChar(unsigned char bit7, unsigned char prev, int offset) {

	unsigned char helper = (255 >> (7 - offset + 1));
	unsigned char prevSave = prev & helper;
	prevSave = prevSave << (7 - offset);

	unsigned char newChar = (bit7 >> (offset + 1)) + prevSave;
	return newChar;
}

void convertCompressedImageToPGM(char* fname) {
	FILE* fpBin = fopen(fname, "rb");
	memoryAndFileValidation(fpBin);

	char* newFileName = getNewFileNameBinToPgm(fname);

	FILE* fpPGM = fopen(newFileName, "w");
	memoryAndFileValidation(fpPGM);

	unsigned short rows;
	unsigned short cols;
	unsigned char value;
	unsigned char prevValue = 0;
	int count = 0;
	int offset = 0;
	unsigned char fixedValue;
	unsigned char maxValue = 0;

	bool isLeftOver = false;
	unsigned char leftOver = 0;

	fread(&rows, sizeof(unsigned short), 1, fpBin);
	fread(&cols, sizeof(unsigned short), 1, fpBin);

	fprintf(fpPGM, "P2\n");
	fprintf(fpPGM, "%d %d\n", cols, rows);

	int maxLocation = ftell(fpPGM);

	// Save location for the max value
	fprintf(fpPGM, "   \n");

	int i = 0;
	int j = 0;

	while (i < rows) {
		while (j < cols) {

			if (isLeftOver) {
				isLeftOver = false;
				fprintf(fpPGM, "%3d ", leftOver);
				j++;
			}

			offset = count % 7;

			fread(&value, sizeof(unsigned char), 1, fpBin);

			fixedValue = convert7BitToChar(value, prevValue, offset);

			fprintf(fpPGM, "%3d ", fixedValue);

			if (offset == 6) {
				if (j < (cols - 1)) {
					fprintf(fpPGM, "%3d ", (value & 127));
					j++;
				}
				else {
					isLeftOver = true;
					leftOver = (value & 127);
				}
			}

			if (fixedValue > maxValue) maxValue = fixedValue;
			
			prevValue = value;
			count++;
			j++;
		}
		j = 0;
		i++;
		fprintf(fpPGM, "\n");
	}


	fseek(fpPGM, maxLocation, SEEK_SET);
	fprintf(fpPGM, "%d", maxValue);

	fclose(fpBin);
	fclose(fpPGM);

}

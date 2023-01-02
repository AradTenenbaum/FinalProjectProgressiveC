#ifndef _OBJECTS_H
#define _OBJECTS_H
#define _CRT_SECURE_NO_WARNINGS 
#define MAX_LINE_LENGTH 1024
#define WIDTH 0
#define HEIGHT 1
#define MAX_VALUE 2

typedef struct rgb {
	unsigned char r, g, b;
}RGB;

typedef struct colorImage {
	unsigned short rows, cols;
	RGB** pixels;
}COLOR_IMAGE;

typedef struct grayImage {
	unsigned short rows, cols;
	unsigned char** pixels;
}GRAY_IMAGE;

typedef unsigned short IMG_POS[2];

// Forward decleration
typedef struct tnode_lnode TNODE_LNODE;

typedef struct tnode_list {
	TNODE_LNODE* head;
	TNODE_LNODE* tail;
}TNODE_LIST;

typedef struct tnode {
	IMG_POS position;
	TNODE_LIST nextPossiblePositions;
}TNODE;

typedef struct tnode_lnode {
	TNODE* node;
	struct tnode_lnode* next;
}TNODE_LNODE;

typedef struct segment {
	TNODE* root;
}SEGMENT;

typedef struct img_pos_node {
	IMG_POS position;
	struct img_pos_node* next, * prev;
}IMG_POS_NODE;

typedef struct img_pos_list {
	IMG_POS_NODE* head;
	IMG_POS_NODE* tail;
}IMG_POS_LIST;

RGB** memoryAllocationMultiPixelArray(int width, int height);
COLOR_IMAGE* readPPM(char* fname);
GRAY_IMAGE* readPGM(char* fname);
void printColorImagePixels(COLOR_IMAGE* colorImage);
void printGrayImagePixels(GRAY_IMAGE* grayImage);
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold);
void printSegment(SEGMENT s, GRAY_IMAGE grayImage);
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments);
IMG_POS_LIST* createPositionsList(SEGMENT segment);
void printPosList(IMG_POS_LIST lst);
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size);

#endif
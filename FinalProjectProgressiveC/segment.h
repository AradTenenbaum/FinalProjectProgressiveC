#ifndef _SEGMENT_H
#define _SEGMENT_H
#define _CRT_SECURE_NO_WARNINGS 
#include "objects.h"
#include <stdbool.h>

SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold);
void printSegment(SEGMENT s, GRAY_IMAGE grayImage);
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments);
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size);
bool isEmptyList(TNODE_LIST* lst);
TNODE_LNODE* createNewListNode(TNODE* data, TNODE_LNODE* next);
TNODE* findSingleSegmentHelper(GRAY_IMAGE* img, IMG_POS start, 
	unsigned char min, unsigned char max, bool** flags);
IMG_POS_LIST* createPositionsList(SEGMENT segment);
void printPosList(IMG_POS_LIST lst);

#endif
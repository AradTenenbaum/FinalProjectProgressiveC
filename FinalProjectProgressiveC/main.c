#include <stdio.h>
#include <stdlib.h>
#include "objects.h"

void main() {
	GRAY_IMAGE* gi;
	gi = readPGM("test1.pgm");
	//printGrayImagePixels(gi);

	IMG_POS p;
	p[0] = 1;
	p[1] = 15;
	//SEGMENT s = findSingleSegment(gi, p, 2);
	//printSegment(s, *gi);
	//IMG_POS_LIST* lst = createPositionsList(s);
	//printPosList(*lst);

	IMG_POS_LIST** segments;
	segments = malloc(sizeof(IMG_POS_LIST*));
	*segments = malloc(sizeof(IMG_POS_LIST) * gi->cols * gi->rows);
	//segments = NULL;
	int size = findAllSegments(gi, 2, segments);
	//printf("segment_last: %d,%d", segments[0]->head->position[0], segments[0]->head->position[1]);
	/*
	for (int i = 0; i < size; i++)
	{
		printPosList(((*segments)[i]));
		printf("new segment\n");
	}
	*/

	//GRAY_IMAGE* newGrayImage = colorSegments(*segments, size);
	//printGrayImagePixels(newGrayImage);

	//saveCompressed("test.bin", gi, 120);

	convertCompressedImageToPGM("test.bin");


}
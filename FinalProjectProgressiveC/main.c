#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "objects.h"
#include "tools.h"

void main() {
	/*GRAY_IMAGE* gi;
	gi = readPGM("test1.pgm");
	IMG_POS p;
	p[0] = 1;
	p[1] = 15;

	IMG_POS_LIST** segments;
	segments = malloc(sizeof(IMG_POS_LIST*));
	*segments = malloc(sizeof(IMG_POS_LIST) * gi->cols * gi->rows);

	int size = findAllSegments(gi, 2, segments);
	*/int flag = 1;
	GRAY_IMAGE* gi = NULL;
	COLOR_IMAGE* ci = NULL;
	char fileName[100];
	int user_Choice;
	unsigned char threshold;
	IMG_POS_LIST** segments = NULL;
	int sizeOfSegmentsArray;
	GRAY_IMAGE* newGrayImage;
	GRAY_IMAGE* image;
	int maxGrayLevel;
	
	while (flag)
	{
		printf("\n\n1. Read an image file in PPM (p3) format\n2. Read an image file in PGM(p2) format\n3. Find all segments\n4. Color the segments\n5. Save the colored image to PGM\n6. Compress and save the original image in a compressed format\n7. Convert a compressed image to PGM format\n8. Convert PPM to PGM(p3->p2)\n9. Convert PGM to BW(p2->p2)\n10.Convert PPM to PGM(p6->p5)\n11.Convert PGM to BW(p5->p5)\n");
		scanf("%d", &user_Choice);
		switch (user_Choice)
		{
		case 1:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			ci = readPPM(fileName);
			break;
		case 2:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			gi = readPGM(fileName);
			break;
		case 3:
			if (gi == NULL)
			{
				printf("Please load pgm file");
			}
			else
			{
				printf("Please enter the threshold: ");
				scanf("%d", &threshold);
				segments = malloc(sizeof(IMG_POS_LIST*));
				*segments = malloc(sizeof(IMG_POS_LIST) * gi->cols * gi->rows);
				sizeOfSegmentsArray = findAllSegments(gi, threshold, segments);
			}
			break;
		case 4:
			if (segments == NULL)
				printf("Please find all segments");
			else
			{
				newGrayImage = colorSegments(*segments, sizeOfSegmentsArray);
			}
			break;
		case 5:
			
			break;
		case 6:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			image = readPGM(fileName);
			printf("Please enter a max gray level: ");
			scanf("%d", &maxGrayLevel);
			saveCompressed(fileName, image, maxGrayLevel);
			break;
		case 7:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			convertCompressedImageToPGM(fileName);
			break;
		case 8:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			convertPPMToPGM(fileName);
			break;
		case 9:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			convertPGMToBW(fileName);
			break;
		case 10:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			convertPPMToPGM_Bin(fileName);
			break;
		case 11:
			printf("Please enter a file name: ");
			scanf("%s", &fileName);
			convertPGMToBW_Bin(fileName);
			break;
		default:
			flag = 0;
			break;
		}
	}
	
}
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
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
	*/
	int user_Choice;
	printf("1. Read an image file in PPM (p3) format\n2. Read an image file in PGM(p2) format\n3. Find all segments\n4. Color the segments\n5. Save the colored image to PGM\n6. Compress and save the original image in a compressed format\n7. Convert a compressed image to PGM format\n8. Convert PPM to PGM(p3->p2)\n9. Convert PGM to BW(p2->p2)\n10.Convert PPM to PGM(p6->p5)\n11.Convert PGM to BW(p5->p5)\n");
	scanf("%d", &user_Choice);

	switch (user_Choice)
	{
	case 1:
		readPPM("test1.ppm");
		break;
	case 2:
		readPGM("test1.pgm");
		break;
	case 3:
		//findSingleSegment(gi, p, 2);
		break;
	case 4:
		//findAllSegments(gi, 2, segments);
		break;
	case 5:
		//colorSegments(*segments, size);
		break;
	case 6:
		//saveCompressed(fname, image, maxGrayLevel);
		break;
	case 7:
		//convertCompressedImageToPGM(fname);
		break;
	case 8:
		convertPPMToPGM("test1.ppm");
		break;
	case 9:
		convertPGMToBW("test1.pgm");
		break;
	case 10:
		convertPPMToPGM_Bin("download.ppm");
		break;
	case 11:
		convertPGMToBW_Bin("PPP.pgm");
		break;
	default:
		break;
	}
	
}
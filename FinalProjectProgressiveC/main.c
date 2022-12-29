#include <stdio.h>
#include "objects.h"

void main() {
	GRAY_IMAGE* gi;
	gi = readPGM("test1.pgm");
	//printGrayImagePixels(gi);

	IMG_POS p;
	p[0] = 3;
	p[1] = 3;
	SEGMENT s = findSingleSegment(gi, p, 2);
	
	printf("s");

}
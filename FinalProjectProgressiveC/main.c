#include <stdio.h>
#include "objects.h"

void main() {
	GRAY_IMAGE* gi;
	gi = readPGM("test1.pgm");
	//printGrayImagePixels(gi);

	IMG_POS p;
	p[0] = 0;
	p[1] = 8;
	SEGMENT s = findSingleSegment(gi, p, 2);
	printSegment(s, *gi);

}
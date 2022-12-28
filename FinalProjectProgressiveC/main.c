#include <stdio.h>
#include "objects.h"

void main() {
	COLOR_IMAGE* ci;
	ci = readPPM("test1.ppm");
	printColorImagePixels(ci);
}
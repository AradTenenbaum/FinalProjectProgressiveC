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


COLOR_IMAGE* readPPM(char* fname) {
	FILE* fp = fopen(fname, "r");
	memoryAndFileValidation(fp);

	COLOR_IMAGE* colorImage = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE));

	char buffer[MAX_LINE_LENGTH];
	RGB** pixels;
	bool flag = false;

	int values[3];

	char* num;
	fgets(buffer, MAX_LINE_LENGTH, fp);
	//if(buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);

	if (strlen(buffer) <= 3) {
		fgets(buffer, MAX_LINE_LENGTH, fp);
		//if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
	}

	int i = 0;

	while (i < 3) {
		num = strtok(buffer, " ");
		if (strcmp(num, "P3") != 0) 
			values[i++] = atoi(num);

		if ((strcmp(buffer, num) == 0) && (i != 3))
		{
			if(flag)
				fgets(buffer, MAX_LINE_LENGTH, fp);
			flag = true;	
		}
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
	
}
#include "objects.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

// Allocate a multi RGB array
RGB** memoryAllocationMultiPixelArray(int width, int height) {
	RGB** pixels;
	pixels = (RGB**)malloc(sizeof(RGB*) * height);
	memoryAndFileValidation(pixels);

	for (int i = 0; i < height; i++) {
		// Allocate the width in each row
		pixels[i] = (RGB*)malloc(sizeof(RGB) * width);
		memoryAndFileValidation(pixels[i]);
	}
	return pixels;
}

// Print color image
void printColorImagePixels(COLOR_IMAGE * colorImage) {
	for (int i = 0; i < colorImage->rows; i++) {
		for (int j = 0; j < colorImage->cols; j++) {
			// Print each color level
			printf("%3d ", colorImage->pixels[i][j].r);
			printf("%3d ", colorImage->pixels[i][j].g);
			printf("%3d ", colorImage->pixels[i][j].b);

		}
		printf("\n");
	}
}

// Print gray image
void printGrayImagePixels(GRAY_IMAGE* grayImage) {
	for (int i = 0; i < grayImage->rows; i++) {
		for (int j = 0; j < grayImage->cols; j++) {
			// Print pixel
			printf("%3d ", grayImage->pixels[i][j]);
		}
		printf("\n");
	}
}

// Read ppm file into COLOR_IMAGE struct
COLOR_IMAGE* readPPM(char* fname) {
	FILE* fp = fopen(fname, "r");
	memoryAndFileValidation(fp);

	// Allocate a color image pointer
	COLOR_IMAGE* colorImage = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE));
	memoryAndFileValidation(colorImage);

	// Buffer to read from file
	char buffer[MAX_LINE_LENGTH];
	RGB** pixels;
	bool flag = false;

	// In the values we insert the max value, width and height
	int values[3];

	char* num;
	// Get line
	fgets(buffer, MAX_LINE_LENGTH, fp);
	// Ignore comments
	if(buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);

	// Read next line if first line is P3
	if (strlen(buffer) <= 3) {
		fgets(buffer, MAX_LINE_LENGTH, fp);
		// Ignore comments
		if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
	}

	int i = 0;
	// Get number
	num = strtok(buffer, " ");
	while (i < 3) {
		
		// While number is not null or comment
		while ((num != NULL) && (num[0] == '#')) {
			// get next line
			fgets(buffer, MAX_LINE_LENGTH, fp);
			// get next number
			num = strtok(buffer, " ");
		}

		// If number is a number insert into values array
		if ((num != NULL) && (strcmp(num, "P3") != 0))
			values[i++] = atoi(num);

		// If we don't have all important values
		if (num == NULL && (i != 3))
		{
			fgets(buffer, MAX_LINE_LENGTH, fp);
			while (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		} // skip spaces
		else if(i != 3) num = strtok(NULL, " ");
	}

	// Allocate a pixels multi array
	pixels = memoryAllocationMultiPixelArray(values[WIDTH], values[HEIGHT]);

	int lineLength;
	char* tempValue;

	// Go over the values and insert to rgb array
	for (i = 0; i < values[HEIGHT]; i++) {
		lineLength = (values[WIDTH] * 3 * log10(values[MAX_VALUE]) + (values[WIDTH] * 3) + 2);
		fgets(buffer, lineLength, fp);
		tempValue = strtok(buffer, " ");
		for (int j = 0; j < values[WIDTH]; j++) {
			pixels[i][j].r = atoi(tempValue);
			tempValue = strtok(NULL, " ");
			pixels[i][j].g = atoi(tempValue);
			tempValue = strtok(NULL, " ");
			pixels[i][j].b = atoi(tempValue);
			tempValue = strtok(NULL, " ");
		}

	}

	colorImage->pixels = pixels;
	colorImage->rows = values[HEIGHT];
	colorImage->cols = values[WIDTH];
	

	fclose(fp);
	return colorImage;
}

// Read pgm file into GRAY_IMAGE struct
GRAY_IMAGE* readPGM(char* fname) {
	FILE* fp = fopen(fname, "r");
	memoryAndFileValidation(fp);

	// Allocate gray image pointer
	GRAY_IMAGE* grayImage = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	memoryAndFileValidation(grayImage);

	char buffer[MAX_LINE_LENGTH];
	char** pixels;

	// Important values - max value, height and width
	int values[3];

	char* num;
	// Read line into the buffer
	fgets(buffer, MAX_LINE_LENGTH, fp);
	// Ignore comments
	if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);

	// Read next line if first line is P3
	if (strlen(buffer) <= 3) {
		fgets(buffer, MAX_LINE_LENGTH, fp);
		if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, fp);
	}

	int i = 0;
	// Get number, superate the spaces
	num = strtok(buffer, " ");
	// Find all 3 important values
	while (i < 3) {

		// While number is valid and not comment
		while ((num != NULL) && (num[0] == '#')) {
			fgets(buffer, MAX_LINE_LENGTH, fp);
			num = strtok(buffer, " ");
		}

		// Add value if not equal to P2 header
		if ((num != NULL) && (strcmp(num, "P2") != 0))
			values[i++] = atoi(num);

		// If there are important values left
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

	// Create the pixel array, read from file
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

// Build a pgm file from gray image
void buildPGMFromGrayImage(GRAY_IMAGE* grayImage, char* fname) {

	FILE* fp = fopen(fname, "w");
	memoryAndFileValidation(fp);
	unsigned char max = 0;

	// Print headers to file
	fprintf(fp, "P2\n");
	fprintf(fp, "%d %d\n", grayImage->cols, grayImage->rows);

	// Save the max number location
	int maxLocation = ftell(fp);
	fprintf(fp, "   \n");

	// Go over the pixels and print to file
	for (int i = 0; i < grayImage->rows; i++) {
		for (int j = 0; j < grayImage->cols; j++) {
			if (grayImage->pixels[i][j] > max) max = grayImage->pixels[i][j];
			fprintf(fp, "%3d ", grayImage->pixels[i][j]);
		}
		fprintf(fp, "\n");
	}

	// Print the max value of the pixels
	fseek(fp, maxLocation, SEEK_SET);
	fprintf(fp, "%d", max);

	fclose(fp);

}
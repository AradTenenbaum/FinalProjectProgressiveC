#include "compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include <string.h>

// convert a char(8 bits) to a 7 bit
unsigned char convertCharTo7Bit(unsigned char c, unsigned char next, int offset) {
	unsigned char newChar = c << (offset + 1);
	newChar = newChar + (next >> (7 - (offset + 1)));
	return newChar;
}

// convert a 7 bit to a char(8 bits)
unsigned char convert7BitToChar(unsigned char bit7, unsigned char prev, int offset) {

	unsigned char helper = (255 >> (7 - offset + 1));
	unsigned char prevSave = prev & helper;
	prevSave = prevSave << (7 - offset);

	unsigned char newChar = (bit7 >> (offset + 1)) + prevSave;
	return newChar;
}

// Convert pgm name to bin
char* getNewFileNamePgmToBin(char* fname) {
	int length = strlen(fname);
	char* fnameTemp = malloc(sizeof(char) * length);
	strcpy(fnameTemp, fname);
	fnameTemp[length - 3] = 'b';
	fnameTemp[length - 2] = 'i';
	fnameTemp[length - 1] = 'n';
	return fnameTemp;
}

// Save the compressed gray image to the file
void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel) {

	char* newFileName = getNewFileNamePgmToBin(fname);

	FILE* fp = fopen(newFileName, "wb");
	memoryAndFileValidation(fp);

	unsigned char newPixel;
	unsigned char nextNewPixel;
	unsigned char combinedPixel;
	int offset = 0;
	int count = 0;
	bool isSet = true;

	// Write rows and cols
	fwrite(&(image->rows), sizeof(unsigned short), 1, fp);
	fwrite(&(image->cols), sizeof(unsigned short), 1, fp);

	// Go over the pixels
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			offset = count % 7;

			// if count 7 next char
			if (offset == 0 && !(i == 0 && j == 0) && isSet) isSet = false;
			else isSet = true;

			if (isSet) {

				newPixel = (image->pixels[i][j] * maxGrayLevel) / 255;
				// case: last pixel
				if (i == (image->rows - 1) && j == (image->cols - 1)) nextNewPixel = 0;
				// case: last pixel in row
				else if (j == (image->cols - 1)) nextNewPixel = ((image->pixels[i + 1][0] * maxGrayLevel) / 255);
				// normal case
				else nextNewPixel = ((image->pixels[i][j + 1] * maxGrayLevel) / 255);

				combinedPixel = convertCharTo7Bit(newPixel, nextNewPixel, offset);
				fwrite(&(combinedPixel), sizeof(char), 1, fp);

				count++;
			}

		}
	}


	fclose(fp);
}

// Convert bin name to pgm
char* getNewFileNameBinToPgm(char* fname) {
	int length = strlen(fname);
	char* fnameTemp = malloc(sizeof(char) * length);
	strcpy(fnameTemp, fname);
	fnameTemp[length - 1] = 'm';
	fnameTemp[length - 2] = 'g';
	fnameTemp[length - 3] = 'p';
	return fnameTemp;
}

// Convert compressed file of gray image to a pgm file
void convertCompressedImageToPGM(char* fname) {
	FILE* fpBin = fopen(fname, "rb");
	memoryAndFileValidation(fpBin);

	char* newFileName = getNewFileNameBinToPgm(fname);

	FILE* fpPGM = fopen(newFileName, "w");
	memoryAndFileValidation(fpPGM);

	unsigned short rows;
	unsigned short cols;
	unsigned char value;
	unsigned char prevValue = 0;
	int count = 0;
	int offset = 0;
	unsigned char fixedValue;
	unsigned char maxValue = 0;

	bool isLeftOver = false;
	unsigned char leftOver = 0;

	// read rows and cols
	fread(&rows, sizeof(unsigned short), 1, fpBin);
	fread(&cols, sizeof(unsigned short), 1, fpBin);

	// Print headers
	fprintf(fpPGM, "P2\n");
	fprintf(fpPGM, "%d %d\n", cols, rows);

	// Save the max location in the file
	int maxLocation = ftell(fpPGM);

	// Save location for the max value
	fprintf(fpPGM, "   \n");

	int i = 0;
	int j = 0;

	// Go over the bin file
	while (i < rows) {
		while (j < cols) {

			// Check if there is a byte that we missed while converting
			if (isLeftOver) {
				isLeftOver = false;
				fprintf(fpPGM, "%3d ", leftOver);
				j++;
			}

			offset = count % 7;

			// Get the value
			fread(&value, sizeof(unsigned char), 1, fpBin);

			fixedValue = convert7BitToChar(value, prevValue, offset);

			// Write the value to file
			fprintf(fpPGM, "%3d ", fixedValue);

			// If offset is 6 the convert function misses a value in the 7 bits of the value
			if (offset == 6) {
				if (j < (cols - 1)) {
					// Print to the file the 7 bits value
					fprintf(fpPGM, "%3d ", (value & 127));
					j++;
				}
				else {
					isLeftOver = true;
					leftOver = (value & 127);
				}
			}

			// Save the max value of the pixels
			if (fixedValue > maxValue) maxValue = fixedValue;

			prevValue = value;
			count++;
			j++;
		}
		j = 0;
		i++;
		fprintf(fpPGM, "\n");
	}

	// Get back to max pixel value
	fseek(fpPGM, maxLocation, SEEK_SET);
	// Print max value to the file
	fprintf(fpPGM, "%d", maxValue);

	fclose(fpBin);
	fclose(fpPGM);

}
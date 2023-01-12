#include "compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include <string.h>

// Get the max byte amount from max gray level
int getMaxBytes(unsigned char maxGrayLevel) {
	int counter = 0;
	while (maxGrayLevel > 0) {
		maxGrayLevel = maxGrayLevel >> 1;
		counter++;
	}
	return counter;
}

void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel) {
	
	// Max byte
	int maxBytes = getMaxBytes(maxGrayLevel);
	int locationInCurrByte = 0;
	unsigned char temp;
	bool isNextByte = false;
	int leftOverBits = 0;
	unsigned char helper;
	unsigned char value;

	unsigned char toSave = 0;

	char* newFileName = getNewFileNamePgmToBin(fname);

	FILE* fp = fopen(newFileName, "wb");
	memoryAndFileValidation(fp);


	// Write rows, cols and max gray level
	fwrite(&(image->rows), sizeof(unsigned short), 1, fp);
	fwrite(&(image->cols), sizeof(unsigned short), 1, fp);
	fwrite(&(maxGrayLevel), sizeof(unsigned char), 1, fp);


	// Go over the pixels
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++) {
			// Change the value by the formula
			value = ((image->pixels[i][j] * maxGrayLevel) / 255);

			// check for leftover bits
			leftOverBits = (-1) * (8 - locationInCurrByte - maxBytes);
			// If the current index in byte + bytes amount needed is 8 or more
			// We need to insert a part from the current byte and a part from the next
			isNextByte = ((locationInCurrByte + maxBytes) >= 8);
			if (isNextByte) temp = value >> (-1) * (8 - locationInCurrByte - maxBytes);
			else temp = value << (8 - locationInCurrByte - maxBytes);

			// Calc current index in the byte
			locationInCurrByte = (locationInCurrByte + maxBytes) % 8;
			toSave += temp;

			if (isNextByte) {
				// Write data to memory
				fwrite(&(toSave), sizeof(char), 1, fp);
				toSave = 0;

				helper = 255 >> (8 - leftOverBits);



				helper = value & helper;
				toSave += (helper << (8 - leftOverBits));
				locationInCurrByte = leftOverBits;
			}

		}
	}

	fclose(fp);

}


void convertCompressedImageToPGM(char* fname) {
	FILE* fpBin = fopen(fname, "rb");
	memoryAndFileValidation(fpBin);

	char* newFileName = getNewFileNameBinToPgm(fname);

	FILE* fpPGM = fopen(newFileName, "w");
	memoryAndFileValidation(fpPGM);

	unsigned short rows;
	unsigned short cols;
	unsigned char maxGrayLevel;
	unsigned char helper;
	unsigned char helper2;
	unsigned char temp;
	unsigned char value;
	int i = 0;
	int locationInCurrByte = 0;
	bool isNextByte = false;


	// read rows and cols and maxGrayLevel
	fread(&rows, sizeof(unsigned short), 1, fpBin);
	fread(&cols, sizeof(unsigned short), 1, fpBin);
	fread(&maxGrayLevel, sizeof(unsigned char), 1, fpBin);

	// Get max bytes amount
	int maxBytes = getMaxBytes(maxGrayLevel);
	int size = rows * cols;
	int index = 0;
	unsigned char toSave = 0;

	// Print headers
	fprintf(fpPGM, "P2\n");
	fprintf(fpPGM, "%d %d\n", cols, rows);
	fprintf(fpPGM, "%d\n", maxGrayLevel);


	// Insert all compressed data to array
	// Compressed data must be smaller than amount of pixels
	unsigned char* data = malloc(sizeof(char)* size);
	memoryAndFileValidation(data);

	// Read the value first so the feof will work
	fread(&value, sizeof(unsigned char), 1, fpBin);

	while (feof(fpBin) == 0) {
		// Insert value to array
		data[index] = value;
		// Read the next value
		fread(&value, sizeof(unsigned char), 1, fpBin);
		index++;
	}

	// Un compress the data
	unsigned char* unCompressedArr = unCompress(data, size, maxBytes);

	// Print data to the file
	for (int i = 0; i < size; i++)
	{
		fprintf(fpPGM, "%3d ", unCompressedArr[i]);
		// Enter when we insert a full row
		if (((i+1) % cols == 0) && (i != 0)) fprintf(fpPGM, "\n");
	}

	
	fclose(fpBin);
	fclose(fpPGM);
	free(data);
	free(unCompressedArr);

}

// Un compress the array received with size elements
unsigned char* unCompress(unsigned char* compressedArr, int size, int maxBytes) {
	unsigned char* unCompressedArr = calloc(size, sizeof(unsigned char));
	memoryAndFileValidation(unCompressedArr);

	unsigned char helper;
	unsigned char helper2;
	unsigned char temp;
	int i = 0;
	int locationInCurrByte = 0;
	int unCompressedArrIndex = 0;
	bool isNextByte = false;

	// While array is not full
	while (unCompressedArrIndex < size) {
		// Use to get the bits from the current index in byte
		helper = 255 >> (locationInCurrByte);
		// Check if we need to procceed a byte
		isNextByte = ((8 - maxBytes - locationInCurrByte) <= 0);
		if (isNextByte) {
			// Insert and fix data to temp
			temp = helper & compressedArr[i];
			temp = temp << ((-1) * (8 - maxBytes - locationInCurrByte));
			// Insert to array
			unCompressedArr[unCompressedArrIndex] += temp;
			// Procceed in compressed array
			i++;
			// Add the left bits of the pixel
			helper = 255 >> ((-1) * (8 - maxBytes - locationInCurrByte));
			helper = helper ^ 255;
			temp = compressedArr[i] & helper;
			temp = temp >> (8 - ((-1) * (8 - maxBytes - locationInCurrByte)));
			// Insert to array
			unCompressedArr[unCompressedArrIndex++] += temp;
			// Update current index in byte
			locationInCurrByte = ((-1) * (8 - maxBytes - locationInCurrByte));
			isNextByte = false;
		}
		else {
			// Get the number in the byte
			helper2 = (255 >> (maxBytes + locationInCurrByte));
			helper = helper ^ helper2;
			// Insert it to temp and fix it
			temp = helper & compressedArr[i];
			temp = temp >> (8 - (maxBytes + locationInCurrByte));
			// Insert value to the array
			unCompressedArr[unCompressedArrIndex++] += temp;
			// Update current index in byte
			locationInCurrByte = (locationInCurrByte + maxBytes) % 8;
		}
	}

	return unCompressedArr;

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
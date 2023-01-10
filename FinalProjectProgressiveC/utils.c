#include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "objects.h"

void memoryAndFileValidation(void* ptr) {
	if (ptr == NULL)
	{
		printf("File/Memory allocation falied.\nError: %s\n", strerror(errno));
		exit(1);
	}
}

int log10(int num)
{
	int counter = 0;
	if (num == 0) return 1;
	while (num > 0) {
		counter++;
		num /= 10;
	}
	return counter;
}


char* simplestrtok(char* str, char delimiter)
{
	// for simplicity the function assumes correct input and gets only one delimiter

	static char* save = NULL; // The initialization happens only once!
	char* start;

	/* if the original string has nothing left */
	if (!str && !save)
		return NULL;

	/* if it is the first call */
	if (str != NULL)
		save = str;

	/* find the start of the next token, skip delimiters */

	while (*save && *save == delimiter)
		save++;

	// now save points at the first REAL character which is not a delimiter
	start = save; // this is for the return 

	/* return NULL if nothing left */
	if (*save == '\0') {
		save = NULL;
		return save;
	}

	while (*save && *save != delimiter)
		save++;

	if (*save == delimiter)
		*save = '\0';

	save++;

	return start;
}

char** memoryAllocationMultiCharArray(int width, int height) {
	char** pixels;
	pixels = (char**)malloc(sizeof(char*) * height);
	memoryAndFileValidation(pixels);
	for (int i = 0; i < height; i++) {
		pixels[i] = (char*)malloc(sizeof(char) * width);
		memoryAndFileValidation(pixels[i]);
	}
	return pixels;
}

bool** memoryAllocationMultiBoolArray(int width, int height) {
	bool** flags;
	flags = (bool**)malloc(sizeof(bool*) * height);
	memoryAndFileValidation(flags);
	for (int i = 0; i < height; i++) {
		flags[i] = (bool*)calloc(width, sizeof(bool));
		memoryAndFileValidation(flags[i]);
	}
	return flags;
}

void remove_substring(char* s, char* toremove)
{
	// Get the length of the substring to remove
	int len = strlen(toremove);

	// Pointers to the start and end of the string
	char* start = s, * end = s + strlen(s);

	// Loop through the string until we reach the end
	while (start < end)
	{
		// Check if the substring to remove is found at the current position
		if (strncmp(start, toremove, len) == 0)
		{
			// If it is, shift the rest of the string to the left to overwrite the substring
			memmove(start, start + len, end - start - len + 1);

			// Update the end pointer to reflect the new length of the string
			end -= len;
		}
		else
		{
			// If the substring was not found, move to the next character
			++start;
		}
	}
}


void byte_to_binary(unsigned char x)
{
	unsigned char z;
	for (z = 128; z > 0; z >>= 1)
	{
		printf(((x & z) == z) ? "1" : "0");
	}
	printf("\n");
}
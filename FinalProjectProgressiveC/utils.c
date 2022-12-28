#include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
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

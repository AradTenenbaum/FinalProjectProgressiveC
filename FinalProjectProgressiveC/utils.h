#include <stdbool.h>
#ifndef _UTILS_H
#define _UTILS_H
#define _CRT_SECURE_NO_WARNINGS 

void memoryAndFileValidation(void* ptr);
int log10(int num);
char* simplestrtok(char* str, char delimiter);
char** memoryAllocationMultiCharArray(int width, int height);
bool** memoryAllocationMultiBoolArray(int width, int height);
void remove_substring(char* s, char* toremove);
void byte_to_binary(unsigned char x);


#endif

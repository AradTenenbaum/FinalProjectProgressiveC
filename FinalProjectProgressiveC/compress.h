#pragma once
#include "objects.h"
#ifndef _COMPRESS_H
#define _COMPRESS_H
#define _CRT_SECURE_NO_WARNINGS 

void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel);
void convertCompressedImageToPGM(char* fname);
char* getNewFileNamePgmToBin(char* fname);
char* getNewFileNameBinToPgm(char* fname);
unsigned char* unCompress(unsigned char* compressedArr, int size, int maxBytes);


#endif
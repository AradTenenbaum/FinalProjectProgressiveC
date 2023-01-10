#pragma once
#include "objects.h"
#ifndef _COMPRESS_H
#define _COMPRESS_H
#define _CRT_SECURE_NO_WARNINGS 

void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel);
void convertCompressedImageToPGM(char* fname);
unsigned char convertCharTo7Bit(unsigned char c, unsigned char next, int offset);

#endif
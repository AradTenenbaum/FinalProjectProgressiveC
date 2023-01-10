#include <stdbool.h>
#ifndef _CONVERT_H
#define _CONVERT_H
#define _CRT_SECURE_NO_WARNINGS 

void getPPMValues(FILE* ppm_file, int* values);
void getPGMValues(FILE* pgm_file, int* values);
void convertPPMToPGM(char* fname);
void convertPPMToPGM_Bin(const char* fname);
void convertToP2(char* inputFile, char* outputFile);
void pgm_text_to_binary(char* text_file, char* binary_file);
void convertPGMToBW(char* fname);
void setImage2(GRAY_IMAGE* gi, GRAY_IMAGE* image2, FILE* pgm_file, int height, int width, int max_color);
void setImage3(GRAY_IMAGE* gi, GRAY_IMAGE* image3, FILE* pgm_file, int height, int width, int max_color);
void setImage4(GRAY_IMAGE* gi, GRAY_IMAGE* image4, FILE* pgm_file, int height, int width, int max_color);
void convert_p5_to_p2(const char* input_filename, const char* output_filename);
void convertPGMToBW_Bin(char* fname);

#endif

#include "objects.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "convert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert a ppm file to pgm
void convertPPMToPGM(char* fname)
{
    // Open the PPM file 
    FILE* ppm_file = fopen(fname, "r");
    memoryAndFileValidation(ppm_file);
    COLOR_IMAGE* ppm = readPPM(fname);

    // Get the max depth, width and length
    int values[3];
    getPPMValues(ppm_file,values);

    char pgmName[100];
    strcpy(pgmName, fname);
    strcat(pgmName, ".pgm");

    remove_substring(pgmName, ".ppm");

    //Open the new file which will be the pgm file
    FILE* pgm_file = fopen(pgmName, "w");
    memoryAndFileValidation(pgm_file);

    char magic_number[3];
    int width = values[WIDTH], height = values[HEIGHT], max_color = values[MAX_VALUE];
    // Allocate memory for the PGM image data
    GRAY_IMAGE* image = malloc(sizeof(GRAY_IMAGE) * (width * height * 3));
    memoryAndFileValidation(image);

    // Go over pixels array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = ppm->pixels[i][j].r, g = ppm->pixels[i][j].g, b = ppm->pixels[i][j].b;
            // Calculate the gray value
            int gray = (r + g + b) / 3;
            // Insert value to array
            image[i * width + j].pixels = gray;
        }
    }

    // Print headers to file
    fprintf(pgm_file, "P2\n%d %d\n%d\n", width, height, max_color);

    // Print the gray pixels to file
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(pgm_file, "%3d ", image[i * width + j].pixels);
        }
        fprintf(pgm_file, "\n");
    }
    // Close the PGM and PPM file
    fclose(ppm_file);
    fclose(pgm_file);

    // Free the image data and file name memory
    free(image);

    printf("Convertion Successful!");
   
}

// Get the important values
void getPPMValues(FILE* ppm_file ,int* values)
{
    char buffer[MAX_LINE_LENGTH];
    char* num;
    // Get line from file
    fgets(buffer, MAX_LINE_LENGTH, ppm_file);
    // Ignore comments
    if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, ppm_file);

    // If buffer contain only P3 header read the next line
    if (strlen(buffer) <= 3) {
        fgets(buffer, MAX_LINE_LENGTH, ppm_file);
        if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, ppm_file);
    }

    int i = 0;
    num = strtok(buffer, " ");
    // Get the 3 values
    while (i < 3) {

        while ((num != NULL) && (num[0] == '#')) {
            fgets(buffer, MAX_LINE_LENGTH, ppm_file);
            num = strtok(buffer, " ");
        }

        if ((num != NULL) && (strcmp(num, "P3") != 0))
            values[i++] = atoi(num);

        if (num == NULL && (i != 3))
        {
            fgets(buffer, MAX_LINE_LENGTH, ppm_file);
            while (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, ppm_file);
            num = strtok(buffer, " ");
        }
        else if (i != 3) num = strtok(NULL, " ");
    }
}

// 
void getPGMValues(FILE* pgm_file, int* values)
{
    char buffer[MAX_LINE_LENGTH];
    char* num;
    // Get line from file
    fgets(buffer, MAX_LINE_LENGTH, pgm_file);
    // Ignore comments
    if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, pgm_file);

    // If buffer contain only P2 header read the next line
    if (strlen(buffer) <= 3) {
        fgets(buffer, MAX_LINE_LENGTH, pgm_file);
        if (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, pgm_file);
    }

    int i = 0;
    num = strtok(buffer, " ");
    // Get the values from file
    while (i < 3) 
    {

        while ((num != NULL) && (num[0] == '#')) {
            fgets(buffer, MAX_LINE_LENGTH, pgm_file);
            num = strtok(buffer, " ");
        }

        if ((num != NULL) && (strcmp(num, "P2") != 0))
            values[i++] = atoi(num);

        if (num == NULL && (i != 3))
        {
            fgets(buffer, MAX_LINE_LENGTH, pgm_file);
            while (buffer[0] == '#') fgets(buffer, MAX_LINE_LENGTH, pgm_file);
            num = strtok(buffer, " ");
        }
        else if (i != 3) num = strtok(NULL, " ");

    }
}

// Convert ppm(p6) to pgm(p5)
void convertPPMToPGM_Bin(const char* fname) 
{
    // Prepare name for text file
    char pgmName[100];
    strcpy(pgmName, fname);
    strcat(pgmName, "TEXT.pgm");
    remove_substring(pgmName, ".ppm");

    convertToP2(fname, pgmName);//converting binary ppm file into text pgm file

    char pgmBinName[100];
    strcpy(pgmBinName, pgmName);
    remove_substring(pgmBinName, "TEXT");

    pgm_text_to_binary(pgmName, pgmBinName);//converting text pgm file into binary pgm file

    printf("Convertion Successful!");

}

// Convert a ppm p6 to pgm p2
void convertToP2(char* inputFile, char* outputFile) {
    FILE* in, * out;
    COLOR_IMAGE* image;
    int i, j;

    // Open the input file in binary mode
    in = fopen(inputFile, "rb");
    memoryAndFileValidation(in);

    // Read the P6 header
    char header[3];
    fread(header, sizeof(char), 3, in);
    if (header[0] != 'P' || header[1] != '6') {
        fprintf(stderr, "Error: input file is not a P6 PPM image\n");
        return;
    }

    // Read the image size and allocate memory for the image
    image = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE));
    fscanf(in, "%hu %hu", &image->cols, &image->rows);
    image->pixels = (RGB**)malloc(image->rows * sizeof(RGB*));
    for (i = 0; i < image->rows; i++) {
        image->pixels[i] = (RGB*)malloc(image->cols * sizeof(RGB));
    }

    // Read the pixel data
    fgetc(in); // Read the newline character
    for (i = 0; i < image->rows; i++) {
        for (j = 0; j < image->cols; j++) {
            fread(&image->pixels[i][j], sizeof(RGB), 1, in);
        }
    }

    // Close the input file
    fclose(in);

    // Open the output file in text mode
    out = fopen(outputFile, "w");
    memoryAndFileValidation(out);

    // Write the P2 header
    fprintf(out, "P2\n");
    fprintf(out, "%hu %hu\n", image->cols, image->rows);
    fprintf(out, "255\n");

    // Write the pixel data
    for (i = 0; i < image->rows; i++) {
        for (j = 0; j < image->cols; j++) {
            RGB* rgb = &image->pixels[i][j];
            unsigned char gray = (rgb->r + rgb->g + rgb->b)/3;
            fprintf(out, "%3hhu ", gray);
        }
        fprintf(out, "\n");
    }

    // Close the output file
    fclose(out);

    // Free the image memory
    for (i = 0; i < image->rows; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

// Convert pgm text to binary
void pgm_text_to_binary(char* text_file, char* binary_file) {
    // Open the text file for reading
    FILE* text = fopen(text_file, "r");
    memoryAndFileValidation(text);

    // Open the binary file for writing
    FILE* binary = fopen(binary_file, "w");
    memoryAndFileValidation(binary);

    // Read the PGM file header
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, text);
    fwrite("P5\n", sizeof(char), strlen(buffer), binary);

    fgets(buffer, BUFFER_SIZE, text);
    fwrite(buffer, sizeof(char), strlen(buffer), binary);

    // Read the width, height, and max value from the header
    int width, height, max_value;
    sscanf(buffer, "%d %d", &width, &height);
    fgets(buffer, BUFFER_SIZE, text);
    fwrite(buffer, sizeof(char), strlen(buffer), binary);
    sscanf(buffer, "%d", &max_value);

    // Write the width, height, and max value to the binary file
    fwrite(&width, sizeof(int), 1, binary);
    fwrite(&height, sizeof(int), 1, binary);
    fwrite(&max_value, sizeof(int), 1, binary);

    // Read the pixel data from the text file and write it to the binary file
    int i, j,c;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(text, "%d", &c);
            fwrite(&c, sizeof(char), 1, binary);
        }
    }

    // Close both files
    fclose(text);
    fclose(binary);
}

// Convert pgm file to black and white
void convertPGMToBW(char* fname)
{
    GRAY_IMAGE* gi;
    int values[3];
    char BWFILE2[100];
    char BWFILE3[100];
    char BWFILE4[100];
 
    strcpy(BWFILE2, fname);
    strcpy(BWFILE3, fname);
    strcpy(BWFILE4, fname);

    // Fix file names
    remove_substring(BWFILE2, ".pgm");
    remove_substring(BWFILE3, ".pgm");
    remove_substring(BWFILE4, ".pgm");
    remove_substring(BWFILE2, "Text");
    remove_substring(BWFILE3, "Text");
    remove_substring(BWFILE4, "Text");

    strcat(BWFILE2, "bw2.pgm");
    strcat(BWFILE3, "bw3.pgm");
    strcat(BWFILE4, "bw4.pgm");

    FILE* pgm_file = fopen(fname, "r");
    memoryAndFileValidation(pgm_file);

    // Get the max depth, width and height from the pgm file
    getPGMValues(pgm_file, values);

    FILE* pgmBW2_file = fopen(BWFILE2, "w");
    memoryAndFileValidation(pgmBW2_file);
    FILE* pgmBW3_file = fopen(BWFILE3, "w");
    memoryAndFileValidation(pgmBW2_file);
    FILE* pgmBW4_file = fopen(BWFILE4, "w");
    memoryAndFileValidation(pgmBW4_file);

    gi = readPGM(fname);

    int width = values[WIDTH], height = values[HEIGHT], max_color = values[MAX_VALUE];

    // Allocate memory for the PGMBW image data
    GRAY_IMAGE* image2 = malloc(sizeof(GRAY_IMAGE) * (width * height * 3));
    memoryAndFileValidation(image2);
    GRAY_IMAGE* image3 = malloc(sizeof(GRAY_IMAGE) * (width * height * 3));
    memoryAndFileValidation(image3);
    GRAY_IMAGE* image4 = malloc(sizeof(GRAY_IMAGE) * (width * height * 3));
    memoryAndFileValidation(image4);

    setImage2(gi, image2, pgm_file, height, width, max_color);
    setImage3(gi, image3, pgm_file, height, width, max_color);
    setImage4(gi, image4, pgm_file, height, width, max_color);

    // Print headers to the new files
    fprintf(pgmBW2_file, "P2\n%d %d\n%d\n", width, height, max_color);
    fprintf(pgmBW3_file, "P2\n%d %d\n%d\n", width, height, max_color);
    fprintf(pgmBW4_file, "P2\n%d %d\n%d\n", width, height, max_color);

    // Print pixels into the files
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(pgmBW2_file, "%d ", image2[i * width + j].pixels);
            fprintf(pgmBW3_file, "%d ", image3[i * width + j].pixels);
            fprintf(pgmBW4_file, "%d ", image4[i * width + j].pixels);
        }
        fprintf(pgmBW2_file, "\n");
        fprintf(pgmBW3_file, "\n");
        fprintf(pgmBW4_file, "\n");

    }
    // Close the PGM and PPM file
    fclose(pgm_file);
    fclose(pgmBW2_file);
    fclose(pgmBW3_file);
    fclose(pgmBW4_file);


    // Free the image data and file name memory
    free(image2);
    free(image3);
    free(image4);

    printf("Convertion Successful!");

}

// Convert each pixel to 1 or 0
void setImage2(GRAY_IMAGE* gi ,GRAY_IMAGE* image2, FILE* pgm_file, int height, int width, int max_color)
{
    int mask2[] = { 0,2,3,1 };
    int count = 0;
    int D = max_color / 4;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int B;
            // Check pixel range
            if (gi->pixels[i][j] >= 0 && gi->pixels[i][j] <= count + D)
                B = 0;
            if (gi->pixels[i][j] > count + D && gi->pixels[i][j] <= count + 2 * D)
                B = 1;
            if (gi->pixels[i][j] > count + 2 * D && gi->pixels[i][j] <= count + 3 * D)
                B = 2;
            if (gi->pixels[i][j] > count + 3 * D && gi->pixels[i][j] <= 255)
                B = 3;
            image2[i * width + j].pixels = B;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < 2 || j < 2)
                image2[i * width + j].pixels = image2[i * width + j].pixels > mask2[i * 2 + j] ? 1 : 0;
            else
            {
                int BW2 = image2[i * width + j].pixels > mask2[(i % 2) * 2 + (j % 2)] ? 1 : 0;
                image2[i * width + j].pixels = BW2;
            }
        }
    }
}

// Convert each pixel to 1 or 0
void setImage3(GRAY_IMAGE* gi, GRAY_IMAGE* image3, FILE* pgm_file, int height, int width, int max_color)
{
    int mask3[] = { 2,6,4,5,0,1,8,3,7 };
    int count = 0;
    int D = max_color / 9;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int W;
            // Check pixel range
            if (gi->pixels[i][j] >= 0 && gi->pixels[i][j] <= count + D)
                W = 0;
            if (gi->pixels[i][j] > count + D && gi->pixels[i][j] <= count + 2 * D)
                W = 1;
            if (gi->pixels[i][j] > count + 2 * D && gi->pixels[i][j] <= count + 3 * D)
                W = 2;
            if (gi->pixels[i][j] > count + 3 * D && gi->pixels[i][j] <= count + 4 * D)
                W = 3;
            if (gi->pixels[i][j] > count + 4 * D && gi->pixels[i][j] <= count + 5 * D)
                W = 4;
            if (gi->pixels[i][j] > count + 5 * D && gi->pixels[i][j] <= count + 6 * D)
                W = 5;
            if (gi->pixels[i][j] > count + 6 * D && gi->pixels[i][j] <= count + 7 * D)
                W = 6;
            if (gi->pixels[i][j] > count + 7 * D && gi->pixels[i][j] <= count + 8 * D)
                W = 7;
            if (gi->pixels[i][j] > count + 8 * D && gi->pixels[i][j] <= 255)
                W = 8;

            image3[i * width + j].pixels = W;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < 3 || j < 3)
                image3[i * width + j].pixels = image3[i * width + j].pixels > mask3[i * 3 + j] ? 1 : 0;
            else
            {
                int BW3 = image3[i * width + j].pixels > mask3[(i % 3) * 3 + (j % 3)] ? 1 : 0;
                image3[i * width + j].pixels = BW3;
            }
        }
    }
}

// Convert each pixel to 1 or 0
void setImage4(GRAY_IMAGE* gi, GRAY_IMAGE* image4, FILE* pgm_file, int height, int width, int max_color)
{
    int mask4[] = { 0,8,2,10,12,4,14,6,3,11,1,9,15,7,13,5 };
    int count = 0;
    int D = max_color / 16;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r;
            // Check pixel range
            if (gi->pixels[i][j] >= 0 && gi->pixels[i][j] <= count + D)
                r = 0;
            if (gi->pixels[i][j] > count + D && gi->pixels[i][j] <= count + 2 * D)
                r = 1;
            if (gi->pixels[i][j] > count + 2 * D && gi->pixels[i][j] <= count + 3 * D)
                r = 2;
            if (gi->pixels[i][j] > count + 3 * D && gi->pixels[i][j] <= count + 4 * D)
                r = 3;
            if (gi->pixels[i][j] > count + 4 * D && gi->pixels[i][j] <= count + 5 * D)
                r = 4;
            if (gi->pixels[i][j] > count + 5 * D && gi->pixels[i][j] <= count + 6 * D)
                r = 5;
            if (gi->pixels[i][j] > count + 6 * D && gi->pixels[i][j] <= count + 7 * D)
                r = 6;
            if (gi->pixels[i][j] > count + 7 * D && gi->pixels[i][j] <= count + 8 * D)
                r = 7;
            if (gi->pixels[i][j] > count + 8 * D && gi->pixels[i][j] <= count + 9 * D)
                r = 8;
            if (gi->pixels[i][j] > count + 9 * D && gi->pixels[i][j] <= count + 10 * D)
                r = 9;
            if (gi->pixels[i][j] > count + 10 * D && gi->pixels[i][j] <= count + 11 * D)
                r = 10;
            if (gi->pixels[i][j] > count + 11 * D && gi->pixels[i][j] <= count + 12 * D)
                r = 11;
            if (gi->pixels[i][j] > count + 12 * D && gi->pixels[i][j] <= count + 13 * D)
                r = 12;
            if (gi->pixels[i][j] > count + 13 * D && gi->pixels[i][j] <= count + 14 * D)
                r = 13;
            if (gi->pixels[i][j] > count + 14 * D && gi->pixels[i][j] <= count + 15 * D)
                r = 14;
            if (gi->pixels[i][j] > count + 15 * D && gi->pixels[i][j] <= 255)
                r = 15;
            image4[i * width + j].pixels = r;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i < 4 || j < 4)
                image4[i * width + j].pixels = image4[i * width + j].pixels > mask4[i * 4 + j] ? 1 : 0;
            else
            {
                int BW4 = image4[i * width + j].pixels > mask4[(i % 4) * 4 + (j % 4)] ? 1 : 0;
                image4[i * width + j].pixels = BW4;
            }
        }
    }
}

// Convert pgm p5 to pgm p2
void convert_p5_to_p2(const char* input_filename, const char* output_filename) 
{
    FILE* in, * out;
    GRAY_IMAGE* image;
    int i, j;

    // Open the input file in binary mode
    in = fopen(input_filename, "rb");
    memoryAndFileValidation(in);

    // Read the P6 header
    char header[3];
    fread(header, sizeof(char), 3, in);
    if (header[0] != 'P' || header[1] != '5') {
        fprintf(stderr, "Error: input file is not a P5 PGM image\n");
        return;
    }

    // Read the image size
    image = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
    fscanf(in, "%hu %hu", &image->cols, &image->rows);
    image->pixels = (char**)malloc(image->rows * sizeof(char*));
    for (i = 0; i < image->rows; i++) {
        image->pixels[i] = (char*)malloc(image->cols * sizeof(char));
    }

    // Read the pixel data
    fgetc(in); // Read the newline character
    for (i = 0; i < image->rows; i++) {
        for (j = 0; j < image->cols; j++) {
            fread(&image->pixels[i][j], 1, 1, in);
        }
    }

    // Close the input file
    fclose(in);

    // Open the output file in text mode
    out = fopen(output_filename, "w");
    memoryAndFileValidation(out);

    // Write the P2 header
    fprintf(out, "P2\n");
    fprintf(out, "%hu %hu\n", image->cols, image->rows);
    fprintf(out, "255\n");

    // Write the pixel data
    for (i = 0; i < image->rows; i++) {
        for (j = 0; j < image->cols; j++) {
            fprintf(out, "%3hhu ", image->pixels[i][j]);
        }
        fprintf(out, "\n");
    }

    // Close the output file
    fclose(out);

    // Free the image memory
    for (i = 0; i < image->rows; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);

}

// Convert pgm p5 to black and white binary
void convertPGMToBW_Bin(char* fname)
{
    char fnameTextFile[100];
    char fnameTextFileBW2[100];
    char fnameTextFileBW3[100];
    char fnameTextFileBW4[100];
    char fnameBinaryFileBW2[100];
    char fnameBinaryFileBW3[100];
    char fnameBinaryFileBW4[100];

    // Fix the names
    strcpy(fnameTextFile, fname);
    remove_substring(fnameTextFile, ".pgm");
    strcat(fnameTextFile, "Text.pgm");

    // Convert p5 to p2
    convert_p5_to_p2(fname, fnameTextFile);
    // Convert pgm p2 to black and white
    convertPGMToBW(fnameTextFile);

    strcpy(fnameTextFileBW2, fnameTextFile);
    strcpy(fnameTextFileBW3, fnameTextFile);
    strcpy(fnameTextFileBW4, fnameTextFile);

    remove_substring(fnameTextFileBW2, "Text.pgm");
    remove_substring(fnameTextFileBW3, "Text.pgm");
    remove_substring(fnameTextFileBW4, "Text.pgm");

    strcat(fnameTextFileBW2, "bw2.pgm");
    strcat(fnameTextFileBW3, "bw3.pgm");
    strcat(fnameTextFileBW4, "bw4.pgm");

    strcpy(fnameBinaryFileBW2, fnameTextFileBW2);
    remove_substring(fnameBinaryFileBW2, "bw2.pgm");
    strcat(fnameBinaryFileBW2, "bw2.bin");

    strcpy(fnameBinaryFileBW3, fnameTextFileBW3);
    remove_substring(fnameBinaryFileBW3, "bw3.pgm");
    strcat(fnameBinaryFileBW3, "bw3.bin");

    strcpy(fnameBinaryFileBW4, fnameTextFileBW4);
    remove_substring(fnameBinaryFileBW4, "bw4.pgm");
    strcat(fnameBinaryFileBW4, "bw4.bin");

    // Convert each black and white pgm p2 file to p5 
    pgm_text_to_binary(fnameTextFileBW2, fnameBinaryFileBW2);
    pgm_text_to_binary(fnameTextFileBW3, fnameBinaryFileBW3);
    pgm_text_to_binary(fnameTextFileBW4, fnameBinaryFileBW4);


}
//
// Created by Anson on 6/5/2024.
//

#ifndef C_IMAGE_EDITOR_BMP_H
#define C_IMAGE_EDITOR_BMP_H
#include <stdint.h>

#pragma pack(1)

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColors;
} BMPInfoHeader;

#pragma pack()

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} Pixel;

typedef struct {
    BMPHeader header;
    BMPInfoHeader infoHeader;
    Pixel** pixels;
} BMPImage;

BMPImage fileToImage(const char* path);
int ImageToFile(const char* path, BMPImage* image);
int freePixels(BMPImage* image);
int copyImage(BMPImage* dest, BMPImage* src);
void printInfo(BMPImage* image, const char* path);
#endif //C_IMAGE_EDITOR_BMP_H

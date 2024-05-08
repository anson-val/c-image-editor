//
// Created by Anson on 6/5/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "bmp.h"

int validateImage(BMPImage* image) {
    if (image->header.type != 0x4D42) {
        fprintf(stderr, "Source file is not a BMP file");
        exit(1);
    }
    if (image->infoHeader.bitsPerPixel != 24 && image->infoHeader.bitsPerPixel != 32 || image->infoHeader.compression != 0) {
        fprintf(stderr, "Source file is not an uncompressed 24-bit or 32-bit BMP file");
        exit(1);
    }
    return 0;
}

BMPImage fileToImage(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        goto readError;
    }

    BMPImage image;
    fread(&image.header, sizeof(BMPHeader), 1, fp);
    fread(&image.infoHeader, sizeof(BMPInfoHeader), 1, fp);

    if (ferror(fp)) {
        goto readError;
    }

    validateImage(&image);

    const int bytesPerPixel = image.infoHeader.bitsPerPixel / 8;

    fseek(fp, (long) image.header.offset, SEEK_SET);
    image.pixels = malloc(image.infoHeader.height * sizeof(Pixel*));
    if (!image.pixels) {
        goto memoryError;
    }

    for (int i = image.infoHeader.height - 1; i >= 0; i--) {
        image.pixels[i] = malloc(image.infoHeader.width * sizeof(Pixel));
        if (!image.pixels[i]) {
            goto memoryError;
        }
        for (int j = 0; j < image.infoHeader.width; j++) {
            fread(&image.pixels[i][j], bytesPerPixel, 1, fp);
        }
        fseek(fp, (long) (4 - (image.infoHeader.width * bytesPerPixel) % 4) % 4, SEEK_CUR);
        if (ferror(fp)) {
            goto readError;
        }
    }

    fclose(fp);
    return image;

    memoryError:
    perror("Memory allocation");
    if (fp) {
        fclose(fp);
    }
    exit(1);

    readError:
    perror("Unable to read source file or it is corrupted");
    if (fp) {
        fclose(fp);
    }
    exit(1);
}

int ImageToFile(const char* path, BMPImage* image) {
    const int bytesPerPixel = image->infoHeader.bitsPerPixel / 8;

    image->infoHeader.imageSize = (image->infoHeader.width * bytesPerPixel + (4 - (image->infoHeader.width * bytesPerPixel) % 4) % 4) * image->infoHeader.height;
    image->infoHeader.size = sizeof(BMPInfoHeader);
    image->header.offset = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    image->header.size = image->infoHeader.imageSize + sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    FILE* fp = fopen(path, "wb");
    if (!fp) {
        goto writeError;
    }

    ftruncate(fileno(fp), (long) image->header.size);
    fwrite(&image->header, sizeof(BMPHeader), 1, fp);
    fwrite(&image->infoHeader, sizeof(BMPInfoHeader), 1, fp);

    if (ferror(fp)) {
        goto writeError;
    }

    for (int i = image->infoHeader.height - 1; i >= 0; i--) {
        for (int j = 0; j < image->infoHeader.width; j++) {
            fwrite(&image->pixels[i][j], bytesPerPixel, 1, fp);
        }
        fseek(fp, (long) (4 - (image->infoHeader.width * bytesPerPixel) % 4) % 4, SEEK_CUR);
        if (ferror(fp)) {
            goto writeError;
        }
    }

    fclose(fp);
    return 0;

    writeError:
    perror("Unable to write destination file");
    if (fp) {
        fclose(fp);
    }
    exit(1);
}

int freePixels(BMPImage* image) {
    for (int i = 0; i < image->infoHeader.height; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    return 0;
}

int copyImage(BMPImage* dest, BMPImage* src) {
    dest->header = src->header;
    dest->infoHeader = src->infoHeader;
    dest->pixels = malloc(dest->infoHeader.height * sizeof(Pixel*));
    if (!dest->pixels) {
        goto memoryError;
    }

    for (int i = 0; i < dest->infoHeader.height; i++) {
        dest->pixels[i] = malloc(dest->infoHeader.width * sizeof(Pixel));
        if (!dest->pixels[i]) {
            goto memoryError;
        }
        memcpy(dest->pixels[i], src->pixels[i], dest->infoHeader.width * sizeof(Pixel));
    }
    return 0;

    memoryError:
    perror("Memory allocation");
    exit(1);
}

void printInfo(BMPImage* image, const char* path) {
    printf("Properties of %s\n\n", path);
    printf("File size: %d byte(s)\n", image->header.size);
    printf("Image offset: %d byte(s)\n\n", image->header.offset);
    printf("DIB Header size: %d byte(s)\n", image->infoHeader.size);
    printf("Image resolution: %d x %d pixel(s)\n", image->infoHeader.width, image->infoHeader.height);
    printf("Number of color planes: %d plane(s)\n", image->infoHeader.planes);
    printf("Bits per pixel: %d bit(s)\n", image->infoHeader.bitsPerPixel);
    printf("Raw image size: %d byte(s)\n", image->infoHeader.imageSize);
    printf("Print resolution: %d x %d pixels per meter\n", image->infoHeader.xPixelsPerMeter, image->infoHeader.yPixelsPerMeter);
    printf("Number of colors in palette: %d color(s)\n", image->infoHeader.colorsUsed);
    printf("Number of important colors: %d color(s)\n", image->infoHeader.importantColors);
}
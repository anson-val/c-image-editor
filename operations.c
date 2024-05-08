//
// Created by Anson on 6/5/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"
#include "operations.h"

int applyCrop(BMPImage* image, Operation* operation) {
    if (operation->xStart < 0 || operation->yStart < 0 || operation->xStart + operation->width > image->infoHeader.width || operation->yStart + operation->height > image->infoHeader.height || operation->width <= 0 || operation->height <= 0) {
        fprintf(stderr, "Crop: Range exceeds image resolution\n");
        exit(1);
    }

    BMPImage originalImage;
    copyImage(&originalImage, image);
    freePixels(image);
    image->pixels = malloc(operation->height * sizeof(Pixel*));
    for (int i = 0; i < operation->height; i++) {
        image->pixels[i] = malloc(operation->width * sizeof(Pixel));
        if (!image->pixels[i]) {
            goto memoryError;
        }
        memcpy(image->pixels[i], &originalImage.pixels[operation->yStart + i][operation->xStart], operation->width * sizeof(Pixel));
    }
    image->infoHeader.height = operation->height;
    image->infoHeader.width = operation->width;
    freePixels(&originalImage);
    return 0;

    memoryError:
    perror("Memory allocation");
    exit(1);
}

int applyReflectHor(BMPImage* image, Operation* operation){
    for (int i = 0; i < image->infoHeader.height; i++) {
        for (int j = 0; j < image->infoHeader.width / 2; j++) {
            Pixel temp = image->pixels[i][j];
            image->pixels[i][j] = image->pixels[i][image->infoHeader.width - j - 1];
            image->pixels[i][image->infoHeader.width - j - 1] = temp;
        }
    }
    return 0;
}

int applyReflectVert(BMPImage* image, Operation* operation){
    for (int i = 0; i < image->infoHeader.height / 2; i++) {
        Pixel* temp = image->pixels[i];
        image->pixels[i] = image->pixels[image->infoHeader.height - i - 1];
        image->pixels[image->infoHeader.height - i - 1] = temp;
    }
    return 0;
}

int applyGrayscale(BMPImage* image, Operation* operation) {
    for (int i = 0; i < image->infoHeader.height; i++) {
        for (int j = 0; j < image->infoHeader.width; j++) {
            Pixel* pixel = &image->pixels[i][j];
            double average = (pixel->red + pixel->green + pixel->blue) / 3.0;
            pixel->red = pixel->green = pixel->blue = (uint8_t) round(average);
        }
    }
    return 0;
}

int applyRotateAnti(BMPImage* image) {
    BMPImage originalImage;
    copyImage(&originalImage, image);
    freePixels(image);

    image->infoHeader.height = originalImage.infoHeader.width;
    image->infoHeader.width = originalImage.infoHeader.height;
    image->pixels = malloc(image->infoHeader.height * sizeof(Pixel*));
    for (int i = 0; i < image->infoHeader.height; i++) {
        image->pixels[i] = malloc(image->infoHeader.width * sizeof(Pixel));
        if (!image->pixels[i]) {
            goto memoryError;
        }
    }

    for (int row = 0; row < originalImage.infoHeader.height; row++) {
        for (int col = 0; col < originalImage.infoHeader.width; col++) {
            image->pixels[originalImage.infoHeader.width - col - 1][row] = originalImage.pixels[row][col];
        }
    }

    freePixels(&originalImage);
    return 0;

    memoryError:
    perror("Memory allocation");
    exit(1);
}

int applyRotate(BMPImage* image, Operation* operation) {
    if (operation->angle % 90 != 0) {
        fprintf(stderr, "Rotate: Angle must be a multiple of 90\n");
        exit(1);
    }

    int rotations = (360 + (operation->angle % 360)) % 360 / 90;

    for (int i = 0; i < rotations; i++) {
        applyRotateAnti(image);
    }
    return 0;
}

int applyBoxBlur(BMPImage* image, Operation* operation) {
    if (operation->radius <= 0) {
        fprintf(stderr, "Box-blur: Radius must be greater than 0\n");
        exit(1);
    }

    BMPImage originalImage;
    copyImage(&originalImage, image);

    for (int i = 0; i < image->infoHeader.height; i++) {
        originalImage.pixels[i] = malloc(image->infoHeader.width * sizeof(Pixel));
        if (!originalImage.pixels[i]) {
            goto memoryError;
        }
        memcpy(originalImage.pixels[i], image->pixels[i], image->infoHeader.width * sizeof(Pixel));
    }

    for (int row = 0; row < image->infoHeader.height; row++) {
        for (int col = 0; col < image->infoHeader.width; col++) {
            int sumRed = image->pixels[row][col].red, sumGreen = image->pixels[row][col].green, sumBlue = image->pixels[row][col].blue;
            int countSurrounding = 1;

            for (int yShift = 1; yShift <= operation->radius; yShift++) {
                int y = row + yShift;
                if (y >= image->infoHeader.height) {
                    break;
                }
                for (int xShift = -1; xShift >= -operation->radius; xShift--) {
                    int x = col + xShift;
                    if (x < 0) {
                        break;
                    }
                    sumRed += originalImage.pixels[y][x].red;
                    sumGreen += originalImage.pixels[y][x].green;
                    sumBlue += originalImage.pixels[y][x].blue;
                    countSurrounding++;
                }

                for (int xShift = 1; xShift <= operation->radius; xShift++) {
                    int x = col + xShift;
                    if (x >= image->infoHeader.width) {
                        break;
                    }
                    sumRed += originalImage.pixels[y][x].red;
                    sumGreen += originalImage.pixels[y][x].green;
                    sumBlue += originalImage.pixels[y][x].blue;
                    countSurrounding++;
                }
            }

            for (int yShift = -1; yShift >= -operation->radius; yShift--) {
                int y = row + yShift;
                if (y < 0) {
                    break;
                }
                for (int xShift = -1; xShift >= -operation->radius; xShift--) {
                    int x = col + xShift;
                    if (x < 0) {
                        break;
                    }
                    sumRed += originalImage.pixels[y][x].red;
                    sumGreen += originalImage.pixels[y][x].green;
                    sumBlue += originalImage.pixels[y][x].blue;
                    countSurrounding++;
                }

                for (int xShift = 1; xShift <= operation->radius; xShift++) {
                    int x = col + xShift;
                    if (x >= image->infoHeader.width) {
                        break;
                    }
                    sumRed += originalImage.pixels[y][x].red;
                    sumGreen += originalImage.pixels[y][x].green;
                    sumBlue += originalImage.pixels[y][x].blue;
                    countSurrounding++;
                }
            }

            image->pixels[row][col].red = (uint8_t) round((double) sumRed / countSurrounding);
            image->pixels[row][col].green = (uint8_t) round((double) sumGreen / countSurrounding);
            image->pixels[row][col].blue = (uint8_t) round((double) sumBlue / countSurrounding);
        }
    }

    freePixels(&originalImage);
    return 0;

    memoryError:
    perror("Memory allocation");
    exit(1);
}

int applySobel(BMPImage* image, Operation* operation) {
    BMPImage originalImage;
    copyImage(&originalImage, image);
    const int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int row = 0; row < image->infoHeader.height; row++) {
        for (int col = 0; col < image-> infoHeader.width; col++) {
            double GxRed = 0, GxGreen = 0, GxBlue = 0, GyRed = 0, GyGreen = 0, GyBlue = 0;

            for (int yShift = -1; yShift <= 1 ; yShift++)
            {
                for (int xShift = -1; xShift <= 1; xShift++)
                {
                    if (row + yShift < 0 || row + yShift >= image->infoHeader.height)
                    {
                        continue;
                    }
                    if (col + xShift < 0 || col + xShift >= image-> infoHeader.width)
                    {
                        continue;
                    }
                    GxRed += originalImage.pixels[row + yShift][col + xShift].red * Gx[yShift + 1][xShift + 1];
                    GxGreen += originalImage.pixels[row + yShift][col + xShift].green * Gx[yShift + 1][xShift + 1];
                    GxBlue += originalImage.pixels[row + yShift][col + xShift].blue * Gx[yShift + 1][xShift + 1];
                    GyRed += originalImage.pixels[row + yShift][col + xShift].red * Gy[yShift + 1][xShift + 1];
                    GyGreen += originalImage.pixels[row + yShift][col + xShift].green * Gy[yShift + 1][xShift + 1];
                    GyBlue += originalImage.pixels[row + yShift][col + xShift].blue * Gy[yShift + 1][xShift + 1];
                }
            }
            Pixel newPixel = {round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue)) > 255 ? 255 : (uint8_t) round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue)), round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen)) > 255 ? 255 : (uint8_t) round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen)), round(sqrt(GxRed * GxRed + GyRed * GyRed)) > 255 ? 255 : (uint8_t) round(sqrt(GxRed * GxRed + GyRed * GyRed))};
            image->pixels[row][col] = newPixel;
        }
    }

    return 0;
}

int (*applyOperation(OperationType type))(BMPImage* image, Operation* operation) {
    switch (type) {
        case CROP:
            return applyCrop;
        case ROTATE:
            return applyRotate;
        case REFLECT_HOR:
            return applyReflectHor;
        case REFLECT_VERT:
            return applyReflectVert;
        case BOX_BLUR:
            return applyBoxBlur;
        case GRAYSCALE:
            return applyGrayscale;
        case SOBEL:
            return applySobel;
        default:
            return NULL;
    }
}

void printOperation(Operation* operation) {
    switch (operation->type) {
        case CROP:
            printf("✂️ Cropping with the range: xStart=%d, yStart=%d, width=%d, height=%d\n", operation->xStart, operation->yStart, operation->width, operation->height);
            break;
        case ROTATE:
            printf("🔁 Rotating by %d° degrees\n", operation->angle);
            break;
        case REFLECT_HOR:
            printf("🪞 Reflecting horizontally\n");
            break;
        case REFLECT_VERT:
            printf("🪞 Reflecting vertically\n");
            break;
        case BOX_BLUR:
            printf("😶‍🌫️ Box-blurring with a radius of %d\n", operation->radius);
            break;
        case GRAYSCALE:
            printf("⚫ Applying grayscale filter\n");
            break;
        case SOBEL:
            printf("🌅 Edge-detecting with Sobel operator\n");
            break;
        default:
            return;
    }
}
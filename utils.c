//
// Created by Anson on 7/5/2024.
//

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseRange(char* str, Operation* operation) {
    char yRange[1000], xRange[1000];
    char *endPtr;
    // Separate the x and y ranges
    char* token = strtok(str, ",");
    if (token == NULL) {
        return 1;
    }
    strcpy(xRange, token);

    token = strtok(NULL, ",");
    if (token == NULL) {
        return 1;
    }
    strcpy(yRange, token);

    // Parse x ranges
    token = strtok(xRange, "-");
    int xStart = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        goto byResolutionX;
    }
    operation->xStart = xStart;

    token = strtok(NULL, "-");
    int xEnd = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        goto byResolutionX;
    }
    operation->width = xEnd - operation->xStart + 1;
    goto byIntervalY;

    byResolutionX:
    token = strtok(xRange, "+");
    xStart = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        return 1;
    }
    operation->xStart = xStart;

    token = strtok(NULL, "+");
    int width = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        return 1;
    }
    operation->width = width;

    // Parse y ranges
    byIntervalY:
    token = strtok(yRange, "-");
    int yStart = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        goto byResolutionY;
    }
    operation->yStart = yStart;

    token = strtok(NULL, "-");
    int yEnd = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        goto byResolutionY;
    }
    operation->height = yEnd - operation->yStart + 1;
    return 0;

    byResolutionY:
    token = strtok(yRange, "+");
    yStart = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        return 1;
    }
    operation->yStart = yStart;

    token = strtok(NULL, "+");
    int height = strtol(token, &endPtr, 10);
    if (token == NULL || *endPtr != '\0') {
        return 1;
    }
    operation->height = height;

    return 0;
}

void printHelp() {
    printf("OVERVIEW: A very simple C uncompressed 24-bit/32-bit BMP image editor\n\n"
           "USAGE:                  c-image-editor -f <input> -o <output> [options]\n"
           "USAGE FOR PROPERTIES:   c-image-editor -f <input> -i\n\n"
           "OPTIONS:\n"
           "   -c, --crop <range>          Crop the image\n"
           "   -r, --rotate <angle>        Rotate the image\n"
           "   -x, --reflect-x             Reflect the image horizontally\n"
           "   -y, --reflect-y             Reflect the image vertically\n"
           "   -b, --box-blur <radius>     Apply a box blur to the image\n"
           "   -g, --grayscale             Convert the image to grayscale\n"
           "   -s, --sobel                 Apply the Sobel operator to the image\n"
           "   -i, --info                  Display the properties of the image\n"
           "   -h, --help                  Display this help message\n"
           "   -v, --version               Display version information\n\n"
           "RANGE FORMAT:\n"
           "   By interval: <start-column>-<end-column>,<start-row>-<end-row>\n"
           "   By resolution: <start-column>+<width>,<start-row>+<height>\n"
           "   Mixed: <start-column>+<width>,<start-row>-<end-row>\n\n"
           );
}


void printVersion() {
    printf("C Image Editor version 0.1.2\n"
           "Created by Anson Ng on 8/5/2024\n"
    );
}
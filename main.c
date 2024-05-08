#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include "operations.h"
#include "bmp.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    int option, optionIndex = 0;
    Operation optionStack[1024];
    Operation *optionStackPtr = optionStack;
    const char* optionString = "c:r:b:gf:o:xyshvi";
    char inputPath[4100], outputPath[4100];
    char *endPtr;
    bool isCheckInfo = false;

    struct option longOptions[] = {
            {"crop", required_argument, NULL, 'c'},
            {"rotate", required_argument, NULL, 'r'},
            {"reflect-horizontal", no_argument, NULL, 'x'},
            {"reflect-vertical", no_argument, NULL, 'y'},
            {"box-blur", required_argument, NULL, 'b'},
            {"grayscale", no_argument, NULL, 'g'},
            {"sobel", no_argument, NULL, 's'},
            {"input", required_argument, NULL, 'f'},
            {"output", required_argument, NULL, 'o'},
            {"info", no_argument, NULL, 'i'},
            {"help", no_argument, NULL, 'h'},
            {"version", no_argument, NULL, 'v'},
            {NULL, 0, NULL, 0}
    };

    while ((option = getopt_long(argc, argv, optionString, longOptions, &optionIndex)) != -1) {
        switch (option) {
            case 'c':
                optionStackPtr->type = CROP;
                if (parseRange(optarg, optionStackPtr)) {
                    fprintf(stderr, "Range: Invalid range format, -h for accepted formats\n");
                    return 1;
                }
                optionStackPtr++;
                break;
            case 'r':
                optionStackPtr->type = ROTATE;
                optionStackPtr->angle = strtol(optarg, &endPtr, 10);
                if (*endPtr != '\0') {
                    fprintf(stderr, "Rotate: Angle must be an integer that is a multiple of 90\n");
                    return 1;
                }
                optionStackPtr++;
                break;
            case 'x':
                optionStackPtr->type = REFLECT_HOR;
                optionStackPtr++;
                break;
            case 'y':
                optionStackPtr->type = REFLECT_VERT;
                optionStackPtr++;
                break;
            case 'b':
                optionStackPtr->type = BOX_BLUR;
                optionStackPtr->radius = strtol(optarg, &endPtr, 10);
                if (*endPtr != '\0') {
                    fprintf(stderr, "Box blur: Radius must be a positive integer\n");
                    return 1;
                }
                optionStackPtr++;
                break;
            case 'g':
                optionStackPtr->type = GRAYSCALE;
                optionStackPtr++;
                break;
            case 's':
                optionStackPtr->type = SOBEL;
                optionStackPtr++;
                break;
            case 'f':
                strcpy(inputPath, optarg);
                break;
            case 'o':
                strcpy(outputPath, optarg);
                break;
            case 'i':
                if (argc != 4) {
                    fprintf(stderr, "Usage --info: %s -f <input> -i\n", argv[0]);
                    return 1;
                }
                isCheckInfo = true;
                break;
            case 'h':
                if (argc != 2) {
                    fprintf(stderr, "Usage --help: -h/--help cannot be used with any other arguments");
                    return 1;
                }
                printHelp();
                return 0;
            case 'v':
                if (argc != 2) {
                    fprintf(stderr, "Usage --version: -v/--version cannot be used with any other arguments");
                    return 1;
                }
                printVersion();
                return 0;
            case '?':
            default:
                printf("Usage: %s -f <input> -o <output> [options]\n", argv[0]);
                break;
        }
    }

    BMPImage image = fileToImage(inputPath);

    if (isCheckInfo) {
        printInfo(&image, inputPath);
        freePixels(&image);
        return 0;
    }

    if (inputPath[0] == '\0' || outputPath[0] == '\0') {
        fprintf(stderr, "Usage: %s -f <input> -o <output> [options]\n", argv[0]);
        return 1;
    }

    for (Operation* opt = optionStack; opt < optionStackPtr; opt++) {
        printOperation(opt);
        applyOperation(opt->type)(&image, opt);
    }

    ImageToFile(outputPath, &image);
    freePixels(&image);
    return 0;
}
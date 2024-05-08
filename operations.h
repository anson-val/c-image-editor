//
// Created by Anson on 6/5/2024.
//

#ifndef C_IMAGE_EDITOR_OPERATIONS_H
#define C_IMAGE_EDITOR_OPERATIONS_H

#include "bmp.h"

typedef enum {
    CROP,
    REFLECT_HOR,
    REFLECT_VERT,
    ROTATE,
    BOX_BLUR,
    GRAYSCALE,
    SOBEL
} OperationType;

typedef struct {
    OperationType type;
    int xStart;
    int yStart;
    int width;
    int height;
    int angle;
    int radius;
} Operation;

int (*applyOperation(OperationType type))(BMPImage* image, Operation* operation);
void printOperation(Operation* operation);
#endif //C_IMAGE_EDITOR_OPERATIONS_H

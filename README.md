# c-image-editor

C Image Editor is a simple C program that allows you to edit uncompressed 24-bit/32-bit BMP images. It is designed for CUHK ESTR2306 project. It provides a range of operations including:
- cropping, 
- rotating, 
- reflecting, 
- applying a box-blur, 
- converting to grayscale, 
- and applying the Sobel operator for edge detection.

## Installation

This project uses cmake for building. Follow the steps below to compile and run the program:

1. Clone the repository:
```bash
git clone https://github.com/anson-val/c-image-editor.git
```

2. Change directory to the project folder:
```bash
cd c-image-editor
```

3. Create a build directory:
```bash
mkdir build
cd build
```
4. Run cmake:
```bash
cmake ..
```

5. Compile the project:
```bash
make
```

6. Run the program:
```bash
./c-image-editor
```

## Usage

To run the program, navigate to the build directory and execute the following command:

```bash
./c_image_editor -f <input> -o <output> [options]
```

### Crop

```bash
./c_image_editor -f <input> -o <output> -c <range>
```

#### Range format

###### By interval: 

```bash
<start-column>-<end-column>,<start-row>-<end-row>
```

For example, to crop the image bounded by the pixels (50, 100), (100,100), (50, 150), and (100, 150), aka from column 50-100 and row 100-150 , use the following command:

```bash
./c_image_editor -f <input> -o <output> -c 50-100,100-150
```

###### By resolution:

```bash
<start-column>+<width>,<start-row>+<height>
```

For example, to crop the image starting from pixel (50, 100) with a width of 50 pixels and a height of 50 pixels, use the following command:

```bash
./c_image_editor -f <input> -o <output> -c 50+50,100+50
```

You can also mix the two formats:

```bash
./c_image_editor -f <input> -o <output> -c 50-100,100+50
```

### Rotate

```bash
./c_image_editor -f <input> -o <output> -r <angle>
```

- `<angle>`: The angle of rotation in degrees. The angle must be a multiple of `90`. Positive values rotate the image anti-clockwise, while negative values rotate the image clockwise.

For example, to rotate the image 90 degrees clockwise, use the following command:

```bash
./c_image_editor -f <input> -o <output> -r -90
```

### Reflect Horizontally (along the y-axis)

```bash
./c_image_editor -f <input> -o <output> -x
```

### Reflect Vertically (along the x-axis)

```bash
./c_image_editor -f <input> -o <output> -y
```

### Box-Blur

```bash
./c_image_editor -f <input> -o <output> -b <radius>
```

- `<radius>`: The radius of the box blur. The radius must be a positive integer.
- The box blur is applied to each pixel in the image.

For example, to apply a box blur with a radius of `3`, use the following command:

```bash
./c_image_editor -f <input> -o <output> -b 3
```

### Grayscale

```bash
./c_image_editor -f <input> -o <output> -g
```

- The grayscale operation converts the image to grayscale.

### Sobel Operator for edge detection

```bash
./c_image_editor -f <input> -o <output> -s
```

- The Sobel operator is applied to the image, resulting in an output image where only the edges are visible.

### Properties of an image

```html
./c_image_editor -f <input> -i
```

- The properties of the image are displayed, which contains the header info of the input BMP file.
- -i cannot be used with other options, including `-o`.

### Mixed Operations

**Note: Operations will be executed in the order of options passed.**

You can combine multiple operations in a single command. For example: 
- First, crop the image bounded by the pixels (50, 100), (100,100), (50, 150), and (100, 150),
- then, rotate it 270 degrees anti-clockwise,
- afterward, apply a box blur with a radius of 3, 
- lastly, apply the Sobel operator for edge detection,

```bash
./c_image_editor -f <input> -o <output> -c 50-100,100-150 -r 270 -b 3 -s
```

You can also group options without arguments together. For example:

```bash
./c_image_editor -f <input> -o <output> -gsxy
```

### Help

The help message contains all the commands, use the following command:

```bash
./c_image_editor -h
```

## License

This project is licensed under the terms of the MIT license.

```text
MIT License

Copyright (c) 2024 Anson Ng

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
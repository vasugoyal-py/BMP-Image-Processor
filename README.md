# BMP Image Processor

I wrote this C program completely from scratch to process and apply filters to BMP images. I wanted to understand exactly how image files work under the hood, so I built my own custom structs to parse the binary BMP headers, extract the raw pixel data, process it in memory, and rebuild a new image file byte by byte. 

To make the tool actually usable for everyday people instead of just a command-line script, I hooked it up to the Windows API (windows.h). Instead of typing out long file paths in the terminal, running the program opens the native Windows file picker. You just click the image you want to edit, choose where to save the output, and select your filter.

### What it does

The program handles the raw binary data, modifies the pixels using different mathematical algorithms, and writes the new image. Right now it supports:
* Grayscale conversion
* Horizontal and vertical reflection
* Gaussian blur (with customizable strength)
* Sobel edge detection (calculates pixel gradients to highlight borders of objects in the picture)

### How to run it

If you just want to test the program, you don't need to look at the code. Go to the Releases section on the right side of this GitHub page and download the zip file. Extract it and double click the executable. It will pop up a window asking for your image, ask where to save the new one, and then ask which filter you want to apply in the terminal.

### Notes for compiling

If you want to compile this yourself, you will need a C compiler for Windows like MSYS2 or MinGW. 

Because I integrated the Windows GUI popups, compiling is a little different than a standard terminal app. You have to link the comdlg32 library, otherwise the file picker won't work.

Here is the exact command I use to build it:

```bash
gcc main.c filters.c image.c -o bmp_processor.exe -lm -lcomdlg32 -O2 -static

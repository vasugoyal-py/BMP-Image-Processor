//=====================================================================================================================================
// This file is filters.h

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include "image.h"
void grayscale(int height, int width, RGB **pixels);
int my_round(float x);
void reflect_vertically(int height, int width, RGB **pixels, RGB **copy_pixels);
void reflect_horisontally(int height, int width, RGB **pixels, RGB **copy_pixels);
void blur (int strength, int height, int width, RGB **pixels, RGB **copy_pixels);
void edge_sobel( int height, int width, RGB **pixels, RGB **copy_pixels);
void invert(int height, int width, RGB **pixels, RGB **copy_pixels);
void sepia(int height, int width, RGB **pixels, RGB **copy_pixels);
void brightness_adjustment(int amount, int height, int width, RGB **pixels, RGB **copy_pixels);
void binarization(int height, int width, RGB **pixels, RGB **copy_pixels);

#endif

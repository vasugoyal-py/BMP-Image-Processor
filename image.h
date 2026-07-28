//=====================================================================================================================================
// This file is image.h

#include <stdint.h>
#ifndef IMAGE_H
#define IMAGE_H

typedef struct
{
    uint8_t signature[2];
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t start;
} __attribute__((__packed__)) BMPHeader;

typedef struct 
{
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
    
} __attribute__((__packed__)) DIB;


typedef struct
{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;   
} __attribute__((__packed__)) RGB;



void bmp_head_Read(void);
void print_architecture_status(void);

#endif
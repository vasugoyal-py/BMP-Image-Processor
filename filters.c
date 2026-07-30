//=====================================================================================================================================
// This file is filters.c

#include "filters.h"
#include <math.h>

int my_round(float x)
{
    return (x >= 0) ? (int)(x + 0.5f) : (int)(x - 0.5f);
}

void grayscale(int height, int width, RGB **pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float pre_avg = (float)(pixels[i][j].Blue + pixels[i][j].Green + pixels[i][j].Red) / 3.0;
            int int_avg = my_round(pre_avg);
            pixels[i][j].Blue = int_avg;
            pixels[i][j].Green = int_avg;
            pixels[i][j].Red = int_avg;
        }
    }
}

void reflect_vertically(int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_pixels[i][j] = pixels[i][width - j - 1];
        }
    }
}

void reflect_horisontally(int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_pixels[i][j] = pixels[height - i - 1][j];
        }
    }
}

void blur (int strength, int height, int width, RGB **pixels, RGB **copy_pixels)
{
    if (strength <= 0)
    {
        strength = 0;
    }
    if (strength >= 1000)
    {
        strength = 1000;
    }

    int radius_of_square = 1 + strength/10;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = 0, green = 0, blue = 0, count = 0;

            for (int row_offset = -radius_of_square; row_offset <= radius_of_square; row_offset++)
            {
                for (int col_offset = -radius_of_square; col_offset <= radius_of_square; col_offset++)
                {
                    int row = i + row_offset;
                    int col = j + col_offset;

                    if (row >= 0 && row < height &&
                        col >= 0 && col < width)
                    {
                        red += pixels[row][col].Red;
                        green += pixels[row][col].Green;
                        blue += pixels[row][col].Blue;
                        count++;
                    }
                }
            }
            if (count == 0)
            {
                continue;
            }
            copy_pixels[i][j].Red = my_round((float) red/count);
            copy_pixels[i][j].Green = my_round((float) green/count);
            copy_pixels[i][j].Blue = my_round((float) blue/count);
        }
    }
}

void edge_sobel( int height, int width, RGB **pixels, RGB **copy_pixels)
{

    int Gx[3][3] = 
    {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1},
    };

    int Gy[3][3] = 
    {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red_gx = 0, green_gx = 0, blue_gx = 0;
            int red_gy = 0, green_gy = 0, blue_gy = 0;
            for (int row_ofset = 0; row_ofset <= 2; row_ofset++)
            {
                for (int col_ofset = 0; col_ofset <= 2; col_ofset++)
                {
                    int row = i + row_ofset - 1;
                    int col = j + col_ofset - 1;

                    if (row >= 0 && row < height && col >= 0 && col < width)
                    {
                        blue_gx += pixels[row][col].Blue * Gx[row_ofset][col_ofset];
                        red_gx += pixels[row][col].Red * Gx[row_ofset][col_ofset];
                        green_gy += pixels[row][col].Green * Gy[row_ofset][col_ofset];

                        blue_gy += pixels[row][col].Blue * Gy[row_ofset][col_ofset];
                        red_gy += pixels[row][col].Red * Gy[row_ofset][col_ofset];
                        green_gx += pixels[row][col].Green * Gx[row_ofset][col_ofset];
                    }
                }
            }

            int red = my_round(sqrtf((float)(red_gx * red_gx + red_gy * red_gy)));
            int green = my_round(sqrtf((float)(green_gx * green_gx + green_gy * green_gy)));
            int blue = my_round(sqrtf((float)(blue_gx * blue_gx + blue_gy * blue_gy)));

            if (red > 255) red = 255;
            if (green > 255) green = 255;
            if (blue > 255) blue = 255;

            copy_pixels[i][j].Red = red;
            copy_pixels[i][j].Green = green;
            copy_pixels[i][j].Blue = blue;
        }
    }

}

void invert(int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_pixels[i][j].Red = 255 - pixels[i][j].Red;
            copy_pixels[i][j].Green = 255 - pixels[i][j].Green;
            copy_pixels[i][j].Blue = 255 - pixels[i][j].Blue;
        }
    }
}

void sepia(int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_red = pixels[i][j].Red;
            int original_green = pixels[i][j].Green;
            int original_blue = pixels[i][j].Blue;

            int sepia_red = my_round(0.393f * original_red + 0.769f * original_green + 0.189f * original_blue);
            int sepia_green = my_round(0.349f * original_red + 0.686f * original_green + 0.168f * original_blue);
            int sepia_blue = my_round(0.272f * original_red + 0.534f * original_green + 0.131f * original_blue);

            if (sepia_red > 255) sepia_red = 255;
            if (sepia_green > 255) sepia_green = 255;
            if (sepia_blue > 255) sepia_blue = 255;

            copy_pixels[i][j].Red = sepia_red;
            copy_pixels[i][j].Green = sepia_green;
            copy_pixels[i][j].Blue = sepia_blue;
        }
    }
}

void brightness_adjustment(int amount, int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = pixels[i][j].Red + amount;
            int green = pixels[i][j].Green + amount;
            int blue = pixels[i][j].Blue + amount;

            if (red > 255) red = 255;
            if (green > 255) green = 255;
            if (blue > 255) blue = 255;

            if (red < 0) red = 0;
            if (green < 0) green = 0;
            if (blue < 0) blue = 0;

            copy_pixels[i][j].Red = red;
            copy_pixels[i][j].Green = green;
            copy_pixels[i][j].Blue = blue;
        }
    }
}

void binarization(int height, int width, RGB **pixels, RGB **copy_pixels)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (float)(pixels[i][j].Red + pixels[i][j].Green + pixels[i][j].Blue) / 3.0f;
            int value = (avg >= 128.0f) ? 255 : 0;

            copy_pixels[i][j].Red = value;
            copy_pixels[i][j].Green = value;
            copy_pixels[i][j].Blue = value;
        }
    }
}

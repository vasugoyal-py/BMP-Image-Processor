#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "image.h"
#include "filters.h"

int get_open_filename(char* buffer, int buffer_size) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = buffer;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = buffer_size;
    ofn.lpstrFilter = "BMP Images\0*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    return GetOpenFileName(&ofn);
}

int get_save_filename(char* buffer, int buffer_size) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = buffer;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = buffer_size;
    ofn.lpstrFilter = "BMP Images\0*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = "bmp";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    return GetSaveFileName(&ofn);
}

int main(int argc, char* argv[])
{
    char input_file[MAX_PATH];
    char output_file[MAX_PATH];


    if (argc == 3)
    {
        strcpy(input_file, argv[1]);
        strcpy(output_file, argv[2]);
    }
    else
    {
        printf("Opening file selector to choose input BMP...\n");
        if (!get_open_filename(input_file, sizeof(input_file))) {
            printf("No input file selected. Exiting program.\n");
            return 1;
        }
        printf("Selected Input: %s\n", input_file);

        printf("Opening file selector to choose output location...\n");
        if (!get_save_filename(output_file, sizeof(output_file))) {
            printf("No output location selected. Exiting program.\n");
            return 1;
        }
        printf("Selected Output: %s\n", output_file);
    }

    FILE *input = fopen(input_file, "rb");
    if (input == NULL)
    {
        printf("Input File couldn't be opened try again (error code: 2)\n");
        return 2;
    }

    FILE *output = fopen(output_file, "wb");
    if (output == NULL)
    {   
        printf("Output File couldn't be opened try again (error code: 3)\n");
        return 3;
    }

    BMPHeader header;
    if (fread(&header, sizeof(BMPHeader), 1, input) != 1)
    {
        printf("Could not read BMP header in input (error code: 4)\n");
        fclose(input);
        fclose(output);
        return 4;
    }

    if (header.signature[0] != 'B' || header.signature[1] != 'M')
    {
        printf("Not a BMP file (error code: 5)\n");
        fclose(input);
        fclose(output);
        return 5;
    }

    printf("Signature: %c%c\n", header.signature[0], header.signature[1]);
    printf("File Size: %u bytes\n", header.file_size);

    if (fwrite(&header, sizeof(BMPHeader), 1, output) != 1)
    {
        printf("Could not write BMP header in output (error code: 6)\n");
        fclose(input);
        fclose(output);
        return 6;
    }

    DIB head_2;

    if (fread(&head_2, sizeof(DIB), 1, input) != 1)
    {
        printf("Could not read BMP DIB in input (error code: 7)\n");
        fclose(input);
        fclose(output);
        return 7;
    }
    
    if (fwrite(&head_2, sizeof(DIB), 1, output) != 1) 
    {
        printf("Could not write BMP DIB in output (error code: 8)\n");
        fclose(input);
        fclose(output);
        return 8;
    }

    int skip = head_2.size - sizeof(DIB);

    for (int i = 0; i < skip; i++)
    {
        int temp = fgetc(input);
        fputc(temp, output);
    }

    int padding = (4 - ((head_2.width * sizeof(RGB)) % 4)) % 4;

    RGB **pixels = malloc(head_2.height * sizeof(RGB *));

    for (int i = 0; i < head_2.height; i++)
    {
        pixels[i] = malloc((head_2.width * sizeof(RGB)));
    }

    RGB **copy_pixels = malloc(head_2.height * (sizeof(RGB *)));

    for (int i = 0; i < head_2.height; i++)
    {
        copy_pixels[i] = malloc(head_2.width * sizeof(RGB));
    }

    for (int i = 0; i < head_2.height; i++)
    {
        if (fread(pixels[i], sizeof(RGB) * head_2.width , 1, input) != 1)
        {
            printf("Could not read pixels in input (error code: 9)\n");
            fclose(input);
            fclose(output);
            return 9;
        }
        fseek(input, padding, SEEK_CUR);
    }

    for (int i = 0; i < head_2.height; i++)
    {
        for (int j = 0; j < head_2.width; j++)
        {
            copy_pixels[i][j] = pixels[i][j];
        }
    }

    int user_choice = -1;
    printf("\nHow do you want to convert:- \n0.) Unchanged\n1.) Greyscale\n2.) Flip vertically (x,y) --> (-x,y)\n3.) Flip Horizontally (x,y) --> (x,-y)\n4.) Gaussian Blur\n5.) Edges (sobel)\n6.) invert(negative)\n7.) Sepia\n8.) Brightness adjustment\n9.) Binarization Choice: ");
    scanf("%d", &user_choice);
    
    if (user_choice == -1)
    {
        printf("No choice was made defaulting to Unchanged\n");
    }
    else if (user_choice == 0){}
    else if (user_choice == 1)
    {
        grayscale(head_2.height, head_2.width, copy_pixels);
    }
    else if (user_choice ==  2)
    {
        reflect_vertically(head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 3)
    {
        reflect_horisontally(head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 4)
    {   
        int strength;
        printf("What strength do you want of the blur: 1-1000\n");
        scanf("%d", &strength);
        blur(strength, head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 5)
    {
        edge_sobel( head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 6)
    {
        invert( head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 7)
    {
        sepia(head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 8)
    {
        int amount;
        printf("What brightness adjustment do you want (-255 to 255): ");
        scanf("%d", &amount);
        brightness_adjustment(amount, head_2.height, head_2.width, pixels, copy_pixels);
    }
    else if (user_choice == 9)
    {
        binarization(head_2.height, head_2.width, pixels, copy_pixels);
    }
    else
    {
        printf("Please choose from given options (error code 10)\n");
        fclose(input);
        fclose(output);
        return 10;
    }
    
    printf("Choice was %d.) changes made to output accordingly\n", user_choice);

    for(int i = 0; i < head_2.height; i++)
    {
        if (fwrite(copy_pixels[i], sizeof(RGB) * head_2.width, 1, output) != 1)
        {
            printf("Could not write pixels in output (error code: 11)\n");
            fclose(input);
            fclose(output);
            return 11;
        }
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, output);
        }
    }
    
    fclose(input);
    fclose(output);
    for (int i = 0; i < head_2.height; i++)
    {
        free(pixels[i]);
    }
    free(pixels);

    for (int i = 0; i < head_2.height; i++)
    {
        free(copy_pixels[i]);
    }
    free(copy_pixels);
    
    printf("\nSuccess! Image processing complete.\n");
    return 0;
}

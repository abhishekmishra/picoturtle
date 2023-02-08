#include "GifUtil.h"
#include <wand/magick_wand.h>
#include <string.h>
#include <stdio.h>

void init_gif_util()
{
    InitializeMagick(NULL);
}

void exit_gif_util()
{
    DestroyMagick();
}

int make_gif_from_images(char *img_filenames[], int num_images, 
    char *output_filename)
{
    if(img_filenames == NULL)
    {
        printf("Error:: Image list for gif is empty. Gif creation aborted!");
        return -1;
    }

    if (num_images == 0)
    {
        printf("Error:: No images in image list for gif. \
            Gif creation aborted!");
        return -1;
    }

    if (output_filename == NULL || strlen(output_filename) == 0)
    {
        printf("Error:: Gif filename is null. \
            Gif creation aborted!");
        return -1;
    }

    MagickWand * wand;
    wand = NewMagickWand();

    for (int i = 0; i < num_images; i++)
    {
        MagickReadImage(wand, img_filenames[i]);
    }

    MagickWriteImages(wand, output_filename, MagickTrue);

    return 0;
}
#include "GifUtil.h"
#include <string.h>
#include <stdio.h>

/* 
    IMP: magick_wand.h refers to ssize_t which is not available on windows 
    The next few lines make sure it is defined on windows build.
    also see https://www.scivision.dev/ssize_t-visual-studio-posix/
*/
#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <wand/magick_wand.h>

void init_gif_util()
{
    InitializeMagick(NULL);
}

void exit_gif_util()
{
    DestroyMagick();
}

int make_gif_from_images(char *img_filenames[], int num_images, 
    const char *output_filename)
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

    // printf("Writing file %s\n", output_filename);
    MagickWriteImages(wand, output_filename, MagickTrue);

    return 0;
}
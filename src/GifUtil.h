#ifndef __PICOTURTLE_GIF_UTIL__
#define __PICOTURTLE_GIF_UTIL__

void init_gif_util();

void exit_gif_util();

int make_gif_from_images(char *img_filenames[], int num_images, 
    const char *output_filename);

#endif //__PICOTURTLE_GIF_UTIL__
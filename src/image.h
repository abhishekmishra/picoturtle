#ifndef ___PICOTURTLE_IMAGE_H___
#define ___PICOTURTLE_IMAGE_H___

#include "runtime.h"

typedef struct picoturtle_image {
    Texture2D texture;
    char *path;
    picoturtle_runtime_t *runtime;
} picoturtle_image_t;

picoturtle_image_t *picoturtle_image_load(picoturtle_runtime_t *runtime, const char *path);
void picoturtle_image_destroy(picoturtle_image_t *image);
void picoturtle_image_draw(const picoturtle_image_t *image, float x, float y);

#endif // ___PICOTURTLE_IMAGE_H___

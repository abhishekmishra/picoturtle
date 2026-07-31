#ifndef ___PTRTL_IMAGE_H___
#define ___PTRTL_IMAGE_H___

#include "runtime.h"

typedef struct ptrl_image {
    Texture2D texture;
    char *path;
    ptrl_runtime_t *runtime;
} ptrl_image_t;

ptrl_image_t *ptrl_image_load(ptrl_runtime_t *runtime, const char *path);
void ptrl_image_destroy(ptrl_image_t *image);
void ptrl_image_draw(const ptrl_image_t *image, float x, float y);

#endif // ___PTRTL_IMAGE_H___

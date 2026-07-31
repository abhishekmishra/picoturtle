#include "image.h"

#include <stdlib.h>
#include <string.h>

ptrl_image_t *ptrl_image_load(ptrl_runtime_t *runtime, const char *path) {
    if (runtime == NULL || !runtime->initialized ||
        path == NULL || path[0] == '\0') {
        return NULL;
    }

    ptrl_image_t *image = calloc(1, sizeof(*image));
    if (image == NULL) {
        return NULL;
    }

    image->path = malloc(strlen(path) + 1);
    if (image->path == NULL) {
        free(image);
        return NULL;
    }
    strcpy(image->path, path);

    image->texture = LoadTexture(path);
    if (image->texture.id == 0) {
        free(image->path);
        free(image);
        return NULL;
    }

    image->runtime = runtime;
    return image;
}

void ptrl_image_destroy(ptrl_image_t *image) {
    if (image == NULL) {
        return;
    }
    if (image->texture.id != 0 &&
        image->runtime != NULL && image->runtime->initialized) {
        UnloadTexture(image->texture);
    }
    free(image->path);
    free(image);
}

void ptrl_image_draw(const ptrl_image_t *image, float x, float y) {
    if (image == NULL || image->runtime == NULL ||
        !image->runtime->initialized || image->texture.id == 0) {
        return;
    }

    ptrl_runtime_begin_canvas(image->runtime);
    DrawTextureV(image->texture, (Vector2){x, y}, WHITE);
    ptrl_runtime_end_canvas(image->runtime);
}

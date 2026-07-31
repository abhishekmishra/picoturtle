#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *resolve_image_path(
    const char *path,
    char *resolved,
    size_t resolved_size
) {
    if (FileExists(path) || strncmp(path, "res/", 4) != 0) {
        return path;
    }

    const char *relative = path + 4;
    const char *resource_root = getenv("PICOTURTLE_RESOURCE_DIR");
    const char *application_dir = GetApplicationDirectory();
    const char *patterns[] = {
        "%s/%s",
        "%s../share/picoturtle/res/%s",
        "%s../res/%s",
        "%sres/%s"
    };

    for (size_t index = 0; index < sizeof(patterns) / sizeof(patterns[0]);
         index++) {
        int written;
        if (index == 0) {
            if (resource_root == NULL || resource_root[0] == '\0') {
                continue;
            }
            written = snprintf(
                resolved, resolved_size, patterns[index],
                resource_root, relative
            );
        } else {
            written = snprintf(
                resolved, resolved_size, patterns[index],
                application_dir, relative
            );
        }
        if (written > 0 && (size_t)written < resolved_size &&
            FileExists(resolved)) {
            return resolved;
        }
    }
    return path;
}

picoturtle_image_t *picoturtle_image_load(picoturtle_runtime_t *runtime, const char *path) {
    if (runtime == NULL || !runtime->initialized ||
        path == NULL || path[0] == '\0') {
        return NULL;
    }

    picoturtle_image_t *image = calloc(1, sizeof(*image));
    if (image == NULL) {
        return NULL;
    }

    image->path = malloc(strlen(path) + 1);
    if (image->path == NULL) {
        free(image);
        return NULL;
    }
    strcpy(image->path, path);

    char resolved[1024];
    image->texture = LoadTexture(
        resolve_image_path(path, resolved, sizeof(resolved))
    );
    if (image->texture.id == 0) {
        free(image->path);
        free(image);
        return NULL;
    }

    image->runtime = runtime;
    return image;
}

void picoturtle_image_destroy(picoturtle_image_t *image) {
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

void picoturtle_image_draw(const picoturtle_image_t *image, float x, float y) {
    if (image == NULL || image->runtime == NULL ||
        !image->runtime->initialized || image->texture.id == 0) {
        return;
    }

    picoturtle_runtime_begin_canvas(image->runtime);
    DrawTextureV(image->texture, (Vector2){x, y}, WHITE);
    picoturtle_runtime_end_canvas(image->runtime);
}

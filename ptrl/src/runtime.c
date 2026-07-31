#include "runtime.h"

#include <stddef.h>

static ptrl_runtime_t *default_runtime = NULL;

bool ptrl_runtime_init(
    ptrl_runtime_t *runtime,
    int width,
    int height,
    const char *title
) {
    if (runtime == NULL || width <= 0 || height <= 0) {
        return false;
    }

    runtime->canvas = (RenderTexture2D){0};
    runtime->canvas_width = width;
    runtime->canvas_height = height;
    runtime->background = RAYWHITE;
    runtime->initialized = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title);
    if (!IsWindowReady()) {
        return false;
    }

    SetTargetFPS(60);
    runtime->canvas = LoadRenderTexture(width, height);
    if (runtime->canvas.id == 0) {
        CloseWindow();
        return false;
    }

    runtime->initialized = true;
    ptrl_runtime_clear(runtime, runtime->background);
    ptrl_runtime_set_default(runtime);
    return true;
}

void ptrl_runtime_destroy(ptrl_runtime_t *runtime) {
    if (runtime == NULL || !runtime->initialized) {
        return;
    }

    if (default_runtime == runtime) {
        default_runtime = NULL;
    }

    UnloadRenderTexture(runtime->canvas);
    CloseWindow();
    runtime->canvas = (RenderTexture2D){0};
    runtime->initialized = false;
}

void ptrl_runtime_set_default(ptrl_runtime_t *runtime) {
    default_runtime = runtime;
}

ptrl_runtime_t *ptrl_runtime_get_default(void) {
    return default_runtime;
}

void ptrl_runtime_begin_canvas(ptrl_runtime_t *runtime) {
    if (runtime != NULL && runtime->initialized) {
        BeginTextureMode(runtime->canvas);
    }
}

void ptrl_runtime_end_canvas(ptrl_runtime_t *runtime) {
    if (runtime != NULL && runtime->initialized) {
        EndTextureMode();
    }
}

void ptrl_runtime_clear(ptrl_runtime_t *runtime, Color color) {
    if (runtime == NULL || !runtime->initialized) {
        return;
    }

    runtime->background = color;
    ptrl_runtime_begin_canvas(runtime);
    ClearBackground(color);
    ptrl_runtime_end_canvas(runtime);
}

bool ptrl_runtime_resize(ptrl_runtime_t *runtime, int width, int height) {
    if (runtime == NULL || width <= 0 || height <= 0) {
        return false;
    }
    if (!runtime->initialized) {
        runtime->canvas_width = width;
        runtime->canvas_height = height;
        return true;
    }
    if (width == runtime->canvas_width && height == runtime->canvas_height) {
        return true;
    }

    RenderTexture2D new_canvas = LoadRenderTexture(width, height);
    if (new_canvas.id == 0) {
        return false;
    }

    UnloadRenderTexture(runtime->canvas);
    runtime->canvas = new_canvas;
    runtime->canvas_width = width;
    runtime->canvas_height = height;
    SetWindowSize(width, height);
    ptrl_runtime_clear(runtime, runtime->background);
    return true;
}

bool ptrl_runtime_export_png(ptrl_runtime_t *runtime, const char *filename) {
    if (runtime == NULL || !runtime->initialized ||
        filename == NULL || filename[0] == '\0') {
        return false;
    }

    Image image = LoadImageFromTexture(runtime->canvas.texture);
    if (image.data == NULL) {
        return false;
    }

    ImageFlipVertical(&image);
    bool exported = ExportImage(image, filename);
    UnloadImage(image);
    return exported;
}

void ptrl_runtime_present(ptrl_runtime_t *runtime) {
    if (runtime == NULL || !runtime->initialized) {
        return;
    }

    BeginDrawing();
    ClearBackground(runtime->background);

    Rectangle source = {
        0.0f,
        0.0f,
        (float)runtime->canvas.texture.width,
        (float)-runtime->canvas.texture.height
    };
    Rectangle destination = {
        0.0f,
        0.0f,
        (float)GetScreenWidth(),
        (float)GetScreenHeight()
    };
    DrawTexturePro(
        runtime->canvas.texture,
        source,
        destination,
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE
    );
    EndDrawing();
}

bool ptrl_runtime_should_close(const ptrl_runtime_t *runtime) {
    return runtime == NULL || !runtime->initialized || WindowShouldClose();
}

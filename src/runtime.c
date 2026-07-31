#include "runtime.h"
#include "font.h"

#include <stddef.h>

static picoturtle_runtime_t *default_runtime = NULL;

bool picoturtle_runtime_init(
    picoturtle_runtime_t *runtime,
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
    runtime->update_enabled = true;
    runtime->close_requested = false;
    runtime->font_cache = NULL;

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
    picoturtle_runtime_clear(runtime, runtime->background);
    picoturtle_runtime_set_default(runtime);
    return true;
}

void picoturtle_runtime_destroy(picoturtle_runtime_t *runtime) {
    if (runtime == NULL || !runtime->initialized) {
        return;
    }

    if (default_runtime == runtime) {
        default_runtime = NULL;
    }

    picoturtle_font_cache_destroy(&runtime->font_cache);
    UnloadRenderTexture(runtime->canvas);
    CloseWindow();
    runtime->canvas = (RenderTexture2D){0};
    runtime->initialized = false;
}

void picoturtle_runtime_set_default(picoturtle_runtime_t *runtime) {
    default_runtime = runtime;
}

picoturtle_runtime_t *picoturtle_runtime_get_default(void) {
    return default_runtime;
}

void picoturtle_runtime_begin_canvas(picoturtle_runtime_t *runtime) {
    if (runtime != NULL && runtime->initialized) {
        BeginTextureMode(runtime->canvas);
    }
}

void picoturtle_runtime_end_canvas(picoturtle_runtime_t *runtime) {
    if (runtime != NULL && runtime->initialized) {
        EndTextureMode();
        if (runtime->update_enabled) {
            picoturtle_runtime_present(runtime);
        }
    }
}

void picoturtle_runtime_clear(picoturtle_runtime_t *runtime, Color color) {
    if (runtime == NULL || !runtime->initialized) {
        return;
    }

    runtime->background = color;
    picoturtle_runtime_begin_canvas(runtime);
    ClearBackground(color);
    picoturtle_runtime_end_canvas(runtime);
}

bool picoturtle_runtime_resize(picoturtle_runtime_t *runtime, int width, int height) {
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
    picoturtle_runtime_clear(runtime, runtime->background);
    return true;
}

bool picoturtle_runtime_export_png(picoturtle_runtime_t *runtime, const char *filename) {
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

void picoturtle_runtime_set_update_enabled(picoturtle_runtime_t *runtime, bool enabled) {
    if (runtime == NULL) {
        return;
    }
    runtime->update_enabled = enabled;
    if (enabled && runtime->initialized) {
        picoturtle_runtime_present(runtime);
    }
}

void picoturtle_runtime_paint(picoturtle_runtime_t *runtime) {
    picoturtle_runtime_present(runtime);
}

void picoturtle_runtime_delay(picoturtle_runtime_t *runtime, int milliseconds) {
    if (runtime == NULL || !runtime->initialized || milliseconds <= 0) {
        return;
    }

    double deadline = GetTime() + ((double)milliseconds / 1000.0);
    while (GetTime() < deadline && !picoturtle_runtime_should_close(runtime)) {
        picoturtle_runtime_present(runtime);
    }
}

void picoturtle_runtime_present(picoturtle_runtime_t *runtime) {
    if (runtime == NULL || !runtime->initialized || runtime->close_requested) {
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
    if (WindowShouldClose()) {
        runtime->close_requested = true;
    }
}

bool picoturtle_runtime_should_close(picoturtle_runtime_t *runtime) {
    if (runtime == NULL || !runtime->initialized || runtime->close_requested) {
        return true;
    }
    if (WindowShouldClose()) {
        runtime->close_requested = true;
        return true;
    }
    return false;
}

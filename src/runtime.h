#ifndef ___PICOTURTLE_RUNTIME_H___
#define ___PICOTURTLE_RUNTIME_H___

#include <stdbool.h>
#include <raylib.h>

typedef struct picoturtle_font_cache picoturtle_font_cache_t;

#define PICOTURTLE_DEFAULT_CANVAS_WIDTH 1024
#define PICOTURTLE_DEFAULT_CANVAS_HEIGHT 1024

typedef struct picoturtle_runtime {
    RenderTexture2D canvas;
    int canvas_width;
    int canvas_height;
    Color background;
    bool initialized;
    bool update_enabled;
    bool close_requested;
    picoturtle_font_cache_t *font_cache;
} picoturtle_runtime_t;

bool picoturtle_runtime_init(
    picoturtle_runtime_t *runtime,
    int width,
    int height,
    const char *title
);
void picoturtle_runtime_destroy(picoturtle_runtime_t *runtime);

void picoturtle_runtime_set_default(picoturtle_runtime_t *runtime);
picoturtle_runtime_t *picoturtle_runtime_get_default(void);

void picoturtle_runtime_begin_canvas(picoturtle_runtime_t *runtime);
void picoturtle_runtime_end_canvas(picoturtle_runtime_t *runtime);
void picoturtle_runtime_clear(picoturtle_runtime_t *runtime, Color color);
bool picoturtle_runtime_resize(picoturtle_runtime_t *runtime, int width, int height);
bool picoturtle_runtime_export_png(picoturtle_runtime_t *runtime, const char *filename);
void picoturtle_runtime_set_update_enabled(picoturtle_runtime_t *runtime, bool enabled);
void picoturtle_runtime_paint(picoturtle_runtime_t *runtime);
void picoturtle_runtime_delay(picoturtle_runtime_t *runtime, int milliseconds);
void picoturtle_runtime_present(picoturtle_runtime_t *runtime);
bool picoturtle_runtime_should_close(picoturtle_runtime_t *runtime);

#endif // ___PICOTURTLE_RUNTIME_H___

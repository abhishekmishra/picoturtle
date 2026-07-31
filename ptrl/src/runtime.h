#ifndef ___PTRTL_RUNTIME_H___
#define ___PTRTL_RUNTIME_H___

#include <stdbool.h>
#include <raylib.h>

#define PTRTL_DEFAULT_CANVAS_WIDTH 1024
#define PTRTL_DEFAULT_CANVAS_HEIGHT 1024

typedef struct ptrl_runtime {
    RenderTexture2D canvas;
    int canvas_width;
    int canvas_height;
    Color background;
    bool initialized;
} ptrl_runtime_t;

bool ptrl_runtime_init(
    ptrl_runtime_t *runtime,
    int width,
    int height,
    const char *title
);
void ptrl_runtime_destroy(ptrl_runtime_t *runtime);

void ptrl_runtime_set_default(ptrl_runtime_t *runtime);
ptrl_runtime_t *ptrl_runtime_get_default(void);

void ptrl_runtime_begin_canvas(ptrl_runtime_t *runtime);
void ptrl_runtime_end_canvas(ptrl_runtime_t *runtime);
void ptrl_runtime_clear(ptrl_runtime_t *runtime, Color color);
bool ptrl_runtime_resize(ptrl_runtime_t *runtime, int width, int height);
bool ptrl_runtime_export_png(ptrl_runtime_t *runtime, const char *filename);
void ptrl_runtime_present(ptrl_runtime_t *runtime);
bool ptrl_runtime_should_close(const ptrl_runtime_t *runtime);

#endif // ___PTRTL_RUNTIME_H___

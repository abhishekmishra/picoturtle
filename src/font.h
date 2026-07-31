#ifndef ___PTRTL_FONT_H___
#define ___PTRTL_FONT_H___

#include <raylib.h>

typedef struct ptrl_font_cache ptrl_font_cache_t;

Font ptrl_font_cache_get(
    ptrl_font_cache_t **cache,
    const char *font_name,
    int font_size
);
void ptrl_font_cache_destroy(ptrl_font_cache_t **cache);

#endif

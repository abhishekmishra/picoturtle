#ifndef ___PICOTURTLE_FONT_H___
#define ___PICOTURTLE_FONT_H___

#include <raylib.h>

typedef struct picoturtle_font_cache picoturtle_font_cache_t;

Font picoturtle_font_cache_get(
    picoturtle_font_cache_t **cache,
    const char *font_name,
    int font_size
);
void picoturtle_font_cache_destroy(picoturtle_font_cache_t **cache);

#endif

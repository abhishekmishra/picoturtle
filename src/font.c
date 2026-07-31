#include "font.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PTRTL_LATIN_FIRST 0x20
#define PTRTL_LATIN_LAST 0xff

typedef enum ptrl_font_kind {
    PTRTL_FONT_LATIN,
    PTRTL_FONT_FILE
} ptrl_font_kind_t;

struct ptrl_font_cache {
    char *key;
    Font font;
    struct ptrl_font_cache *next;
};

static ptrl_font_kind_t classify_font(const char *font_name) {
    if (font_name != NULL && FileExists(font_name)) {
        return PTRTL_FONT_FILE;
    }
    return PTRTL_FONT_LATIN;
}

static const char *bundled_font_path(
    ptrl_font_kind_t kind,
    char *resolved,
    size_t resolved_size
) {
    (void)kind;
    const char *relative = "fonts/noto-sans/NotoSans.ttf";
    const char *resource_root = getenv("PTRTL_RESOURCE_DIR");
    const char *application_dir = GetApplicationDirectory();
    const char *patterns[] = {
        "%s/%s",
        "ptrl/res/%s",
        "res/%s",
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
        } else if (index < 3) {
            written = snprintf(
                resolved, resolved_size, patterns[index], relative
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
    return NULL;
}

static int *make_codepoints(ptrl_font_kind_t kind, int *count) {
    (void)kind;
    int latin_count = PTRTL_LATIN_LAST - PTRTL_LATIN_FIRST + 1;
    *count = latin_count;
    int *codepoints = malloc((size_t)*count * sizeof(*codepoints));
    if (codepoints == NULL) {
        *count = 0;
        return NULL;
    }

    int output = 0;
    for (int codepoint = PTRTL_LATIN_FIRST;
         codepoint <= PTRTL_LATIN_LAST; codepoint++) {
        codepoints[output++] = codepoint;
    }
    return codepoints;
}

Font ptrl_font_cache_get(
    ptrl_font_cache_t **cache,
    const char *font_name,
    int font_size
) {
    if (cache == NULL || font_size <= 0) {
        return GetFontDefault();
    }

    ptrl_font_kind_t kind = classify_font(font_name);
    char resolved[1024];
    const char *path = kind == PTRTL_FONT_FILE
        ? font_name
        : bundled_font_path(kind, resolved, sizeof(resolved));
    if (path == NULL) {
        TraceLog(LOG_WARNING, "FONT: bundled PicoTurtle font not found");
        return GetFontDefault();
    }

    char key[1152];
    int written = snprintf(key, sizeof(key), "%s:%d", path, font_size);
    if (written <= 0 || (size_t)written >= sizeof(key)) {
        return GetFontDefault();
    }
    for (ptrl_font_cache_t *entry = *cache; entry != NULL;
         entry = entry->next) {
        if (strcmp(entry->key, key) == 0) {
            return entry->font;
        }
    }

    int codepoint_count = 0;
    int *codepoints = make_codepoints(kind, &codepoint_count);
    if (codepoints == NULL) {
        return GetFontDefault();
    }
    Font font = LoadFontEx(path, font_size, codepoints, codepoint_count);
    free(codepoints);
    if (!IsFontValid(font)) {
        TraceLog(LOG_WARNING, "FONT: failed to load %s", path);
        return GetFontDefault();
    }

    ptrl_font_cache_t *entry = calloc(1, sizeof(*entry));
    if (entry == NULL) {
        UnloadFont(font);
        return GetFontDefault();
    }
    entry->key = malloc(strlen(key) + 1);
    if (entry->key == NULL) {
        free(entry);
        UnloadFont(font);
        return GetFontDefault();
    }
    strcpy(entry->key, key);
    entry->font = font;
    entry->next = *cache;
    *cache = entry;
    return font;
}

void ptrl_font_cache_destroy(ptrl_font_cache_t **cache) {
    if (cache == NULL) {
        return;
    }
    ptrl_font_cache_t *entry = *cache;
    while (entry != NULL) {
        ptrl_font_cache_t *next = entry->next;
        UnloadFont(entry->font);
        free(entry->key);
        free(entry);
        entry = next;
    }
    *cache = NULL;
}

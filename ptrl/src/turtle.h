#ifndef ___TURTLE_H___
#define ___TURTLE_H___

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} location_t;

float location_get_x(const location_t *loc) {
    return loc->x;
}
float location_get_y(const location_t *loc) {
    return loc->y;
}
void location_set_x(location_t *loc, float x) {
    loc->x = x;
}
void location_set_y(location_t *loc, float y) {
    loc->y = y;
}

void make_location(location_t **loc)
{
    *loc = (location_t *)malloc(sizeof(location_t));
    if (*loc != NULL) {
        (*loc)->x = 0.0f;
        (*loc)->y = 0.0f;
    }
}

void make_location(location_t **loc, float x, float y) {
    make_location(loc);
    if (*loc != NULL) {
        (*loc)->x = x;
        (*loc)->y = y;
    }
}

void free_location(location_t *loc) {
    if (loc != NULL) {
        free(loc);
    }
}

void print_location(const location_t *loc) {
    if (loc != NULL) {
        printf("Location: (%.2f, %.2f)\n", loc->x, loc->y);
    } else {
        printf("Location is NULL\n");
    }
}

#endif // ___TURTLE_H___
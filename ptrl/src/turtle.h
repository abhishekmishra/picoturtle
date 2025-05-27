#ifndef ___TURTLE_H___
#define ___TURTLE_H___

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*------------ trtl_location_t --------------*/
// The Location struct for representing the location
// of the turtle in a 2D space.
typedef struct {
    float x;
    float y;
} trtl_location_t;

float location_get_x(const trtl_location_t *loc) {
    return loc->x;
}
float location_get_y(const trtl_location_t *loc) {
    return loc->y;
}
void location_set_x(trtl_location_t *loc, float x) {
    loc->x = x;
}
void location_set_y(trtl_location_t *loc, float y) {
    loc->y = y;
}

void make_location(trtl_location_t **loc)
{
    *loc = (trtl_location_t *)malloc(sizeof(trtl_location_t));
    if (*loc != NULL) {
        (*loc)->x = 0.0f;
        (*loc)->y = 0.0f;
    }
}

void make_location(trtl_location_t **loc, float x, float y) {
    make_location(loc);
    if (*loc != NULL) {
        (*loc)->x = x;
        (*loc)->y = y;
    }
}

void free_location(trtl_location_t *loc) {
    if (loc != NULL) {
        free(loc);
    }
}

void print_location(const trtl_location_t *loc) {
    if (loc != NULL) {
        printf("Location: (%.2f, %.2f)\n", loc->x, loc->y);
    } else {
        printf("Location is NULL\n");
    }
}

/*------------ trtl_colour_t --------------*/

// using uint8_t for RGBA values,
// and char* for the name of the colour
typedef struct {
    uint8_t r; // Red component
    uint8_t g; // Green component
    uint8_t b; // Blue component
    uint8_t a; // Alpha component
    char *name; // Name of the colour
} trtl_colour_t;

void make_colour(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name) {
    *col = (trtl_colour_t *)malloc(sizeof(trtl_colour_t));
    if (*col != NULL) {
        (*col)->r = r;
        (*col)->g = g;
        (*col)->b = b;
        (*col)->a = a;
        (*col)->name = (char *)malloc(strlen(name) + 1);
        if ((*col)->name != NULL) {
            strcpy((*col)->name, name);
        }
    }
}
void free_colour(trtl_colour_t *col) {
    if (col != NULL) {
        if (col->name != NULL) {
            free(col->name);
        }
        free(col);
    }
}

void print_colour(const trtl_colour_t *col) {
    if (col != NULL) {
        printf("Colour: (%d, %d, %d, %d), Name: %s\n", col->r, col->g, col->b, col->a, col->name);
    } else {
        printf("Colour is NULL\n");
    }
}

/*------------ trtl_state_t --------------*/

typedef struct {
    trtl_location_t *location;
    trtl_colour_t *colour;
    double heading;
    int pen_down; // 1 for down, 0 for up
    float pen_width;
} trtl_state_t;

#endif // ___TURTLE_H___
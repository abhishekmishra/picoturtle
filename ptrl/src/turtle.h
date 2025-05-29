#ifndef ___TURTLE_H___
#define ___TURTLE_H___

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <raylib.h>

/*------------ trtl_location_t --------------*/
typedef struct {
    float x;
    float y;
} trtl_location_t;

float location_get_x(const trtl_location_t *loc);
float location_get_y(const trtl_location_t *loc);
void location_set_x(trtl_location_t *loc, float x);
void location_set_y(trtl_location_t *loc, float y);
void make_location(trtl_location_t **loc);
void make_location_xy(trtl_location_t **loc, float x, float y);
void free_location(trtl_location_t *loc);
void print_location(const trtl_location_t *loc);

/*------------ trtl_colour_t --------------*/
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    char *name;
} trtl_colour_t;

void make_colour(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name);
void free_colour(trtl_colour_t *col);
void print_colour(const trtl_colour_t *col);

/*------------ trtl_state_t --------------*/
typedef struct {
    trtl_location_t *location;
    trtl_colour_t *pen_colour;
    double heading;
    int pen_down;
    float pen_width;
} trtl_state_t;

void make_state(trtl_state_t **state);
void free_state(trtl_state_t *state);
trtl_location_t* trtl_state_get_location(const trtl_state_t *state);
trtl_colour_t* trtl_state_get_pen_colour(const trtl_state_t *state);
double trtl_state_get_heading(const trtl_state_t *state);
void trtl_state_set_heading(trtl_state_t *state, double heading);
int trtl_state_is_pen_down(const trtl_state_t *state);
void trtl_state_set_pen_down(trtl_state_t *state, int pen_down);
float trtl_state_get_pen_width(const trtl_state_t *state);
void trtl_state_set_pen_width(trtl_state_t *state, float pen_width);

/*------------ trtl_t --------------*/
typedef struct {
    trtl_state_t *current_state;
    char* name;
    char* id;
    long start_time;
} trtl_t;

void make_turtle(trtl_t **turtle, const char *name, const char *id);
void free_turtle(trtl_t *turtle);
trtl_state_t* trtl_get_state(const trtl_t *turtle);
trtl_location_t* trtl_get_location(const trtl_t *turtle);
trtl_colour_t* trtl_get_pen_colour(const trtl_t *turtle);
double trtl_get_heading(const trtl_t *turtle);
double trtl_get_canvas_heading(const trtl_t *turtle);
float trtl_get_canvas_location_x(const trtl_t *turtle);
float trtl_get_canvas_location_y(const trtl_t *turtle);
float trtl_get_pen_width(const trtl_t *turtle);
void trtl_draw_me(const trtl_t *turtle);

// movement functions
// with pen movement
void trtl_forward(trtl_t *turtle, float distance);
void trtl_backward(trtl_t *turtle, float distance);
// levitate/teleport without drawing
void trtl_set_position(trtl_t *turtle, float x, float y);
void trtl_set_x(trtl_t *turtle, float x);
void trtl_set_y(trtl_t *turtle, float y);
// heading functions
void trtl_set_heading(trtl_t *turtle, double heading);

#endif // ___TURTLE_H___
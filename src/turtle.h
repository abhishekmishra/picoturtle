#ifndef ___TURTLE_H___
#define ___TURTLE_H___

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <raylib.h>
#include <color_names.h>
#include "runtime.h"

/*------------ picoturtle_turtle_location_t --------------*/
typedef struct {
    float x;
    float y;
} picoturtle_turtle_location_t;

float picoturtle_turtle_location_get_x(const picoturtle_turtle_location_t *loc);
float picoturtle_turtle_location_get_y(const picoturtle_turtle_location_t *loc);
void picoturtle_turtle_location_set_x(picoturtle_turtle_location_t *loc, float x);
void picoturtle_turtle_location_set_y(picoturtle_turtle_location_t *loc, float y);
void picoturtle_turtle_make_location(picoturtle_turtle_location_t **loc);
void picoturtle_turtle_make_location_xy(picoturtle_turtle_location_t **loc, float x, float y);
void picoturtle_turtle_free_location(picoturtle_turtle_location_t *loc);
void picoturtle_turtle_print_location(const picoturtle_turtle_location_t *loc);

/*------------ picoturtle_turtle_colour_t --------------*/
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    char *name;
} picoturtle_turtle_colour_t;

void picoturtle_turtle_make_colour(picoturtle_turtle_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name);

/**
 * @brief Looks up the colour names library for the given RGB values.
 * If the colour is not found, it creates a colour with the given RGB values
 * and the name it was given in the c-color-names library.
 * 
 * The alpha value is set to fully opaque (255).
 * 
 * @param col 
 * @param r 
 * @param g 
 * @param b 
 */
void picoturtle_turtle_make_colour_rgba(picoturtle_turtle_colour_t **col, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Looks up the colour names library for the given name.
 * If the colour is not found, it creates a colour with the name
 * and sets the appropriate RGB values to the colour.
 * 
 * The alpha value is set to fully opaque (255).
 * 
 * @param col 
 * @param name 
 */
void picoturtle_turtle_make_colour_from_name(picoturtle_turtle_colour_t **col, const char *name);

const char* picoturtle_turtle_colour_get_name(const picoturtle_turtle_colour_t *col);
Color picoturtle_turtle_colour_get_raylib_color(const picoturtle_turtle_colour_t *col);
void picoturtle_turtle_free_colour(picoturtle_turtle_colour_t *col);
void picoturtle_turtle_print_colour(const picoturtle_turtle_colour_t *col);

/*------------ picoturtle_turtle_state_t --------------*/
typedef struct {
    picoturtle_turtle_location_t *location;
    picoturtle_turtle_colour_t *pen_colour;
    double heading;
    int pen_down;
    float pen_width;
    int font_size;
    char *font_name;
} picoturtle_turtle_state_t;

void picoturtle_turtle_make_state(picoturtle_turtle_state_t **state);
int picoturtle_turtle_copy_state(const picoturtle_turtle_state_t *source, picoturtle_turtle_state_t **destination);
void picoturtle_turtle_free_state(picoturtle_turtle_state_t *state);
picoturtle_turtle_location_t* picoturtle_turtle_state_get_location(const picoturtle_turtle_state_t *state);
picoturtle_turtle_colour_t* picoturtle_turtle_state_get_pen_colour(const picoturtle_turtle_state_t *state);
double picoturtle_turtle_state_get_heading(const picoturtle_turtle_state_t *state);
void picoturtle_turtle_state_set_heading(picoturtle_turtle_state_t *state, double heading);
int picoturtle_turtle_state_is_pen_down(const picoturtle_turtle_state_t *state);
void picoturtle_turtle_state_set_pen_down(picoturtle_turtle_state_t *state, int pen_down);
float picoturtle_turtle_state_get_pen_width(const picoturtle_turtle_state_t *state);
void picoturtle_turtle_state_set_pen_width(picoturtle_turtle_state_t *state, float pen_width);
int picoturtle_turtle_state_get_font_size(const picoturtle_turtle_state_t *state);
void picoturtle_turtle_state_set_font_size(picoturtle_turtle_state_t *state, int size);
const char* picoturtle_turtle_state_get_font_name(const picoturtle_turtle_state_t *state);
void picoturtle_turtle_state_set_font_name(picoturtle_turtle_state_t *state, const char *font_name);

/*------------ picoturtle_turtle_t --------------*/
typedef struct {
    picoturtle_turtle_state_t *current_state;
    picoturtle_turtle_state_t **saved_states;
    size_t saved_state_count;
    size_t saved_state_capacity;
    char* name;
    char* id;
    uint64_t start_time_ms;
    picoturtle_runtime_t *runtime;
} picoturtle_turtle_t;

void picoturtle_turtle_make_turtle(picoturtle_turtle_t **turtle, const char *name, const char *id);
void picoturtle_turtle_free_turtle(picoturtle_turtle_t *turtle);
picoturtle_turtle_state_t* picoturtle_turtle_get_state(const picoturtle_turtle_t *turtle);
picoturtle_turtle_location_t* picoturtle_turtle_get_location(const picoturtle_turtle_t *turtle);
picoturtle_turtle_colour_t* picoturtle_turtle_get_pen_colour(const picoturtle_turtle_t *turtle);
double picoturtle_turtle_get_heading(const picoturtle_turtle_t *turtle);
double picoturtle_turtle_get_canvas_heading(const picoturtle_turtle_t *turtle);
float picoturtle_turtle_get_canvas_location_x(const picoturtle_turtle_t *turtle);
float picoturtle_turtle_get_canvas_location_y(const picoturtle_turtle_t *turtle);
float picoturtle_turtle_get_pen_width(const picoturtle_turtle_t *turtle);
int picoturtle_turtle_colour(picoturtle_turtle_t *turtle, const char *name);
void picoturtle_turtle_colour_rgba(picoturtle_turtle_t *turtle, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

// drawing miscellaneous functions
void picoturtle_turtle_draw_me(const picoturtle_turtle_t *turtle);

// reset the turtle to its initial state
void picoturtle_turtle_reset(picoturtle_turtle_t *turtle);
void picoturtle_turtle_home(picoturtle_turtle_t *turtle);
int picoturtle_turtle_save(picoturtle_turtle_t *turtle);
int picoturtle_turtle_restore(picoturtle_turtle_t *turtle);
uint64_t picoturtle_turtle_elapsed_time_ms(const picoturtle_turtle_t *turtle);

// movement functions
// with pen movement
void picoturtle_turtle_forward(picoturtle_turtle_t *turtle, float distance);
void picoturtle_turtle_backward(picoturtle_turtle_t *turtle, float distance);
// levitate/teleport without drawing
void picoturtle_turtle_set_position(picoturtle_turtle_t *turtle, float x, float y);
void picoturtle_turtle_set_x(picoturtle_turtle_t *turtle, float x);
void picoturtle_turtle_set_y(picoturtle_turtle_t *turtle, float y);
// heading functions
void picoturtle_turtle_heading(picoturtle_turtle_t *turtle, double heading);
// turn left by angle degrees
void picoturtle_turtle_left(picoturtle_turtle_t *turtle, float angle);
// turn right by angle degrees
void picoturtle_turtle_right(picoturtle_turtle_t *turtle, float angle);
// pen functions
void picoturtle_turtle_pen_down(picoturtle_turtle_t *turtle);
void picoturtle_turtle_pen_up(picoturtle_turtle_t *turtle);
void picoturtle_turtle_pen_width(picoturtle_turtle_t *turtle, float width);
void picoturtle_turtle_circle(picoturtle_turtle_t *turtle, float radius);
void picoturtle_turtle_arc(picoturtle_turtle_t *turtle, float radius, float extent, int steps);

// information functions
void picoturtle_turtle_print_info(const picoturtle_turtle_t *turtle);
void picoturtle_turtle_draw_info(const picoturtle_turtle_t *turtle);

// fps/timing related functions
void picoturtle_turtle_set_target_fps(int fps);
float picoturtle_turtle_get_delta_time(void);
double picoturtle_turtle_get_time(void);
int picoturtle_turtle_get_fps(void);

// text related functions
void picoturtle_turtle_text(const picoturtle_turtle_t *turtle, const char *text);
void picoturtle_turtle_set_font_size(const picoturtle_turtle_t *turtle, int size);
int picoturtle_turtle_get_font_size(const picoturtle_turtle_t *turtle);
void picoturtle_turtle_set_font(const picoturtle_turtle_t *turtle, const char *font_name);
const char* picoturtle_turtle_get_font(const picoturtle_turtle_t *turtle);

// canvas size related functions
int picoturtle_turtle_get_canvas_width(void);
int picoturtle_turtle_get_canvas_height(void);
int picoturtle_turtle_set_canvas_size(picoturtle_turtle_t *turtle, int width, int height);

// canvas clear function
void picoturtle_turtle_clear_canvas_colour(const picoturtle_turtle_t *turtle, const char *color_name);

// clear the canvas with the turtle's pen colour
void picoturtle_turtle_clear_canvas(const picoturtle_turtle_t *turtle);

#endif // ___TURTLE_H___

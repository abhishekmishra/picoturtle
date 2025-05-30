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

/*------------ trtl_location_t --------------*/
typedef struct {
    float x;
    float y;
} trtl_location_t;

float trtl_location_get_x(const trtl_location_t *loc);
float trtl_location_get_y(const trtl_location_t *loc);
void trtl_location_set_x(trtl_location_t *loc, float x);
void trtl_location_set_y(trtl_location_t *loc, float y);
void trtl_make_location(trtl_location_t **loc);
void trtl_make_location_xy(trtl_location_t **loc, float x, float y);
void trtl_free_location(trtl_location_t *loc);
void trtl_print_location(const trtl_location_t *loc);

/*------------ trtl_colour_t --------------*/
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    char *name;
} trtl_colour_t;

void trtl_make_colour(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name);

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
void trtl_make_colour_rgba(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b);

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
void trtl_make_colour_from_name(trtl_colour_t **col, const char *name);

const char* trtl_colour_get_name(const trtl_colour_t *col);
Color trtl_colour_get_raylib_color(const trtl_colour_t *col);
void trtl_free_colour(trtl_colour_t *col);
void trtl_print_colour(const trtl_colour_t *col);

/*------------ trtl_state_t --------------*/
typedef struct {
    trtl_location_t *location;
    trtl_colour_t *pen_colour;
    double heading;
    int pen_down;
    float pen_width;
    int font_size;
    char *font_name;
} trtl_state_t;

void trtl_make_state(trtl_state_t **state);
void trtl_free_state(trtl_state_t *state);
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

void trtl_make_turtle(trtl_t **turtle, const char *name, const char *id);
void trtl_free_turtle(trtl_t *turtle);
trtl_state_t* trtl_get_state(const trtl_t *turtle);
trtl_location_t* trtl_get_location(const trtl_t *turtle);
trtl_colour_t* trtl_get_pen_colour(const trtl_t *turtle);
double trtl_get_heading(const trtl_t *turtle);
double trtl_get_canvas_heading(const trtl_t *turtle);
float trtl_get_canvas_location_x(const trtl_t *turtle);
float trtl_get_canvas_location_y(const trtl_t *turtle);
float trtl_get_pen_width(const trtl_t *turtle);
void trtl_colour(trtl_t *turtle, const char *name);
void trtl_colour_rgba(trtl_t *turtle, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

// drawing miscellaneous functions
void trtl_draw_me(const trtl_t *turtle);

// reset the turtle to its initial state
void trtl_reset(trtl_t *turtle);

// movement functions
// with pen movement
void trtl_forward(trtl_t *turtle, float distance);
void trtl_backward(trtl_t *turtle, float distance);
// levitate/teleport without drawing
void trtl_set_position(trtl_t *turtle, float x, float y);
void trtl_set_x(trtl_t *turtle, float x);
void trtl_set_y(trtl_t *turtle, float y);
// heading functions
void trtl_heading(trtl_t *turtle, double heading);
// turn left by angle degrees
void trtl_left(trtl_t *turtle, float angle);
// turn right by angle degrees
void trtl_right(trtl_t *turtle, float angle);
// pen functions
void trtl_pen_down(trtl_t *turtle);
void trtl_pen_up(trtl_t *turtle);
void trtl_pen_width(trtl_t *turtle, float width);

// information functions
void trtl_print_info(const trtl_t *turtle);
void trtl_draw_info(const trtl_t *turtle);

// fps/timing related functions
void trtl_set_target_fps(int fps);
float trtl_get_delta_time(void);
double trtl_get_time(void);
int trtl_get_fps(void);

// text related functions
void trtl_text(const trtl_t *turtle, const char *text);
void trtl_set_font_size(const trtl_t *turtle, int size);
int trtl_get_font_size(const trtl_t *turtle);
void trtl_set_font(const trtl_t *turtle, const char *font_name);
const char* trtl_get_font(const trtl_t *turtle);

// canvas size related functions
int trtl_get_canvas_width(void);
int trtl_get_canvas_height(void);
// TODO: implement later after thinking of good way to handle hidpi
// void trtl_set_canvas_size(int width, int height);

#endif // ___TURTLE_H___
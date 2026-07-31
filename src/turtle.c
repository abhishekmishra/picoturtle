#include "turtle.h"
#include "color_names.h"
#include "font.h"
#include <time.h>

#ifdef _MSC_VER
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) _snprintf_s(dest, size, _TRUNCATE, fmt, __VA_ARGS__)
#else
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) snprintf(dest, size, fmt, __VA_ARGS__)
#endif

static uint64_t current_time_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return ((uint64_t)now.tv_sec * 1000u) +
        ((uint64_t)now.tv_nsec / 1000000u);
}

float picoturtle_turtle_location_get_x(const picoturtle_turtle_location_t *loc) {
    return loc->x;
}
float picoturtle_turtle_location_get_y(const picoturtle_turtle_location_t *loc) {
    return loc->y;
}
void picoturtle_turtle_location_set_x(picoturtle_turtle_location_t *loc, float x) {
    loc->x = x;
}
void picoturtle_turtle_location_set_y(picoturtle_turtle_location_t *loc, float y) {
    loc->y = y;
}

void picoturtle_turtle_make_location(picoturtle_turtle_location_t **loc)
{
    *loc = (picoturtle_turtle_location_t *)malloc(sizeof(picoturtle_turtle_location_t));
    if (*loc != NULL) {
        (*loc)->x = 0.0f;
        (*loc)->y = 0.0f;
    }
}

void picoturtle_turtle_make_location_xy(picoturtle_turtle_location_t **loc, float x, float y) {
    picoturtle_turtle_make_location(loc);
    if (*loc != NULL) {
        (*loc)->x = x;
        (*loc)->y = y;
    }
}

void picoturtle_turtle_free_location(picoturtle_turtle_location_t *loc) {
    if (loc != NULL) {
        free(loc);
    }
}

void picoturtle_turtle_print_location(const picoturtle_turtle_location_t *loc) {
    if (loc != NULL) {
        printf("Location: (%.2f, %.2f)\n", loc->x, loc->y);
    } else {
        printf("Location is NULL\n");
    }
}

void picoturtle_turtle_make_colour(picoturtle_turtle_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name) {
    *col = (picoturtle_turtle_colour_t *)malloc(sizeof(picoturtle_turtle_colour_t));
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

void picoturtle_turtle_make_colour_rgba(picoturtle_turtle_colour_t **col, uint8_t r, uint8_t g, uint8_t b) {
    // Look up the color name from the c-color-names library
    const char *name = color_name_find_rgb(r, g, b);
    if (!name) {
        name = "unknown";
    }
    picoturtle_turtle_make_colour(col, r, g, b, 255, name);
}

void picoturtle_turtle_make_colour_from_name(picoturtle_turtle_colour_t **col, const char *name) {
    uint8_t r = 0, g = 0, b = 0;
    int found = color_name_get_rgb(name, &r, &g, &b);
    if (!found) {
        *col = NULL;
        return;
    }
    picoturtle_turtle_make_colour(col, r, g, b, 255, name);
}

void picoturtle_turtle_free_colour(picoturtle_turtle_colour_t *col) {
    if (col != NULL) {
        if (col->name != NULL) {
            free(col->name);
        }
        free(col);
    }
}

void picoturtle_turtle_print_colour(const picoturtle_turtle_colour_t *col) {
    if (col != NULL) {
        printf("Colour: (%d, %d, %d, %d), Name: %s\n", col->r, col->g, col->b, col->a, col->name);
    } else {
        printf("Colour is NULL\n");
    }
}

const char* picoturtle_turtle_colour_get_name(const picoturtle_turtle_colour_t *col) {
    if (col && col->name) {
        return col->name;
    }
    return "unknown";
}

Color picoturtle_turtle_colour_get_raylib_color(const picoturtle_turtle_colour_t *col) {
    if (col) {
        return (Color){ col->r, col->g, col->b, col->a };
    }
    // Return opaque black as fallback
    return (Color){ 0, 0, 0, 255 };
}

void picoturtle_turtle_make_state(picoturtle_turtle_state_t **state) {
    *state = (picoturtle_turtle_state_t *)malloc(sizeof(picoturtle_turtle_state_t));
    if (*state != NULL) {
        (*state)->location = NULL;
        (*state)->pen_colour = NULL;
        (*state)->heading = 0.0;
        (*state)->pen_down = 0;
        (*state)->pen_width = 1.0f;
        (*state)->font_size = 20;
        (*state)->font_name = NULL;
    }

    picoturtle_turtle_make_location(&(*state)->location);
    if ((*state)->location == NULL) {
        free(*state);
        *state = NULL;
        return;
    }
    picoturtle_turtle_make_colour(&(*state)->pen_colour, 0, 0, 0, 255, "black");
    if ((*state)->pen_colour == NULL) {
        picoturtle_turtle_free_location((*state)->location);
        free(*state);
        *state = NULL;
        return;
    }
}

int picoturtle_turtle_copy_state(const picoturtle_turtle_state_t *source, picoturtle_turtle_state_t **destination) {
    if (source == NULL || destination == NULL) {
        return 0;
    }

    picoturtle_turtle_state_t *copy = NULL;
    picoturtle_turtle_make_state(&copy);
    if (copy == NULL) {
        return 0;
    }

    picoturtle_turtle_location_set_x(copy->location, picoturtle_turtle_location_get_x(source->location));
    picoturtle_turtle_location_set_y(copy->location, picoturtle_turtle_location_get_y(source->location));

    picoturtle_turtle_free_colour(copy->pen_colour);
    copy->pen_colour = NULL;
    picoturtle_turtle_make_colour(
        &copy->pen_colour,
        source->pen_colour->r,
        source->pen_colour->g,
        source->pen_colour->b,
        source->pen_colour->a,
        picoturtle_turtle_colour_get_name(source->pen_colour)
    );
    if (copy->pen_colour == NULL) {
        picoturtle_turtle_free_state(copy);
        return 0;
    }

    copy->heading = source->heading;
    copy->pen_down = source->pen_down;
    copy->pen_width = source->pen_width;
    copy->font_size = source->font_size;
    picoturtle_turtle_state_set_font_name(copy, source->font_name);

    *destination = copy;
    return 1;
}

void picoturtle_turtle_free_state(picoturtle_turtle_state_t *state) {
    if (state != NULL) {
        picoturtle_turtle_free_location(state->location);
        picoturtle_turtle_free_colour(state->pen_colour);
        if (state->font_name) {
            free(state->font_name);
        }
        free(state);
    }
}

picoturtle_turtle_location_t* picoturtle_turtle_state_get_location(const picoturtle_turtle_state_t *state) {
    return state->location;
}

picoturtle_turtle_colour_t* picoturtle_turtle_state_get_pen_colour(const picoturtle_turtle_state_t *state) {
    return state->pen_colour;
}

double picoturtle_turtle_state_get_heading(const picoturtle_turtle_state_t *state) {
    return state->heading;
}

void picoturtle_turtle_state_set_heading(picoturtle_turtle_state_t *state, double heading) {
    state->heading = heading;
}

int picoturtle_turtle_state_is_pen_down(const picoturtle_turtle_state_t *state) {
    return state->pen_down;
}

void picoturtle_turtle_state_set_pen_down(picoturtle_turtle_state_t *state, int pen_down) {
    state->pen_down = pen_down;
}

float picoturtle_turtle_state_get_pen_width(const picoturtle_turtle_state_t *state) {
    return state->pen_width;
}

void picoturtle_turtle_state_set_pen_width(picoturtle_turtle_state_t *state, float pen_width) {
    state->pen_width = pen_width;
}

int picoturtle_turtle_state_get_font_size(const picoturtle_turtle_state_t *state) {
    return state && state->font_size > 0 ? state->font_size : 20;
}

void picoturtle_turtle_state_set_font_size(picoturtle_turtle_state_t *state, int size) {
    if (state) state->font_size = size;
}

const char* picoturtle_turtle_state_get_font_name(const picoturtle_turtle_state_t *state) {
    return state && state->font_name ? state->font_name : "default";
}

void picoturtle_turtle_state_set_font_name(picoturtle_turtle_state_t *state, const char *font_name) {
    if (!state) return;
    if (state->font_name) free(state->font_name);
    if (font_name) {
        state->font_name = strdup(font_name);
    } else {
        state->font_name = NULL;
    }
}

void picoturtle_turtle_make_turtle(picoturtle_turtle_t **turtle, const char *name, const char *id) {
    *turtle = (picoturtle_turtle_t *)malloc(sizeof(picoturtle_turtle_t));
    if (*turtle != NULL) {
        (*turtle)->current_state = NULL;
        (*turtle)->name = (char *)malloc(strlen(name) + 1);
        if ((*turtle)->name != NULL) {
            strcpy((*turtle)->name, name);
        }
        (*turtle)->id = (char *)malloc(strlen(id) + 1);
        if ((*turtle)->id != NULL) {
            strcpy((*turtle)->id, id);
        }
        (*turtle)->saved_states = NULL;
        (*turtle)->saved_state_count = 0;
        (*turtle)->saved_state_capacity = 0;
        (*turtle)->start_time_ms = current_time_ms();
        (*turtle)->runtime = picoturtle_runtime_get_default();
        picoturtle_turtle_make_state(&(*turtle)->current_state);
        if ((*turtle)->current_state != NULL) {
            picoturtle_turtle_reset(*turtle);
        }
    }
}

void picoturtle_turtle_free_turtle(picoturtle_turtle_t *turtle) {
    if (turtle != NULL) {
        picoturtle_turtle_free_state(turtle->current_state);
        for (size_t index = 0; index < turtle->saved_state_count; index++) {
            picoturtle_turtle_free_state(turtle->saved_states[index]);
        }
        free(turtle->saved_states);
        if (turtle->name != NULL) {
            free(turtle->name);
        }
        if (turtle->id != NULL) {
            free(turtle->id);
        }
        free(turtle);
    }
}

picoturtle_turtle_state_t* picoturtle_turtle_get_state(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL) {
        return turtle->current_state;
    }
    return NULL;
}

picoturtle_turtle_location_t* picoturtle_turtle_get_location(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return picoturtle_turtle_state_get_location(turtle->current_state);
    }
    return NULL;
}

picoturtle_turtle_colour_t* picoturtle_turtle_get_pen_colour(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return picoturtle_turtle_state_get_pen_colour(turtle->current_state);
    }
    return NULL;
}

double picoturtle_turtle_get_heading(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return picoturtle_turtle_state_get_heading(turtle->current_state);
    }
    return 0.0;
}

double picoturtle_turtle_get_canvas_heading(const picoturtle_turtle_t *turtle)
{
    return 360.0 - picoturtle_turtle_get_heading(turtle);
}

float picoturtle_turtle_get_canvas_location_x(const picoturtle_turtle_t *turtle)
{
    return picoturtle_turtle_location_get_x(picoturtle_turtle_get_location(turtle));
}

float picoturtle_turtle_get_canvas_location_y(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL && turtle->runtime != NULL) {
        return (float)turtle->runtime->canvas_height -
            picoturtle_turtle_location_get_y(picoturtle_turtle_get_location(turtle));
    }
    return picoturtle_turtle_location_get_y(picoturtle_turtle_get_location(turtle));
}

float picoturtle_turtle_get_pen_width(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return picoturtle_turtle_state_get_pen_width(turtle->current_state);
    }
    return 1.0f;
}

void picoturtle_turtle_draw_me(const picoturtle_turtle_t *turtle)
{
    if (turtle == NULL || turtle->runtime == NULL ||
        !turtle->runtime->initialized) {
        return;
    }

    int d = 25;
    float theta1 = (float)((picoturtle_turtle_get_canvas_heading(turtle) - 145) * (M_PI / 180));
    float y2 = (float)(d * (sin(theta1)) + picoturtle_turtle_get_canvas_location_y(turtle));
    float x2 = (float)(d * (cos(theta1)) + picoturtle_turtle_get_canvas_location_x(turtle));
    float theta2 = (float)((picoturtle_turtle_get_canvas_heading(turtle) + 145) * (M_PI / 180));
    float y3 = (float)(d * (sin(theta2)) + picoturtle_turtle_get_canvas_location_y(turtle));
    float x3 = (float)(d * (cos(theta2)) + picoturtle_turtle_get_canvas_location_x(turtle));

    Vector2 v1 = {picoturtle_turtle_get_canvas_location_x(turtle), picoturtle_turtle_get_canvas_location_y(turtle)};
    Vector2 v2 = {x2, y2};
    Vector2 v3 = {x3, y3};
    picoturtle_runtime_begin_canvas(turtle->runtime);
    DrawTriangle(
        v1,
        v2,
        v3,
        DARKGREEN 
    );
    picoturtle_runtime_end_canvas(turtle->runtime);
}

// movement functions
// with pen movement
void picoturtle_turtle_forward(picoturtle_turtle_t *turtle, float distance)
{
    float theta = (float)(picoturtle_turtle_get_heading(turtle) * (M_PI / 180));
    float canvas_theta = (float)(picoturtle_turtle_get_canvas_heading(turtle) * (M_PI / 180));
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    picoturtle_turtle_location_t *current_location = picoturtle_turtle_get_location(turtle);
    float y2 = (float)(distance * (sin(theta)) + picoturtle_turtle_location_get_y(current_location));
    float x2 = (float)(distance * (cos(theta)) + picoturtle_turtle_location_get_x(current_location));
    float cy2 = (float)(distance * (sin(canvas_theta)) + picoturtle_turtle_get_canvas_location_y(turtle));
    float cx2 = (float)(distance * (cos(canvas_theta)) + picoturtle_turtle_get_canvas_location_x(turtle));

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    picoturtle_turtle_state_t *current_state = turtle->current_state;
    if (current_state == NULL || current_location == NULL) {
        return; // No state or location to draw from
    }
    if (picoturtle_turtle_state_is_pen_down(current_state)) {
        // Draw the line from the current location to the new location
        // using the pen colour and width
        picoturtle_turtle_colour_t *pen_colour = picoturtle_turtle_get_pen_colour(turtle);
        if (pen_colour == NULL) {
            return; // No pen colour set
        }
        Color color = picoturtle_turtle_colour_get_raylib_color(pen_colour);
        if (turtle->runtime != NULL && turtle->runtime->initialized) {
            picoturtle_runtime_begin_canvas(turtle->runtime);
            DrawLineEx(
                (Vector2){
                    picoturtle_turtle_get_canvas_location_x(turtle),
                    picoturtle_turtle_get_canvas_location_y(turtle)
                },
                (Vector2){cx2, cy2},
                current_state->pen_width,
                color
            );
            picoturtle_runtime_end_canvas(turtle->runtime);
        }
    }

    // set the new location
    picoturtle_turtle_location_set_x(current_location, x2);
    picoturtle_turtle_location_set_y(current_location, y2);
}

void picoturtle_turtle_backward(picoturtle_turtle_t *turtle, float distance)
{
    // to move backward, we just move forward in the opposite direction
    picoturtle_turtle_forward(turtle, -distance);
}

void picoturtle_turtle_set_position(picoturtle_turtle_t *turtle, float x, float y)
{
    picoturtle_turtle_state_t *state = picoturtle_turtle_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        if (picoturtle_turtle_state_is_pen_down(state) && turtle->runtime != NULL &&
            turtle->runtime->initialized) {
            picoturtle_turtle_colour_t *pen_colour = picoturtle_turtle_get_pen_colour(turtle);
            Color color = picoturtle_turtle_colour_get_raylib_color(pen_colour);
            float canvas_y = (float)turtle->runtime->canvas_height - y;
            picoturtle_runtime_begin_canvas(turtle->runtime);
            DrawLineEx(
                (Vector2){
                    picoturtle_turtle_get_canvas_location_x(turtle),
                    picoturtle_turtle_get_canvas_location_y(turtle)
                },
                (Vector2){x, canvas_y},
                state->pen_width,
                color
            );
            picoturtle_runtime_end_canvas(turtle->runtime);
        }
        picoturtle_turtle_location_set_x(state->location, x);
        picoturtle_turtle_location_set_y(state->location, y);
    }
}

void picoturtle_turtle_set_x(picoturtle_turtle_t *turtle, float x)
{
    picoturtle_turtle_state_t *state = picoturtle_turtle_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        picoturtle_turtle_set_position(turtle, x, picoturtle_turtle_location_get_y(state->location));
    }
}

void picoturtle_turtle_set_y(picoturtle_turtle_t *turtle, float y)
{
    picoturtle_turtle_state_t *state = picoturtle_turtle_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        picoturtle_turtle_set_position(turtle, picoturtle_turtle_location_get_x(state->location), y);
    }
}

// heading functions
void picoturtle_turtle_heading(picoturtle_turtle_t *turtle, double heading)
{
    if (turtle != NULL && picoturtle_turtle_get_state(turtle) != NULL) {
        picoturtle_turtle_state_set_heading(picoturtle_turtle_get_state(turtle), heading);
    }
}

void picoturtle_turtle_left(picoturtle_turtle_t *turtle, float angle) {
    if (turtle) {
        double current_heading = picoturtle_turtle_get_heading(turtle);
        double new_heading = fmod(current_heading + angle, 360.0);
        if (new_heading < 0) new_heading += 360.0;
        picoturtle_turtle_heading(turtle, new_heading);
    }
}

void picoturtle_turtle_right(picoturtle_turtle_t *turtle, float angle) {
    if (turtle) {
        double current_heading = picoturtle_turtle_get_heading(turtle);
        double new_heading = fmod(current_heading - angle, 360.0);
        if (new_heading < 0) new_heading += 360.0;
        picoturtle_turtle_heading(turtle, new_heading);
    }
}

void picoturtle_turtle_reset(picoturtle_turtle_t *turtle) {
    if (turtle) {
        picoturtle_turtle_home(turtle);
        picoturtle_turtle_heading(turtle, 90.0);
        picoturtle_turtle_pen_down(turtle);
        if (turtle->current_state) {
            picoturtle_turtle_state_set_pen_width(turtle->current_state, 1.0f);
        }
        if (turtle->runtime != NULL) {
            picoturtle_runtime_clear(turtle->runtime, RAYWHITE);
        }
    }
}

void picoturtle_turtle_home(picoturtle_turtle_t *turtle) {
    if (turtle == NULL || turtle->current_state == NULL) {
        return;
    }

    float x = 0.0f;
    float y = 0.0f;
    if (turtle->runtime != NULL) {
        x = (float)turtle->runtime->canvas_width / 2.0f;
        y = (float)turtle->runtime->canvas_height / 2.0f;
    }
    picoturtle_turtle_location_set_x(turtle->current_state->location, x);
    picoturtle_turtle_location_set_y(turtle->current_state->location, y);
}

int picoturtle_turtle_save(picoturtle_turtle_t *turtle) {
    if (turtle == NULL || turtle->current_state == NULL) {
        return 0;
    }

    picoturtle_turtle_state_t *copy = NULL;
    if (!picoturtle_turtle_copy_state(turtle->current_state, &copy)) {
        return 0;
    }

    if (turtle->saved_state_count == turtle->saved_state_capacity) {
        size_t new_capacity = turtle->saved_state_capacity == 0
            ? 4
            : turtle->saved_state_capacity * 2;
        picoturtle_turtle_state_t **new_states = realloc(
            turtle->saved_states,
            new_capacity * sizeof(*new_states)
        );
        if (new_states == NULL) {
            picoturtle_turtle_free_state(copy);
            return 0;
        }
        turtle->saved_states = new_states;
        turtle->saved_state_capacity = new_capacity;
    }

    turtle->saved_states[turtle->saved_state_count++] = copy;
    return 1;
}

int picoturtle_turtle_restore(picoturtle_turtle_t *turtle) {
    if (turtle == NULL || turtle->saved_state_count == 0) {
        return 0;
    }

    picoturtle_turtle_state_t *restored =
        turtle->saved_states[--turtle->saved_state_count];
    picoturtle_turtle_free_state(turtle->current_state);
    turtle->current_state = restored;
    return 1;
}

uint64_t picoturtle_turtle_elapsed_time_ms(const picoturtle_turtle_t *turtle) {
    if (turtle == NULL) {
        return 0;
    }
    uint64_t now = current_time_ms();
    return now >= turtle->start_time_ms ? now - turtle->start_time_ms : 0;
}

// pen state functions
void picoturtle_turtle_pen_down(picoturtle_turtle_t *turtle) {
    if (turtle && turtle->current_state) {
        picoturtle_turtle_state_set_pen_down(turtle->current_state, 1);
    }
}

void picoturtle_turtle_pen_up(picoturtle_turtle_t *turtle) {
    if (turtle && turtle->current_state) {
        picoturtle_turtle_state_set_pen_down(turtle->current_state, 0);
    }
}

void picoturtle_turtle_pen_width(picoturtle_turtle_t *turtle, float width) {
    if (turtle && turtle->current_state) {
        picoturtle_turtle_state_set_pen_width(turtle->current_state, width);
    }
}

void picoturtle_turtle_circle(picoturtle_turtle_t *turtle, float radius) {
    if (turtle == NULL || turtle->current_state == NULL ||
        turtle->runtime == NULL || !turtle->runtime->initialized ||
        radius == 0.0f) {
        return;
    }

    float absolute_radius = fabsf(radius);
    float half_width = picoturtle_turtle_get_pen_width(turtle) / 2.0f;
    float inner_radius = fmaxf(0.0f, absolute_radius - half_width);
    float outer_radius = absolute_radius + half_width;
    int segments = (int)fminf(fmaxf(absolute_radius, 32.0f), 180.0f);
    Color color = picoturtle_turtle_colour_get_raylib_color(picoturtle_turtle_get_pen_colour(turtle));

    picoturtle_runtime_begin_canvas(turtle->runtime);
    DrawRing(
        (Vector2){
            picoturtle_turtle_get_canvas_location_x(turtle),
            picoturtle_turtle_get_canvas_location_y(turtle)
        },
        inner_radius,
        outer_radius,
        0.0f,
        360.0f,
        segments,
        color
    );
    picoturtle_runtime_end_canvas(turtle->runtime);
}

void picoturtle_turtle_arc(picoturtle_turtle_t *turtle, float radius, float extent, int steps) {
    if (turtle == NULL || radius == 0.0f) {
        return;
    }
    if (extent < 0.0f) {
        extent = 360.0f;
    }
    if (steps <= 0) {
        float fraction = fabsf(extent) / 360.0f;
        steps = 1 + (int)(fminf(11.0f + fabsf(radius) / 6.0f, 59.0f) *
                          fraction);
    }

    float turn = extent / (float)steps;
    float half_turn = turn / 2.0f;
    float segment = 2.0f * radius * sinf(half_turn * (float)(M_PI / 180.0));
    if (radius < 0.0f) {
        segment = -segment;
        turn = -turn;
        half_turn = -half_turn;
    }

    picoturtle_turtle_left(turtle, half_turn);
    for (int i = 0; i < steps; i++) {
        picoturtle_turtle_forward(turtle, segment);
        picoturtle_turtle_right(turtle, turn);
    }
    picoturtle_turtle_right(turtle, half_turn);
}

// Information functions
void picoturtle_turtle_print_info(const picoturtle_turtle_t *turtle)
{
    if (turtle != NULL) {
        printf("Turtle Name: %s\n", turtle->name ? turtle->name : "Unknown");
        printf("Turtle ID: %s\n", turtle->id ? turtle->id : "Unknown");
        printf("Start Time: %llu ms\n",
               (unsigned long long)turtle->start_time_ms);
        if (turtle->current_state != NULL) {
            printf("Current Heading: %.2f\n", picoturtle_turtle_state_get_heading(turtle->current_state));
            printf("Pen Down: %s\n", picoturtle_turtle_state_is_pen_down(turtle->current_state) ? "Yes" : "No");
            printf("Pen Width: %.2f\n", picoturtle_turtle_state_get_pen_width(turtle->current_state));
            printf("Pen Colour: ");
            picoturtle_turtle_print_colour(picoturtle_turtle_state_get_pen_colour(turtle->current_state));
            printf("Location: ");
            picoturtle_turtle_print_location(picoturtle_turtle_state_get_location(turtle->current_state));
            printf("Font Size: %d\n", picoturtle_turtle_state_get_font_size(turtle->current_state));
            printf("Font Name: %s\n", picoturtle_turtle_state_get_font_name(turtle->current_state));
        } else {
            printf("No current state available.\n");
        }
    } else {
        printf("Turtle is NULL.\n");
    }
}

void picoturtle_turtle_draw_info(const picoturtle_turtle_t *turtle)
{
    // draw turtle information such that it shows up in a line at the bottom
    // of the screen, with the turtle's name, id, heading, pen down status, and pen width, and location.
    int font_size = 12;
    int x = 10;
    // Draw at the bottom of the screen
    int y = GetScreenHeight() - font_size - 8; 
    if (turtle != NULL) {
        // Draw turtle information on the screen
        const char * name_text = turtle->name ? turtle->name : "Unknown Turtle";
        int tl = MeasureText(name_text, font_size);
        DrawText(name_text, x, y, font_size, BLACK);
        x += tl + 10; // Move x position to the right for the next text
        const char * id_text = turtle->id ? turtle->id : "Unknown ID";
        tl = MeasureText(id_text, font_size);
        DrawText(id_text, x, y, font_size, BLACK);
        x += tl + 10; // Move x position after name
        char info[256];
        // Add location info before heading
        float loc_x = picoturtle_turtle_location_get_x(picoturtle_turtle_get_location(turtle));
        float loc_y = picoturtle_turtle_location_get_y(picoturtle_turtle_get_location(turtle));
        snprintf_safe(info, sizeof(info), "Location: [%.2f, %.2f]", loc_x, loc_y);
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10;
        snprintf_safe(info, sizeof(info), "Heading: %.2f", picoturtle_turtle_state_get_heading(turtle->current_state));
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10; // Move x position after heading
        snprintf_safe(info, sizeof(info), "Pen Down: %s", picoturtle_turtle_state_is_pen_down(turtle->current_state) ? "Yes" : "No");
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10; // Move x position after pen down status
        snprintf_safe(info, sizeof(info), "Pen Width: %.2f", picoturtle_turtle_state_get_pen_width(turtle->current_state));
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        // x += tl + 10; // Move x position after pen width (not needed)
    } else {
        DrawText("Turtle is NULL", x, 10, font_size, RED);
    }
}

int picoturtle_turtle_colour(picoturtle_turtle_t *turtle, const char *name) {
    if (turtle && turtle->current_state) {
        picoturtle_turtle_colour_t *new_col = NULL;
        picoturtle_turtle_make_colour_from_name(&new_col, name);
        if (new_col) {
            picoturtle_turtle_free_colour(turtle->current_state->pen_colour);
            turtle->current_state->pen_colour = new_col;
            return 1;
        }
    }
    return 0;
}

void picoturtle_turtle_colour_rgba(picoturtle_turtle_t *turtle, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (turtle && turtle->current_state) {
        picoturtle_turtle_colour_t *new_col = NULL;
        picoturtle_turtle_make_colour_rgba(&new_col, r, g, b);
        if (new_col) {
            new_col->a = a;
            picoturtle_turtle_free_colour(turtle->current_state->pen_colour);
            turtle->current_state->pen_colour = new_col;
        }
    }
}

void picoturtle_turtle_text(const picoturtle_turtle_t *turtle, const char *text) {
    if (!turtle || !text || !turtle->runtime ||
        !turtle->runtime->initialized) return;
    picoturtle_turtle_state_t *state = turtle->current_state;
    int font_size = picoturtle_turtle_state_get_font_size(state);
    Color color = picoturtle_turtle_colour_get_raylib_color(state->pen_colour);
    float x = picoturtle_turtle_get_canvas_location_x(turtle);
    float y = picoturtle_turtle_get_canvas_location_y(turtle);
    float rotation = (float)picoturtle_turtle_get_canvas_heading(turtle);
    Font font = picoturtle_font_cache_get(
        &turtle->runtime->font_cache,
        picoturtle_turtle_state_get_font_name(state),
        font_size
    );
    picoturtle_runtime_begin_canvas(turtle->runtime);
    DrawTextPro(
        font,
        text,
        (Vector2){x, y},
        (Vector2){0, 0},
        rotation,
        (float)font_size,
        1,
        color
    );
    picoturtle_runtime_end_canvas(turtle->runtime);
}

void picoturtle_turtle_set_font_size(const picoturtle_turtle_t *turtle, int size) {
    if (!turtle) return;
    picoturtle_turtle_state_set_font_size(turtle->current_state, size);
}

int picoturtle_turtle_get_font_size(const picoturtle_turtle_t *turtle) {
    if (!turtle) return 20;
    return picoturtle_turtle_state_get_font_size(turtle->current_state);
}

void picoturtle_turtle_set_font(const picoturtle_turtle_t *turtle, const char *font_name) {
    if (!turtle) return;
    picoturtle_turtle_state_set_font_name(turtle->current_state, font_name);
}

const char* picoturtle_turtle_get_font(const picoturtle_turtle_t *turtle) {
    if (!turtle) return NULL;
    return picoturtle_turtle_state_get_font_name(turtle->current_state);
}

// fps/timing related functions
void picoturtle_turtle_set_target_fps(int fps) {
    SetTargetFPS(fps);
}

float picoturtle_turtle_get_delta_time(void) {
    return GetFrameTime();
}

double picoturtle_turtle_get_time(void) {
    return GetTime();
}

int picoturtle_turtle_get_fps(void) {
    return GetFPS();
}

// canvas size related functions
int picoturtle_turtle_get_canvas_width(void) {
    picoturtle_runtime_t *runtime = picoturtle_runtime_get_default();
    return runtime != NULL ? runtime->canvas_width : 0;
}

int picoturtle_turtle_get_canvas_height(void) {
    picoturtle_runtime_t *runtime = picoturtle_runtime_get_default();
    return runtime != NULL ? runtime->canvas_height : 0;
}

int picoturtle_turtle_set_canvas_size(picoturtle_turtle_t *turtle, int width, int height) {
    if (turtle == NULL || turtle->runtime == NULL) {
        return 0;
    }
    return picoturtle_runtime_resize(turtle->runtime, width, height) ? 1 : 0;
}

// canvas clear function
void picoturtle_turtle_clear_canvas_colour(const picoturtle_turtle_t *turtle, const char *color_name)
{
    if (!turtle || !color_name) return;

    picoturtle_turtle_colour_t *col = NULL;
    picoturtle_turtle_make_colour_from_name(&col, color_name);
    if (col) {
        Color color = picoturtle_turtle_colour_get_raylib_color(col);
        picoturtle_runtime_clear(turtle->runtime, color);
        picoturtle_turtle_free_colour(col);
    }
}

// clear the canvas with the turtle's pen colour
void picoturtle_turtle_clear_canvas(const picoturtle_turtle_t *turtle)
{
    if (!turtle || !turtle->current_state) return;

    picoturtle_turtle_colour_t *pen_colour = picoturtle_turtle_get_pen_colour(turtle);
    if (pen_colour) {
        Color color = picoturtle_turtle_colour_get_raylib_color(pen_colour);
        picoturtle_runtime_clear(turtle->runtime, color);
    } else {
        // If no pen colour is set, clear with black
        picoturtle_runtime_clear(turtle->runtime, BLACK);
    }
}

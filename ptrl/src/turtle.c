#include "turtle.h"
#include "color_names.h"

#ifdef _MSC_VER
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) _snprintf_s(dest, size, _TRUNCATE, fmt, __VA_ARGS__)
#else
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) snprintf(dest, size, fmt, __VA_ARGS__)
#endif

float trtl_location_get_x(const trtl_location_t *loc) {
    return loc->x;
}
float trtl_location_get_y(const trtl_location_t *loc) {
    return loc->y;
}
void trtl_location_set_x(trtl_location_t *loc, float x) {
    loc->x = x;
}
void trtl_location_set_y(trtl_location_t *loc, float y) {
    loc->y = y;
}

void trtl_make_location(trtl_location_t **loc)
{
    *loc = (trtl_location_t *)malloc(sizeof(trtl_location_t));
    if (*loc != NULL) {
        (*loc)->x = 0.0f;
        (*loc)->y = 0.0f;
    }
}

void trtl_make_location_xy(trtl_location_t **loc, float x, float y) {
    trtl_make_location(loc);
    if (*loc != NULL) {
        (*loc)->x = x;
        (*loc)->y = y;
    }
}

void trtl_free_location(trtl_location_t *loc) {
    if (loc != NULL) {
        free(loc);
    }
}

void trtl_print_location(const trtl_location_t *loc) {
    if (loc != NULL) {
        printf("Location: (%.2f, %.2f)\n", loc->x, loc->y);
    } else {
        printf("Location is NULL\n");
    }
}

void trtl_make_colour(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char *name) {
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

void trtl_make_colour_rgba(trtl_colour_t **col, uint8_t r, uint8_t g, uint8_t b) {
    // Look up the color name from the c-color-names library
    const char *name = color_name_find_rgb(r, g, b);
    if (!name) {
        name = "unknown";
    }
    trtl_make_colour(col, r, g, b, 255, name);
}

void trtl_make_colour_from_name(trtl_colour_t **col, const char *name) {
    uint8_t r = 0, g = 0, b = 0;
    int found = color_name_get_rgb(name, &r, &g, &b);
    if (!found) {
        // If not found, use black as fallback
        r = g = b = 0;
    }
    trtl_make_colour(col, r, g, b, 255, name);
}

void trtl_free_colour(trtl_colour_t *col) {
    if (col != NULL) {
        if (col->name != NULL) {
            free(col->name);
        }
        free(col);
    }
}

void trtl_print_colour(const trtl_colour_t *col) {
    if (col != NULL) {
        printf("Colour: (%d, %d, %d, %d), Name: %s\n", col->r, col->g, col->b, col->a, col->name);
    } else {
        printf("Colour is NULL\n");
    }
}

const char* trtl_colour_get_name(const trtl_colour_t *col) {
    if (col && col->name) {
        return col->name;
    }
    return "unknown";
}

Color trtl_colour_get_raylib_color(const trtl_colour_t *col) {
    if (col) {
        return (Color){ col->r, col->g, col->b, col->a };
    }
    // Return opaque black as fallback
    return (Color){ 0, 0, 0, 255 };
}

void trtl_make_state(trtl_state_t **state) {
    *state = (trtl_state_t *)malloc(sizeof(trtl_state_t));
    if (*state != NULL) {
        (*state)->location = NULL;
        (*state)->pen_colour = NULL;
        (*state)->heading = 0.0;
        (*state)->pen_down = 0;
        (*state)->pen_width = 1.0f;
        (*state)->font_size = 20;
        (*state)->font_name = NULL;
    }

    trtl_make_location(&(*state)->location);
    if ((*state)->location == NULL) {
        free(*state);
        *state = NULL;
        return;
    }
    trtl_make_colour(&(*state)->pen_colour, 0, 0, 0, 255, "black");
    if ((*state)->pen_colour == NULL) {
        trtl_free_location((*state)->location);
        free(*state);
        *state = NULL;
        return;
    }
}

void trtl_free_state(trtl_state_t *state) {
    if (state != NULL) {
        trtl_free_location(state->location);
        trtl_free_colour(state->pen_colour);
        if (state->font_name) {
            free(state->font_name);
        }
        free(state);
    }
}

trtl_location_t* trtl_state_get_location(const trtl_state_t *state) {
    return state->location;
}

trtl_colour_t* trtl_state_get_pen_colour(const trtl_state_t *state) {
    return state->pen_colour;
}

double trtl_state_get_heading(const trtl_state_t *state) {
    return state->heading;
}

void trtl_state_set_heading(trtl_state_t *state, double heading) {
    state->heading = heading;
}

int trtl_state_is_pen_down(const trtl_state_t *state) {
    return state->pen_down;
}

void trtl_state_set_pen_down(trtl_state_t *state, int pen_down) {
    state->pen_down = pen_down;
}

float trtl_state_get_pen_width(const trtl_state_t *state) {
    return state->pen_width;
}

void trtl_state_set_pen_width(trtl_state_t *state, float pen_width) {
    state->pen_width = pen_width;
}

int trtl_state_get_font_size(const trtl_state_t *state) {
    return state && state->font_size > 0 ? state->font_size : 20;
}

void trtl_state_set_font_size(trtl_state_t *state, int size) {
    if (state) state->font_size = size;
}

const char* trtl_state_get_font_name(const trtl_state_t *state) {
    return state && state->font_name ? state->font_name : "default";
}

void trtl_state_set_font_name(trtl_state_t *state, const char *font_name) {
    if (!state) return;
    if (state->font_name) free(state->font_name);
    if (font_name) {
        state->font_name = strdup(font_name);
    } else {
        state->font_name = NULL;
    }
}

void trtl_make_turtle(trtl_t **turtle, const char *name, const char *id) {
    *turtle = (trtl_t *)malloc(sizeof(trtl_t));
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
        (*turtle)->start_time = 0;
        trtl_make_state(&(*turtle)->current_state);
    }
}

void trtl_free_turtle(trtl_t *turtle) {
    if (turtle != NULL) {
        trtl_free_state(turtle->current_state);
        if (turtle->name != NULL) {
            free(turtle->name);
        }
        if (turtle->id != NULL) {
            free(turtle->id);
        }
        free(turtle);
    }
}

trtl_state_t* trtl_get_state(const trtl_t *turtle)
{
    if (turtle != NULL) {
        return turtle->current_state;
    }
    return NULL;
}

trtl_location_t* trtl_get_location(const trtl_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return trtl_state_get_location(turtle->current_state);
    }
    return NULL;
}

trtl_colour_t* trtl_get_pen_colour(const trtl_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return trtl_state_get_pen_colour(turtle->current_state);
    }
    return NULL;
}

double trtl_get_heading(const trtl_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return trtl_state_get_heading(turtle->current_state);
    }
    return 0.0;
}

double trtl_get_canvas_heading(const trtl_t *turtle)
{
    return trtl_get_heading(turtle);
}

float trtl_get_canvas_location_x(const trtl_t *turtle)
{
    return trtl_location_get_x(trtl_get_location(turtle));
}

float trtl_get_canvas_location_y(const trtl_t *turtle)
{
    return trtl_location_get_y(trtl_get_location(turtle));
}

float trtl_get_pen_width(const trtl_t *turtle)
{
    if (turtle != NULL && turtle->current_state != NULL) {
        return trtl_state_get_pen_width(turtle->current_state);
    }
    return 1.0f;
}

void trtl_draw_me(const trtl_t *turtle)
{
    int d = 25;
    float theta1 = (float)((trtl_get_canvas_heading(turtle) - 145) * (M_PI / 180));
    float y2 = (float)(d * (sin(theta1)) + trtl_get_canvas_location_y(turtle));
    float x2 = (float)(d * (cos(theta1)) + trtl_get_canvas_location_x(turtle));
    float theta2 = (float)((trtl_get_canvas_heading(turtle) + 145) * (M_PI / 180));
    float y3 = (float)(d * (sin(theta2)) + trtl_get_canvas_location_y(turtle));
    float x3 = (float)(d * (cos(theta2)) + trtl_get_canvas_location_x(turtle));

    short cr = trtl_get_pen_colour(turtle)->r;
    short cg = trtl_get_pen_colour(turtle)->g;
    short cb = trtl_get_pen_colour(turtle)->b;
    const char* cname = trtl_get_pen_colour(turtle)->name ? trtl_get_pen_colour(turtle)->name : "unknown";

    float cpw = trtl_get_pen_width(turtle);

    Vector2 v1 = {trtl_get_canvas_location_x(turtle), trtl_get_canvas_location_y(turtle)};
    Vector2 v2 = {x2, y2};
    Vector2 v3 = {x3, y3};
    DrawTriangle(
        v1,
        v2,
        v3,
        DARKGREEN 
    );
}

// movement functions
// with pen movement
void trtl_forward(trtl_t *turtle, float distance)
{
    float theta = (float)(trtl_get_heading(turtle) * (M_PI / 180));
    float canvas_theta = (float)(trtl_get_canvas_heading(turtle) * (M_PI / 180));
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    trtl_location_t *current_location = trtl_get_location(turtle);
    float y2 = (float)(distance * (sin(theta)) + trtl_location_get_y(current_location));
    float x2 = (float)(distance * (cos(theta)) + trtl_location_get_x(current_location));
    float cy2 = (float)(distance * (sin(canvas_theta)) + trtl_get_canvas_location_y(turtle));
    float cx2 = (float)(distance * (cos(canvas_theta)) + trtl_get_canvas_location_x(turtle));

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    trtl_state_t *current_state = turtle->current_state;
    if (current_state == NULL || current_location == NULL) {
        return; // No state or location to draw from
    }
    if (trtl_state_is_pen_down(current_state)) {
        // Draw the line from the current location to the new location
        // using the pen colour and width
        trtl_colour_t *pen_colour = trtl_get_pen_colour(turtle);
        if (pen_colour == NULL) {
            return; // No pen colour set
        }
        Color color = trtl_colour_get_raylib_color(pen_colour);
        DrawLineEx(
            (Vector2){trtl_location_get_x(current_location), trtl_location_get_y(current_location)},
            (Vector2){x2, y2},
            current_state->pen_width,
            color
        );
    }

    // set the new location
    trtl_location_set_x(current_location, x2);
    trtl_location_set_y(current_location, y2);
}

void trtl_backward(trtl_t *turtle, float distance)
{
    // to move backward, we just move forward in the opposite direction
    trtl_forward(turtle, -distance);
}

// levitate/teleport without drawing
void trtl_set_position(trtl_t *turtle, float x, float y)
{
    trtl_state_t *state = trtl_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        trtl_location_set_x(state->location, x);
        trtl_location_set_y(state->location, y);
    }
}

void trtl_set_x(trtl_t *turtle, float x)
{
    trtl_state_t *state = trtl_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        trtl_location_set_x(state->location, x);
    }
}

void trtl_set_y(trtl_t *turtle, float y)
{
    trtl_state_t *state = trtl_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        trtl_location_set_y(state->location, y);
    }
}

// heading functions
void trtl_heading(trtl_t *turtle, double heading)
{
    if (turtle != NULL && trtl_get_state(turtle) != NULL) {
        trtl_state_set_heading(trtl_get_state(turtle), heading);
    }
}

void trtl_left(trtl_t *turtle, float angle) {
    if (turtle) {
        double current_heading = trtl_get_heading(turtle);
        double new_heading = fmod(current_heading + angle, 360.0);
        if (new_heading < 0) new_heading += 360.0;
        trtl_heading(turtle, new_heading);
    }
}

void trtl_right(trtl_t *turtle, float angle) {
    if (turtle) {
        double current_heading = trtl_get_heading(turtle);
        double new_heading = fmod(current_heading - angle, 360.0);
        if (new_heading < 0) new_heading += 360.0;
        trtl_heading(turtle, new_heading);
    }
}

void trtl_reset(trtl_t *turtle) {
    if (turtle) {
        trtl_set_position(turtle, 0.0f, 0.0f);
        trtl_heading(turtle, 0.0);
        trtl_pen_down(turtle);
        // TODO: Replace direct state access with trtl_set_pen_width when available
        if (turtle->current_state) {
            trtl_state_set_pen_width(turtle->current_state, 1.0f);
        }
        // Not resetting pen_colour here, as it may be user-defined
    }
}

// pen state functions
void trtl_pen_down(trtl_t *turtle) {
    if (turtle && turtle->current_state) {
        trtl_state_set_pen_down(turtle->current_state, 1);
    }
}

void trtl_pen_up(trtl_t *turtle) {
    if (turtle && turtle->current_state) {
        trtl_state_set_pen_down(turtle->current_state, 0);
    }
}

void trtl_pen_width(trtl_t *turtle, float width) {
    if (turtle && turtle->current_state) {
        trtl_state_set_pen_width(turtle->current_state, width);
    }
}

// Information functions
void trtl_print_info(const trtl_t *turtle)
{
    if (turtle != NULL) {
        printf("Turtle Name: %s\n", turtle->name ? turtle->name : "Unknown");
        printf("Turtle ID: %s\n", turtle->id ? turtle->id : "Unknown");
        printf("Start Time: %ld\n", turtle->start_time);
        if (turtle->current_state != NULL) {
            printf("Current Heading: %.2f\n", trtl_state_get_heading(turtle->current_state));
            printf("Pen Down: %s\n", trtl_state_is_pen_down(turtle->current_state) ? "Yes" : "No");
            printf("Pen Width: %.2f\n", trtl_state_get_pen_width(turtle->current_state));
            printf("Pen Colour: ");
            trtl_print_colour(trtl_state_get_pen_colour(turtle->current_state));
            printf("Location: ");
            trtl_print_location(trtl_state_get_location(turtle->current_state));
            printf("Font Size: %d\n", trtl_state_get_font_size(turtle->current_state));
            printf("Font Name: %s\n", trtl_state_get_font_name(turtle->current_state));
        } else {
            printf("No current state available.\n");
        }
    } else {
        printf("Turtle is NULL.\n");
    }
}

void trtl_draw_info(const trtl_t *turtle)
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
        float loc_x = trtl_location_get_x(trtl_get_location(turtle));
        float loc_y = trtl_location_get_y(trtl_get_location(turtle));
        snprintf_safe(info, sizeof(info), "Location: [%.2f, %.2f]", loc_x, loc_y);
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10;
        snprintf_safe(info, sizeof(info), "Heading: %.2f", trtl_state_get_heading(turtle->current_state));
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10; // Move x position after heading
        snprintf_safe(info, sizeof(info), "Pen Down: %s", trtl_state_is_pen_down(turtle->current_state) ? "Yes" : "No");
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        x += tl + 10; // Move x position after pen down status
        snprintf_safe(info, sizeof(info), "Pen Width: %.2f", trtl_state_get_pen_width(turtle->current_state));
        tl = MeasureText(info, font_size);
        DrawText(info, x, y, font_size, BLACK);
        // x += tl + 10; // Move x position after pen width (not needed)
    } else {
        DrawText("Turtle is NULL", x, 10, font_size, RED);
    }
}

void trtl_colour(trtl_t *turtle, const char *name) {
    if (turtle && turtle->current_state) {
        trtl_colour_t *new_col = NULL;
        trtl_make_colour_from_name(&new_col, name);
        if (new_col) {
            trtl_free_colour(turtle->current_state->pen_colour);
            turtle->current_state->pen_colour = new_col;
        }
    }
}

void trtl_colour_rgba(trtl_t *turtle, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (turtle && turtle->current_state) {
        trtl_colour_t *new_col = NULL;
        trtl_make_colour_rgba(&new_col, r, g, b);
        if (new_col) {
            new_col->a = a;
            trtl_free_colour(turtle->current_state->pen_colour);
            turtle->current_state->pen_colour = new_col;
        }
    }
}

void trtl_text(const trtl_t *turtle, const char *text) {
    if (!turtle || !text) return;
    trtl_state_t *state = turtle->current_state;
    trtl_location_t *loc = state->location;
    int font_size = trtl_state_get_font_size(state);
    Color color = trtl_colour_get_raylib_color(state->pen_colour);
    float x = loc->x;
    float y = loc->y;
    float rotation = (float)state->heading;
    // For now, use raylib's default font
    DrawTextPro(GetFontDefault(), text, (Vector2){x, y}, (Vector2){0, 0}, rotation, (float)font_size, 1, color);
}

void trtl_set_font_size(const trtl_t *turtle, int size) {
    if (!turtle) return;
    trtl_state_set_font_size(turtle->current_state, size);
}

int trtl_get_font_size(const trtl_t *turtle) {
    if (!turtle) return 20;
    return trtl_state_get_font_size(turtle->current_state);
}

void trtl_set_font(const trtl_t *turtle, const char *font_name) {
    if (!turtle) return;
    trtl_state_set_font_name(turtle->current_state, font_name);
}

const char* trtl_get_font(const trtl_t *turtle) {
    if (!turtle) return NULL;
    return trtl_state_get_font_name(turtle->current_state);
}

// fps/timing related functions
void trtl_set_target_fps(int fps) {
    SetTargetFPS(fps);
}

float trtl_get_delta_time(void) {
    return GetFrameTime();
}

double trtl_get_time(void) {
    return GetTime();
}

int trtl_get_fps(void) {
    return GetFPS();
}

// canvas size related functions
int trtl_get_canvas_width(void) {
    return GetRenderWidth();
}

int trtl_get_canvas_height(void) {
    return GetRenderHeight();
}

// canvas clear function
void trtl_clear_canvas_colour(const trtl_t *turtle, const char *color_name)
{
    if (!turtle || !color_name) return;

    trtl_colour_t *col = NULL;
    trtl_make_colour_from_name(&col, color_name);
    if (col) {
        Color color = trtl_colour_get_raylib_color(col);
        ClearBackground(color);
        trtl_free_colour(col);
    } else {
        // If the color is not found, clear with black
        ClearBackground(BLACK);
    }
}

// clear the canvas with the turtle's pen colour
void trtl_clear_canvas(const trtl_t *turtle)
{
    if (!turtle || !turtle->current_state) return;

    trtl_colour_t *pen_colour = trtl_get_pen_colour(turtle);
    if (pen_colour) {
        Color color = trtl_colour_get_raylib_color(pen_colour);
        ClearBackground(color);
    } else {
        // If no pen colour is set, clear with black
        ClearBackground(BLACK);
    }
}

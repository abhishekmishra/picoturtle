#include "turtle.h"

#ifdef _MSC_VER
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) _snprintf_s(dest, size, _TRUNCATE, fmt, __VA_ARGS__)
#else
#include <stdio.h>
#define snprintf_safe(dest, size, fmt, ...) snprintf(dest, size, fmt, __VA_ARGS__)
#endif

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

void make_location_xy(trtl_location_t **loc, float x, float y) {
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

void make_state(trtl_state_t **state) {
    *state = (trtl_state_t *)malloc(sizeof(trtl_state_t));
    if (*state != NULL) {
        (*state)->location = NULL;
        (*state)->pen_colour = NULL;
        (*state)->heading = 0.0;
        (*state)->pen_down = 0;
        (*state)->pen_width = 1.0f;
    }

    make_location(&(*state)->location);
    if ((*state)->location == NULL) {
        free(*state);
        *state = NULL;
        return;
    }
    make_colour(&(*state)->pen_colour, 0, 0, 0, 255, "black");
    if ((*state)->pen_colour == NULL) {
        free_location((*state)->location);
        free(*state);
        *state = NULL;
        return;
    }
}

void free_state(trtl_state_t *state) {
    if (state != NULL) {
        free_location(state->location);
        free_colour(state->pen_colour);
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

void make_turtle(trtl_t **turtle, const char *name, const char *id) {
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
        make_state(&(*turtle)->current_state);
    }
}

void free_turtle(trtl_t *turtle) {
    if (turtle != NULL) {
        free_state(turtle->current_state);
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
    return location_get_x(trtl_get_location(turtle));
}

float trtl_get_canvas_location_y(const trtl_t *turtle)
{
    return location_get_y(trtl_get_location(turtle));
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
    float theta1 = (trtl_get_canvas_heading(turtle) - 145) * (M_PI / 180);
    float y2 = d * (sin(theta1)) + trtl_get_canvas_location_y(turtle);
    float x2 = d * (cos(theta1)) + trtl_get_canvas_location_x(turtle);
    float theta2 = (trtl_get_canvas_heading(turtle) + 145) * (M_PI / 180);
    float y3 = d * (sin(theta2)) + trtl_get_canvas_location_y(turtle);
    float x3 = d * (cos(theta2)) + trtl_get_canvas_location_x(turtle);

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
    float theta = trtl_get_heading(turtle) * (M_PI / 180);
    float canvas_theta = trtl_get_canvas_heading(turtle) * (M_PI / 180);
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    trtl_location_t *current_location = trtl_get_location(turtle);
    float y2 = distance * (sin(theta)) + location_get_y(current_location);
    float x2 = distance * (cos(theta)) + location_get_x(current_location);
    float cy2 = distance * (sin(canvas_theta)) + trtl_get_canvas_location_y(turtle);
    float cx2 = distance * (cos(canvas_theta)) + trtl_get_canvas_location_x(turtle);

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    trtl_state_t *current_state = turtle->current_state;
    if (current_state == NULL || current_location == NULL) {
        return; // No state or location to draw from
    }
    if (trtl_state_is_pen_down(current_state)) {
        // Draw the line from the current location to the new location
        // using the pen colour and width
        Color color = (Color){
            current_state->pen_colour->r,
            current_state->pen_colour->g,
            current_state->pen_colour->b,
            current_state->pen_colour->a
        };
        DrawLineEx(
            (Vector2){location_get_x(current_location), location_get_y(current_location)},
            (Vector2){x2, y2},
            current_state->pen_width,
            color
        );
    }

    // set the new location
    location_set_x(current_location, x2);
    location_set_y(current_location, y2);
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
        location_set_x(state->location, x);
        location_set_y(state->location, y);
    }
}

void trtl_set_x(trtl_t *turtle, float x)
{
    trtl_state_t *state = trtl_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        location_set_x(state->location, x);
    }
}

void trtl_set_y(trtl_t *turtle, float y)
{
    trtl_state_t *state = trtl_get_state(turtle);
    if (turtle != NULL && state != NULL && state->location != NULL) {
        location_set_y(state->location, y);
    }
}

// heading functions
void trtl_set_heading(trtl_t *turtle, double heading)
{
    if (turtle != NULL && trtl_get_state(turtle) != NULL) {
        trtl_state_set_heading(trtl_get_state(turtle), heading);
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
            print_colour(trtl_state_get_pen_colour(turtle->current_state));
            printf("Location: ");
            print_location(trtl_state_get_location(turtle->current_state));
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
        float loc_x = location_get_x(trtl_get_location(turtle));
        float loc_y = location_get_y(trtl_get_location(turtle));
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
#include <assert.h>
#include <math.h>
#include <string.h>

#include "runtime.h"
#include "turtle.h"

static int nearly_equal(float left, float right) {
    return fabsf(left - right) < 0.001f;
}

int main(void) {
    picoturtle_turtle_location_t *location = NULL;
    picoturtle_turtle_make_location_xy(&location, 12.0f, 34.0f);
    assert(location != NULL);
    assert(nearly_equal(picoturtle_turtle_location_get_x(location), 12.0f));
    assert(nearly_equal(picoturtle_turtle_location_get_y(location), 34.0f));
    picoturtle_turtle_location_set_x(location, -5.0f);
    picoturtle_turtle_location_set_y(location, 8.0f);
    assert(nearly_equal(location->x, -5.0f));
    assert(nearly_equal(location->y, 8.0f));
    picoturtle_turtle_free_location(location);

    picoturtle_turtle_colour_t *colour = NULL;
    picoturtle_turtle_make_colour_from_name(&colour, "red");
    assert(colour != NULL);
    assert(colour->r == 255 && colour->g == 0 && colour->b == 0);
    picoturtle_turtle_free_colour(colour);
    colour = NULL;
    picoturtle_turtle_make_colour_from_name(&colour, "not-a-colour");
    assert(colour == NULL);
    picoturtle_turtle_make_colour_rgba(&colour, 12, 34, 56);
    assert(colour != NULL);
    assert(colour->r == 12 && colour->g == 34 && colour->b == 56);
    assert(colour->a == 255);
    picoturtle_turtle_free_colour(colour);

    picoturtle_runtime_t runtime = {
        .canvas_width = 800,
        .canvas_height = 600,
        .initialized = false
    };
    picoturtle_runtime_set_default(&runtime);

    picoturtle_turtle_t *first = NULL;
    picoturtle_turtle_t *second = NULL;
    picoturtle_turtle_make_turtle(&first, "first", "turtle-1");
    picoturtle_turtle_make_turtle(&second, "second", "turtle-2");

    assert(first != NULL);
    assert(second != NULL);
    assert(first->runtime == &runtime);
    assert(second->runtime == &runtime);
    assert(first->current_state != second->current_state);
    assert(nearly_equal(picoturtle_turtle_get_heading(first), 90.0f));
    assert(first->current_state->pen_down);
    assert(nearly_equal(picoturtle_turtle_get_pen_width(first), 1.0f));

    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 300.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->y, 300.0f));

    picoturtle_turtle_heading(first, 450.0);
    assert(nearly_equal(picoturtle_turtle_get_heading(first), 450.0f));
    picoturtle_turtle_right(first, 180.0f);
    assert(nearly_equal(picoturtle_turtle_get_heading(first), 270.0f));
    picoturtle_turtle_left(first, 180.0f);
    assert(nearly_equal(picoturtle_turtle_get_heading(first), 90.0f));

    picoturtle_turtle_colour(first, "red");
    picoturtle_turtle_pen_width(first, 7.0f);
    assert(picoturtle_turtle_save(first));

    picoturtle_turtle_pen_up(first);
    picoturtle_turtle_forward(first, 25.0f);
    picoturtle_turtle_colour(first, "blue");
    picoturtle_turtle_pen_width(first, 2.0f);
    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 325.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->y, 300.0f));

    picoturtle_turtle_set_x(first, 10.0f);
    picoturtle_turtle_set_y(first, 20.0f);
    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 10.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 20.0f));

    assert(picoturtle_turtle_restore(first));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 300.0f));
    assert(nearly_equal(picoturtle_turtle_get_pen_width(first), 7.0f));
    assert(first->current_state->pen_colour->r == 255);
    assert(first->current_state->pen_colour->g == 0);
    assert(first->current_state->pen_colour->b == 0);
    assert(first->current_state->pen_down);
    assert(!picoturtle_turtle_restore(first));

    picoturtle_turtle_set_font(first, "Noto Sans");
    picoturtle_turtle_set_font_size(first, 42);
    picoturtle_turtle_state_t *snapshot = NULL;
    assert(picoturtle_turtle_copy_state(first->current_state, &snapshot));
    picoturtle_turtle_location_set_x(first->current_state->location, 123.0f);
    picoturtle_turtle_set_font(first, "changed");
    assert(nearly_equal(snapshot->location->x, 400.0f));
    assert(strcmp(snapshot->font_name, "Noto Sans") == 0);
    assert(snapshot->font_size == 42);
    picoturtle_turtle_free_state(snapshot);
    assert(picoturtle_turtle_elapsed_time_ms(first) < 10000);

    assert(picoturtle_turtle_set_canvas_size(first, 640, 480));
    assert(runtime.canvas_width == 640);
    assert(runtime.canvas_height == 480);
    picoturtle_turtle_reset(first);
    assert(nearly_equal(first->current_state->location->x, 320.0f));
    assert(nearly_equal(first->current_state->location->y, 240.0f));
    assert(nearly_equal(first->current_state->heading, 90.0f));
    assert(first->current_state->pen_down);
    assert(nearly_equal(first->current_state->pen_width, 1.0f));

    picoturtle_turtle_free_turtle(first);
    picoturtle_turtle_free_turtle(second);
    picoturtle_runtime_set_default(NULL);
    return 0;
}

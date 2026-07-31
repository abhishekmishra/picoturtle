#include <assert.h>
#include <math.h>

#include "runtime.h"
#include "turtle.h"

static int nearly_equal(float left, float right) {
    return fabsf(left - right) < 0.001f;
}

int main(void) {
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

    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 300.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(second)->y, 300.0f));

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

    assert(picoturtle_turtle_restore(first));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->x, 400.0f));
    assert(nearly_equal(picoturtle_turtle_get_location(first)->y, 300.0f));
    assert(nearly_equal(picoturtle_turtle_get_pen_width(first), 7.0f));
    assert(first->current_state->pen_colour->r == 255);
    assert(first->current_state->pen_colour->g == 0);
    assert(first->current_state->pen_colour->b == 0);
    assert(!picoturtle_turtle_restore(first));

    picoturtle_turtle_state_t *snapshot = NULL;
    assert(picoturtle_turtle_copy_state(first->current_state, &snapshot));
    picoturtle_turtle_location_set_x(first->current_state->location, 123.0f);
    assert(nearly_equal(snapshot->location->x, 400.0f));
    picoturtle_turtle_free_state(snapshot);
    assert(picoturtle_turtle_elapsed_time_ms(first) < 10000);

    picoturtle_turtle_free_turtle(first);
    picoturtle_turtle_free_turtle(second);
    picoturtle_runtime_set_default(NULL);
    return 0;
}

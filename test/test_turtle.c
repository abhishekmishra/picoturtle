#include <assert.h>
#include <math.h>

#include "runtime.h"
#include "turtle.h"

static int nearly_equal(float left, float right) {
    return fabsf(left - right) < 0.001f;
}

int main(void) {
    ptrl_runtime_t runtime = {
        .canvas_width = 800,
        .canvas_height = 600,
        .initialized = false
    };
    ptrl_runtime_set_default(&runtime);

    trtl_t *first = NULL;
    trtl_t *second = NULL;
    trtl_make_turtle(&first, "first", "turtle-1");
    trtl_make_turtle(&second, "second", "turtle-2");

    assert(first != NULL);
    assert(second != NULL);
    assert(first->runtime == &runtime);
    assert(second->runtime == &runtime);
    assert(first->current_state != second->current_state);

    assert(nearly_equal(trtl_get_location(first)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(first)->y, 300.0f));
    assert(nearly_equal(trtl_get_location(second)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(second)->y, 300.0f));

    trtl_colour(first, "red");
    trtl_pen_width(first, 7.0f);
    assert(trtl_save(first));

    trtl_pen_up(first);
    trtl_forward(first, 25.0f);
    trtl_colour(first, "blue");
    trtl_pen_width(first, 2.0f);
    assert(nearly_equal(trtl_get_location(first)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(first)->y, 325.0f));
    assert(nearly_equal(trtl_get_location(second)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(second)->y, 300.0f));

    assert(trtl_restore(first));
    assert(nearly_equal(trtl_get_location(first)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(first)->y, 300.0f));
    assert(nearly_equal(trtl_get_pen_width(first), 7.0f));
    assert(first->current_state->pen_colour->r == 255);
    assert(first->current_state->pen_colour->g == 0);
    assert(first->current_state->pen_colour->b == 0);
    assert(!trtl_restore(first));

    trtl_state_t *snapshot = NULL;
    assert(trtl_copy_state(first->current_state, &snapshot));
    trtl_location_set_x(first->current_state->location, 123.0f);
    assert(nearly_equal(snapshot->location->x, 400.0f));
    trtl_free_state(snapshot);
    assert(trtl_elapsed_time_ms(first) < 10000);

    trtl_free_turtle(first);
    trtl_free_turtle(second);
    ptrl_runtime_set_default(NULL);
    return 0;
}

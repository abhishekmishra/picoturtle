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

    trtl_pen_up(first);
    trtl_forward(first, 25.0f);
    assert(nearly_equal(trtl_get_location(first)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(first)->y, 325.0f));
    assert(nearly_equal(trtl_get_location(second)->x, 400.0f));
    assert(nearly_equal(trtl_get_location(second)->y, 300.0f));

    trtl_free_turtle(first);
    trtl_free_turtle(second);
    ptrl_runtime_set_default(NULL);
    return 0;
}

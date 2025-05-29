#ifndef __TEST_DRAWING_FEATURES_H__
#define __TEST_DRAWING_FEATURES_H__

#include <raylib.h>
#include "turtle.h"

void draw_test_graphics_start(trtl_t *turtle)
{
    if (turtle == NULL || turtle->current_state == NULL) {
        return; // Nothing to draw
    }

    trtl_location_t *location = trtl_get_location(turtle);
    if (location == NULL) {
        return; // No location to draw from
    }

    trtl_colour_t *pen_colour = trtl_get_pen_colour(turtle);
    if (pen_colour == NULL) {
        return; // No pen colour set
    }

    // set the pen down
    trtl_state_set_pen_down(turtle->current_state, 1);

    // Set heading to 0 degrees
    // TODO: use turtle function after implementing it
    trtl_state_set_heading(turtle->current_state, 45.0);

    // move turtle to the center of the screen
    trtl_forward(turtle, 100.0f);
}

#endif // __TEST_DRAWING_FEATURES_H__
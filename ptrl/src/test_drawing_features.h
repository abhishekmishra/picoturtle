#ifndef __TEST_DRAWING_FEATURES_H__
#define __TEST_DRAWING_FEATURES_H__

#include <raylib.h>
#include "turtle.h"

void test1_move_draw_square(trtl_t *turtle);

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

    test1_move_draw_square(turtle);

    // reset turtle to initial state
    // trtl_reset(turtle);
}

void test1_move_draw_square(trtl_t *turtle)
{
    // pen up
    trtl_pen_up(turtle);

    // go to 100, 100
    trtl_heading(turtle, 0.0);

    trtl_forward(turtle, 100.0f);

    trtl_heading(turtle, 90.0);

    trtl_forward(turtle, 100.0f);

    // set the pen down
    trtl_pen_down(turtle);

    // set pen colour to red
    trtl_colour(turtle, "red");

    // set pen width to 5
    trtl_pen_width(turtle, 5.0f);

    for (int i = 0; i < 4; i++)
    {
        // Draw a square
        trtl_forward(turtle, 100.0f);
        trtl_right(turtle, 90.0f);
    }
}

#endif // __TEST_DRAWING_FEATURES_H__
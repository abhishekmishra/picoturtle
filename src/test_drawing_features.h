#ifndef __TEST_DRAWING_FEATURES_H__
#define __TEST_DRAWING_FEATURES_H__

#include <raylib.h>
#include "turtle.h"

void test1_move_draw_square(trtl_t *turtle);
void test2_move_draw_circle(trtl_t *turtle);

void clear_show_test(trtl_t *turtle);

double _start_time = 0.0;
float seconds_per_test = 0.3f;
int test_number = 0;

// array of function pointers to test functions
void (*test_functions[])(trtl_t *) = {
    test1_move_draw_square,
    test2_move_draw_circle
};

// get the number of test functions
size_t num_test = sizeof(test_functions) / sizeof(test_functions[0]);

void test_graphics_start(trtl_t *turtle)
{
    // set the start time
    _start_time = 0;

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

    clear_show_test(turtle);
}

void clear_show_test(trtl_t *turtle)
{
    if (turtle == NULL || turtle->current_state == NULL) {
        return; // Nothing to clear
    }

    // Clear the canvas
    trtl_colour(turtle, "white");
    trtl_clear_canvas(turtle);

    // Reset the turtle state
    trtl_reset(turtle);

    // Showing test #
    // printf("Showing test %d\n", test_number);

    // Show the next test
    test_functions[test_number](turtle);
}

void test_graphics_update(trtl_t *turtle)
{
    if (turtle == NULL || turtle->current_state == NULL) {
        return; // Nothing to update
    }

    _start_time += trtl_get_delta_time();
    if (_start_time >= seconds_per_test)
    {
        // Reset the start time
        _start_time = 0.0;

        // Increment the test number
        test_number++;
        if (test_number >= num_test)
        {
            test_number = 0; // Reset to the first test
        }

        // printf("Incremented test number to %d\n", test_number);

        // Clear and show the next test
        clear_show_test(turtle);
    }
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

void test2_move_draw_circle(trtl_t *turtle)
{
    // pen up
    trtl_pen_up(turtle);

    // go to 200, 200
    trtl_heading(turtle, 0.0);

    trtl_forward(turtle, 200.0f);

    trtl_heading(turtle, 90.0);

    trtl_forward(turtle, 200.0f);

    // set the pen down
    trtl_pen_down(turtle);

    // set pen colour to blue
    trtl_colour(turtle, "blue");

    // set pen width to 3
    trtl_pen_width(turtle, 3.0f);

    // Draw a circle with radius 50
    for (int i = 0; i < 360; i++)
    {
        float angle = (float)i * DEG2RAD;
        float x = cosf(angle) * 50.0f;
        float y = sinf(angle) * 50.0f;
        trtl_set_position(turtle, x + 200.0f, y + 200.0f);
        trtl_forward(turtle, 1.0f);
    }
}

#endif // __TEST_DRAWING_FEATURES_H__
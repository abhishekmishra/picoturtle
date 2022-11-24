#include <iostream>

#include "PicoTurtle.hpp"

using namespace turtle;

SkiaCanvas * PicoTurtle::turtleCanvas = NULL;
picoturtle_callback PicoTurtle::init_cb = NULL;
void *PicoTurtle::init_cb_args = NULL;
picoturtle_callback PicoTurtle::destroy_cb = NULL;
void *PicoTurtle::destroy_cb_args = NULL;

PicoTurtle::PicoTurtle() : Turtle()
{
    // set the canvas and then call reset
    // so that center is appropriately called based on canvas size.
    if(turtleCanvas == NULL)
    {
        turtleCanvas = new SkiaCanvas();
    }

    this->set_canvas(turtleCanvas);
    this->canvas_size(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT);

    this->reset();

    turtleCanvas->UpdateTurtleBrush(this->get_pen_color(), this->get_pen_width());
    if (init_cb != NULL)
    {
        init_cb(this, init_cb_args);
    }
};

PicoTurtle::~PicoTurtle()
{
    if (destroy_cb != NULL)
    {
        destroy_cb(this, destroy_cb_args);
    }
}

void PicoTurtle::set_init_callback(picoturtle_callback fn, void *cb_args)
{
    init_cb = fn;
    init_cb_args = cb_args;
}

void PicoTurtle::set_destroy_callback(picoturtle_callback fn, void *cb_args)
{
    destroy_cb = fn;
    destroy_cb_args = cb_args;
}

void PicoTurtle::unset_init_callback()
{
    init_cb = NULL;
    init_cb_args = NULL;
}

void PicoTurtle::unset_destroy_callback()
{
    destroy_cb = NULL;
    destroy_cb_args = NULL;
}

sk_sp<SkSurface> PicoTurtle::getRasterSurface()
{
    return turtleCanvas->getRasterSurface();
}

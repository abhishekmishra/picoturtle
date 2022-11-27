#include <iostream>

#include "PicoTurtle.hpp"

using namespace turtle;

// initialize the static members of the class
SkiaCanvas *PicoTurtle::turtleCanvas = NULL;
picoturtle_callback PicoTurtle::init_cb = NULL;
void *PicoTurtle::init_cb_args = NULL;
picoturtle_callback PicoTurtle::update_cb = NULL;
void *PicoTurtle::update_cb_args = NULL;
picoturtle_callback PicoTurtle::paint_cb = NULL;
void *PicoTurtle::paint_cb_args = NULL;
picoturtle_delay PicoTurtle::delay_cb = NULL;
picoturtle_callback PicoTurtle::destroy_cb = NULL;
void *PicoTurtle::destroy_cb_args = NULL;

PicoTurtle::PicoTurtle() : Turtle()
{
    // set the canvas and then call reset
    // so that center is appropriately called based on canvas size.
    if (turtleCanvas == NULL)
    {
        turtleCanvas = new SkiaCanvas();
    }

    this->set_canvas(turtleCanvas);
    this->canvas_size(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT);

    this->reset();

    turtleCanvas->update_turtle_brush(this->get_pen_color(), this->get_pen_width());
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

void PicoTurtle::set_update_callback(picoturtle_callback fn, void *cb_args)
{
    update_cb = fn;
    update_cb_args = cb_args;
}

void PicoTurtle::set_paint_callback(picoturtle_callback fn, void *cb_args)
{
    paint_cb = fn;
    paint_cb_args = cb_args;
}

void PicoTurtle::set_delay_callback(picoturtle_delay fn)
{
    delay_cb = fn;
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

void PicoTurtle::unset_update_callback()
{
    update_cb = NULL;
    update_cb_args = NULL;
}

void PicoTurtle::unset_paint_callback()
{
    paint_cb = NULL;
    paint_cb_args = NULL;
}

void PicoTurtle::unset_delay_callback()
{
    delay_cb = NULL;
}

void PicoTurtle::unset_destroy_callback()
{
    destroy_cb = NULL;
    destroy_cb_args = NULL;
}

void PicoTurtle::delay_ms(int tm)
{
    // std::cout << "PicoTurtle delay_ms called with " << tm;
    if (delay_cb != NULL)
    {
        delay_cb(this, tm);
    }
}

void PicoTurtle::paint()
{
    if (paint_cb)
    {
        paint_cb(this, paint_cb_args);
    }
}

sk_sp<SkSurface> PicoTurtle::getRasterSurface()
{
    return turtleCanvas->getRasterSurface();
}

// void PicoTurtle::penup()
// {
//     Turtle::penup();
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::pendown()
// {
//     Turtle::pendown();
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::penwidth(float w)
// {
//     Turtle::penwidth(w);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::stop()
// {
//     Turtle::stop();
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::home()
// {
//     Turtle::home();
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::forward(float d)
// {
//     Turtle::forward(d);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::back(float d)
// {
//     Turtle::back(d);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::setpos(float x, float y)
// {
//     Turtle::setpos(x, y);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::setx(float x)
// {
//     Turtle::setx(x);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::sety(float y)
// {
//     Turtle::sety(y);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::left(float a)
// {
//     Turtle::left(a);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::right(float a)
// {
//     Turtle::right(a);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::heading(float a)
// {
//     Turtle::heading(a);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::pencolor(unsigned int r, unsigned int g, unsigned int b)
// {
//     Turtle::pencolor(r, g, b);
//     update_cb(this, update_cb_args);
// }

// int PicoTurtle::pencolor(const char *color)
// {
//     int res = Turtle::pencolor(color);
//     update_cb(this, update_cb_args);
//     return res;
// }

// void PicoTurtle::canvas_size(int width, int height)
// {
//     Turtle::canvas_size(width, height);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::reset()
// {
//     Turtle::reset();
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::font(const char *f, unsigned int sz)
// {
//     Turtle::font(f, sz);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::filltext(const char *text)
// {
//     Turtle::filltext(text);
//     update_cb(this, update_cb_args);
// }

// void PicoTurtle::stroketext(const char *text)
// {
//     Turtle::stroketext(text);
//     update_cb(this, update_cb_args);
// }

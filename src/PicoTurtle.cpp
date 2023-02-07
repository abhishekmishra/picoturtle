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
    this->set_canvas_size(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT);

    this->reset();

    turtleCanvas->update_turtle_brush(this->get_pen_color(), this->get_pen_width());
    if (init_cb != NULL)
    {
        init_cb(this, init_cb_args);
    }

    // set update enabled flag to on
    this->enable_update();
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

void PicoTurtle::enable_update()
{
    this->update_enabled = true;
}

void PicoTurtle::disable_update()
{
    this->update_enabled = false;
}

bool PicoTurtle::is_update_enabled()
{
    return this->update_enabled;
}

void PicoTurtle::call_update()
{
    if (is_update_enabled())
    {
        update_cb(this, update_cb_args);
    }
}

sk_sp<SkSurface> PicoTurtle::getRasterSurface()
{
    return turtleCanvas->getRasterSurface();
}

void PicoTurtle::penup()
{
    Turtle::penup();
    call_update();
}

void PicoTurtle::pendown()
{
    Turtle::pendown();
    call_update();
}

void PicoTurtle::penwidth(float w)
{
    Turtle::penwidth(w);
    call_update();
}

void PicoTurtle::stop()
{
    Turtle::stop();
    call_update();
}

void PicoTurtle::home()
{
    Turtle::home();
    call_update();
}

void PicoTurtle::forward(float d)
{
    Turtle::forward(d);
    call_update();
}

void PicoTurtle::back(float d)
{
    Turtle::back(d);
    call_update();
}

void PicoTurtle::setpos(float x, float y)
{
    Turtle::setpos(x, y);
    call_update();
}

void PicoTurtle::setx(float x)
{
    Turtle::setx(x);
    call_update();
}

void PicoTurtle::sety(float y)
{
    Turtle::sety(y);
    call_update();
}

void PicoTurtle::left(float a)
{
    Turtle::left(a);
    call_update();
}

void PicoTurtle::right(float a)
{
    Turtle::right(a);
    call_update();
}

void PicoTurtle::heading(float a)
{
    Turtle::heading(a);
    call_update();
}

void PicoTurtle::pencolor(unsigned int r, unsigned int g, unsigned int b)
{
    Turtle::pencolor(r, g, b);
    call_update();
}

int PicoTurtle::pencolor(const char *color)
{
    int res = Turtle::pencolor(color);
    call_update();
    return res;
}

void PicoTurtle::set_canvas_size(unsigned int width, unsigned int height)
{
    Turtle::set_canvas_size(width, height);
    call_update();
}

void PicoTurtle::reset()
{
    Turtle::reset();
    call_update();
}

void PicoTurtle::font(const char *f, unsigned int sz)
{
    Turtle::font(f, sz);
    call_update();
}

void PicoTurtle::filltext(const char *text)
{
    Turtle::filltext(text);
    call_update();
}

void PicoTurtle::stroketext(const char *text)
{
    Turtle::stroketext(text);
    call_update();
}

void PicoTurtle::circle(float radius)
{
    Turtle::circle(radius);
    call_update();
}

void PicoTurtle::arc(float radius, float extent, int steps)
{
    Turtle::arc(radius, extent, steps);
    call_update();
}

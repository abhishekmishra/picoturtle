#include "Turtle.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "color_names.h"
#include <algorithm>

//TODO: fix all the memory leaks in this class by changing allocation to use
// smart pointers.

using namespace turtle;

Turtle::Turtle() : Turtle(new TurtleOptions())
{
}

Turtle::Turtle(TurtleOptions *options)
{
    options = options;
    name = "PicoTurtle";
    id = "";

    current_state = new TurtleState();
    saved_states = new std::stack<TurtleState *>();

    current_state->get_pen_color()->set_a(255);
    current_state->get_pen_color()->set_r(128);
    current_state->get_pen_color()->set_g(64);
    current_state->get_pen_color()->set_b(32);

    // set the start time of the turtle
    start_tm = std::chrono::steady_clock::now();
};

std::string Turtle::get_name()
{
    return name;
}

std::string Turtle::get_id()
{
    return id;
}

long long Turtle::elapsed_time_ms()
{
    auto current_tm = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_tm - start_tm).count();
    return elapsed_ms;
}

void Turtle::delay_ms(int tm)
{
    // do nothing.
}

void Turtle::paint()
{
    // do nothing.
}

TurtleState *Turtle::get_current_state() const
{
    TurtleState *detached = new TurtleState();
    detached->copyFromTurtleState(current_state);
    return detached;
}

TurtleState *Turtle::get_saved_state() const
{
    TurtleState *detached = new TurtleState();
        if(saved_states->empty())
    {
        detached->copyFromTurtleState(current_state);
    } 
    else 
    {
        TurtleState *saved_state = saved_states->top();
        detached->copyFromTurtleState(saved_state);
    }

    return detached;
}

void Turtle::save()
{
    TurtleState *saved_state = new TurtleState();
    saved_state->copyFromTurtleState(current_state);
    saved_states->push(saved_state);
}

void Turtle::restore()
{
    if(!saved_states->empty())
    {
        TurtleState *saved_state = saved_states->top();
        saved_states->pop();
        current_state->copyFromTurtleState(saved_state);
        canvas->update_turtle_brush(get_pen_color(), get_pen_width());
        delete saved_state;
    }
}

float Turtle::get_canvas_location_x()
{
    return current_state->get_location()->getX();
}

float Turtle::get_canvas_location_y()
{
    return this->canvas->get_height() - current_state->get_location()->getY();
}

float Turtle::canvas_heading()
{
    return 360.0 - current_state->get_heading();
}

float Turtle::get_heading()
{
    return current_state->get_heading();
}

float Turtle::get_pen_width()
{
    return current_state->get_pen_width();
}

void Turtle::draw_me()
{
    int d = 25;
    float theta1 = (canvas_heading() - 145) * (M_PI / 180);
    float y2 = d * (sin(theta1)) + get_canvas_location_y();
    float x2 = d * (cos(theta1)) + get_canvas_location_x();
    float theta2 = (canvas_heading() + 145) * (M_PI / 180);
    float y3 = d * (sin(theta2)) + get_canvas_location_y();
    float x3 = d * (cos(theta2)) + get_canvas_location_x();

    // store current colour and pen width
    // to reset later
    // TODO: review how to add alpha support to brush.
    // short ca = current_state->get_pen_color()->get_a();
    short cr = current_state->get_pen_color()->get_r();
    short cg = current_state->get_pen_color()->get_g();
    short cb = current_state->get_pen_color()->get_b();
    std::string cname = current_state->get_pen_color()->get_color_name();

    float cpw = get_pen_width();

    // set turtle colour
    pencolor(255u, 0u, 0u);
    penwidth(2);

    canvas->draw_triangle(get_canvas_location_x(), get_canvas_location_y(), x2, y2, x3, y3);

    // reset turtle colour and pen width
    if (cname == "unknown")
    {
        pencolor(cr, cg, cb);
    }
    else
    {
        pencolor(cname.c_str());
    }
    penwidth(cpw);
}

void Turtle::penup()
{
    current_state->set_pen_down(false);
}

void Turtle::pendown()
{
    current_state->set_pen_down(true);
}

void Turtle::penwidth(float w)
{
    current_state->set_pen_width(w);
    canvas->update_turtle_brush(get_pen_color(), get_pen_width());
}

void Turtle::stop()
{
    // does nothing for now
    // draw_me();
}

void Turtle::home()
{
    current_state->get_location()->setX(this->canvas->get_width() / 2.0f);
    current_state->get_location()->setY(this->canvas->get_height() / 2.0f);
}

void Turtle::forward(float d)
{
    float theta = current_state->get_heading() * (M_PI / 180);
    float canvas_theta = canvas_heading() * (M_PI / 180);
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    float y2 = d * (sin(theta)) + current_state->get_location()->getY();
    float x2 = d * (cos(theta)) + current_state->get_location()->getX();
    float cy2 = d * (sin(canvas_theta)) + get_canvas_location_y();
    float cx2 = d * (cos(canvas_theta)) + get_canvas_location_x();

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    if (current_state->is_pen_down())
    {
        canvas->draw_line(get_canvas_location_x(), get_canvas_location_y(), cx2, cy2);
    }

    current_state->get_location()->setX(x2);
    current_state->get_location()->setY(y2);
}

void Turtle::back(float d)
{
    forward(-d);
}

void Turtle::setpos(float x, float y)
{
    if (current_state->is_pen_down())
    {
        canvas->draw_line(get_canvas_location_x(), get_canvas_location_y(), x, this->canvas->get_height() - y);
    }

    current_state->get_location()->setX(x);
    current_state->get_location()->setY(y);
}

float Turtle::getx()
{
    return current_state->get_location()->getX();
}

float Turtle::gety()
{
    return current_state->get_location()->getY();
}

void Turtle::setx(float x)
{
    return setpos(x, current_state->get_location()->getY());
}

void Turtle::sety(float y)
{
    return setpos(current_state->get_location()->getX(), y);
}

void Turtle::left(float a)
{
    current_state->set_heading(current_state->get_heading() + a);
}

void Turtle::right(float a)
{
    current_state->set_heading(current_state->get_heading() - a);
}

void Turtle::heading(float a)
{
    current_state->set_heading(a);
}

void Turtle::pencolor(unsigned int r, unsigned int g, unsigned int b)
{
    current_state->get_pen_color()->set_r(r);
    current_state->get_pen_color()->set_g(g);
    current_state->get_pen_color()->set_b(b);

    // TODO: lookup color in color names list and set appropriately.
    current_state->get_pen_color()->set_color_name("unknown");
    canvas->update_turtle_brush(get_pen_color(), get_pen_width());
}

int Turtle::pencolor(const char *color)
{
    int res = current_state->get_pen_color()->update_color_by_name(color);
    if (res == 1)
    {
        canvas->update_turtle_brush(get_pen_color(), get_pen_width());
    }
    return res;
}

void Turtle::set_canvas_size(unsigned int width, unsigned int height)
{
    // printf("Dimensions are now [%d, %d].\n", width, height);
    this->canvas->set_width(width);
    this->canvas->set_height(height);
    canvas->update_canvas();
}

unsigned int Turtle::get_canvas_width()
{
    return this->canvas->get_width();
}

unsigned int Turtle::get_canvas_height()
{
    return this->canvas->get_height();
}

void Turtle::clear(unsigned int r, unsigned int g, unsigned int b)
{
    save();

    pencolor(r, g, b);
    penup();
    setpos(0, get_canvas_height() / 2);
    heading(0);
    pendown();
    penwidth(get_canvas_height());
    forward(get_canvas_width());

    restore();
}

void Turtle::clear(const char *color)
{
    if (color_name_exists(color) == 1)
    {
        save();

        pencolor(color);
        penup();
        setpos(0, get_canvas_height() / 2);
        heading(0);
        pendown();
        penwidth(get_canvas_height());
        forward(get_canvas_width());

        restore();
    }
}

void Turtle::clear()
{
    clear("white");
}

void Turtle::reset()
{
    home();
    pendown();
    penwidth(TURTLE_DEFAULT_PENWIDTH);
    heading(TURTLE_DEFAULT_HEADING);
    canvas->clear();
}

TurtleColor *Turtle::get_pen_color()
{
    return current_state->get_pen_color();
}

Canvas *Turtle::get_canvas()
{
    return canvas;
}

void Turtle::font(const char *f, unsigned int sz)
{
    this->canvas->font(f, sz);
}

void Turtle::filltext(const char *text)
{
    this->canvas->filltext(get_canvas_location_x(), get_canvas_location_y(), current_state->get_heading(), text);
}

void Turtle::stroketext(const char *text)
{
    this->canvas->stroketext(get_canvas_location_x(), get_canvas_location_y(), current_state->get_heading(), text);
}

void Turtle::set_canvas(Canvas *c)
{
    this->canvas = c;
}

void Turtle::circle(float radius)
{
    this->canvas->draw_circle(get_canvas_location_x(), get_canvas_location_y(), radius);
}

// This implementation is largely based on the Turtle:circle method
// in the Python turtle module
void Turtle::arc(float radius, float extent, int steps)
{
    if (extent < 0)
    {
        extent = 360.0;
    }
    if (steps <= 0)
    {
        float frac = abs(extent) / 360.0;
        steps = 1 + (std::min(11 + abs(radius) / 6.0, 59.0) * frac);
    }

    // printf("radius, extent, steps -> %f, %f, %d\n", radius, extent, steps);

    float w = 1.0 * extent / steps;
    float w2 = 0.5 * w;
    float l = 2.0 * radius * sin(w2 * (M_PI / 180.0));
    if (radius < 0)
    {
        l = -l;
        w = -w;
        w2 = -w2;
    }

    left(w2);

    for (int i = 0; i < steps; i++)
    {
        forward(l);
        right(w);
    }

    right(w2);
}

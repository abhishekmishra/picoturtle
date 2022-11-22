#include "Turtle.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

using namespace turtle;

Turtle::Turtle() : Turtle(new TurtleOptions())
{
}

Turtle::Turtle(TurtleOptions *options)
{
    options = options;
    name = "PicoTurtle";
    id = "";

    turtle_state = new TurtleState();
    turtle_state->get_pen_color()->setA(255);
    turtle_state->get_pen_color()->setR(128);
    turtle_state->get_pen_color()->setG(64);
    turtle_state->get_pen_color()->setB(32);
};

std::string Turtle::get_name() {
    return name;
}

std::string Turtle::get_id() {
    return id;
}

float Turtle::get_canvas_location_x()
{
    return turtle_state->get_location()->getX();
}

float Turtle::get_canvas_location_y()
{
    return this->canvas->getHeight() - turtle_state->get_location()->getY();
}

float Turtle::get_x()
{
    return turtle_state->get_location()->getX();
}

float Turtle::get_y()
{
    return turtle_state->get_location()->getY();
}

float Turtle::canvas_heading()
{
    return 360.0 - turtle_state->get_heading();
}

float Turtle::get_heading()
{
    return turtle_state->get_heading();
}

float Turtle::get_pen_width()
{
    return turtle_state->get_pen_width();
}

void Turtle::DrawTurtle()
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
    float ca = turtle_state->get_pen_color()->getA();
    float cr = turtle_state->get_pen_color()->getR();
    float cg = turtle_state->get_pen_color()->getG();
    float cb = turtle_state->get_pen_color()->getB();

    float cpw = get_pen_width();

    // set turtle colour
    pencolor(255u, 0u, 0u);
    penwidth(2);

    canvas->DrawTriangle(get_canvas_location_x(), get_canvas_location_y(), x2, y2, x3, y3);

    // reset turtle colour and pen width
    pencolor(cr, cg, cb);
    penwidth(cpw);
}

void Turtle::penup()
{
    turtle_state->set_pen_down(false);
}

void Turtle::pendown()
{
    turtle_state->set_pen_down(true);
}

void Turtle::penwidth(float w)
{
    turtle_state->set_pen_width(w);
    canvas->UpdateTurtleBrush(get_pen_color(), get_pen_width());
}

void Turtle::stop()
{
    // does nothing for now
    DrawTurtle();
}

void Turtle::home()
{
    turtle_state->get_location()->setX(this->canvas->getWidth() / 2.0f);
    turtle_state->get_location()->setY(this->canvas->getHeight() / 2.0f);
}

void Turtle::forward(float d)
{
    float theta = turtle_state->get_heading() * (M_PI / 180);
    float canvas_theta = canvas_heading() * (M_PI / 180);
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    float y2 = d * (sin(theta)) + turtle_state->get_location()->getY();
    float x2 = d * (cos(theta)) + turtle_state->get_location()->getX();
    float cy2 = d * (sin(canvas_theta)) + get_canvas_location_y();
    float cx2 = d * (cos(canvas_theta)) + get_canvas_location_x();

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    if (turtle_state->is_pen_down())
    {
        canvas->DrawLine(get_canvas_location_x(), get_canvas_location_y(), cx2, cy2);
    }

    turtle_state->get_location()->setX(x2);
    turtle_state->get_location()->setY(y2);
}

void Turtle::back(float d)
{
    forward(-d);
}

void Turtle::setpos(float x, float y)
{
    if (turtle_state->is_pen_down())
    {
        canvas->DrawLine(get_canvas_location_x(), get_canvas_location_y(), x, this->canvas->getHeight() - y);
    }

    turtle_state->get_location()->setX(x);
    turtle_state->get_location()->setY(y);
}

float Turtle::getx()
{
    return turtle_state->get_location()->getX();
}

float Turtle::gety()
{
    return turtle_state->get_location()->getY();
}

void Turtle::setx(float x)
{
    return setpos(x, turtle_state->get_location()->getY());
}

void Turtle::sety(float y)
{
    return setpos(turtle_state->get_location()->getX(), y);
}

void Turtle::left(float a)
{
    turtle_state->set_heading(turtle_state->get_heading() + a);
}

void Turtle::right(float a)
{
    turtle_state->set_heading(turtle_state->get_heading() - a);
}

void Turtle::heading(float a)
{
    turtle_state->set_heading(a);
}

void Turtle::pencolor(unsigned int r, unsigned int g, unsigned int b)
{
    turtle_state->get_pen_color()->setR(r);
    turtle_state->get_pen_color()->setG(g);
    turtle_state->get_pen_color()->setB(b);
    canvas->UpdateTurtleBrush(get_pen_color(), get_pen_width());
}

int Turtle::pencolor(const char* color)
{
    TurtleColor *c;
    int res = TurtleColor::get_color_by_name(&c, color);
    if (res == 1)
    {
        pencolor(c->getR(), c->getG(), c->getB());
        delete c;
    }
    return res;
}

void Turtle::canvas_size(int width, int height)
{
    // printf("Dimensions are now [%d, %d].\n", width, height);
    this->canvas->setWidth(width);
    this->canvas->setHeight(height);
    canvas->UpdateCanvas();
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
    return turtle_state->get_pen_color();
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
    this->canvas->filltext(get_canvas_location_x(), get_canvas_location_y(), turtle_state->get_heading(), text);
}

void Turtle::stroketext(const char *text)
{
    this->canvas->stroketext(get_canvas_location_x(), get_canvas_location_y(), turtle_state->get_heading(), text);
}

void Turtle::set_canvas(Canvas* c)
{
    this->canvas = c;
}

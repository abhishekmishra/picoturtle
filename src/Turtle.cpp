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
    Options = options;
    Name = "PicoTurtle";
    Id = "";

    turtle_state = new TurtleState();
    turtle_state->get_pen_color()->setA(255);
    turtle_state->get_pen_color()->setR(128);
    turtle_state->get_pen_color()->setG(64);
    turtle_state->get_pen_color()->setB(32);
};

std::string Turtle::getName() {
    return Name;
}

std::string Turtle::getId() {
    return Id;
}

float Turtle::getCanvasLocationX()
{
    return turtle_state->get_location()->getX();
}

float Turtle::getCanvasLocationY()
{
    return this->canvas->getHeight() - turtle_state->get_location()->getY();
}

float Turtle::getX()
{
    return turtle_state->get_location()->getX();
}

float Turtle::getY()
{
    return turtle_state->get_location()->getY();
}

float Turtle::CanvasAngle()
{
    return 360.0 - turtle_state->get_heading();
}

float Turtle::getHeading()
{
    return turtle_state->get_heading();
}

float Turtle::getPenWidth()
{
    return turtle_state->get_pen_width();
}

void Turtle::DrawTurtle()
{
    int d = 25;
    float theta1 = (CanvasAngle() - 145) * (M_PI / 180);
    float y2 = d * (sin(theta1)) + getCanvasLocationY();
    float x2 = d * (cos(theta1)) + getCanvasLocationX();
    float theta2 = (CanvasAngle() + 145) * (M_PI / 180);
    float y3 = d * (sin(theta2)) + getCanvasLocationY();
    float x3 = d * (cos(theta2)) + getCanvasLocationX();

    // store current colour and pen width
    // to reset later
    float ca = turtle_state->get_pen_color()->getA();
    float cr = turtle_state->get_pen_color()->getR();
    float cg = turtle_state->get_pen_color()->getG();
    float cb = turtle_state->get_pen_color()->getB();

    float cpw = getPenWidth();

    // set turtle colour
    pencolor(255u, 0u, 0u);
    penwidth(2);

    canvas->DrawTriangle(getCanvasLocationX(), getCanvasLocationY(), x2, y2, x3, y3);

    // reset turtle colour and pen width
    pencolor(cr, cg, cb);
    penwidth(cpw);
}

// TurtleState *Turtle::CurrentState()
// {
//     TurtleState *state = new TurtleState();
//     // state->colour = Colour;
//     return state;
// };

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
    canvas->UpdateTurtleBrush(getPenColor(), getPenWidth());
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
    float canvas_theta = CanvasAngle() * (M_PI / 180);
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    float y2 = d * (sin(theta)) + turtle_state->get_location()->getY();
    float x2 = d * (cos(theta)) + turtle_state->get_location()->getX();
    float cy2 = d * (sin(canvas_theta)) + getCanvasLocationY();
    float cx2 = d * (cos(canvas_theta)) + getCanvasLocationX();

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, turtle_state->get_location()->getX(), turtle_state->get_location()->getY(), x2, y2);
    if (turtle_state->is_pen_down())
    {
        canvas->DrawLine(getCanvasLocationX(), getCanvasLocationY(), cx2, cy2);
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
        canvas->DrawLine(getCanvasLocationX(), getCanvasLocationY(), x, this->canvas->getHeight() - y);
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
    canvas->UpdateTurtleBrush(getPenColor(), getPenWidth());
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

TurtleColor *Turtle::getPenColor()
{
    return turtle_state->get_pen_color();
}

Canvas *Turtle::getCanvas()
{
    return canvas;
}

void Turtle::font(const char *f, unsigned int sz)
{
    this->canvas->font(f, sz);
}


void Turtle::filltext(const char *text)
{
    this->canvas->filltext(getCanvasLocationX(), getCanvasLocationY(), turtle_state->get_heading(), text);
}

void Turtle::stroketext(const char *text)
{
    this->canvas->stroketext(getCanvasLocationX(), getCanvasLocationY(), turtle_state->get_heading(), text);
}

void Turtle::setCanvas(Canvas* c)
{
    this->canvas = c;
}

#include "Turtle.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

using namespace turtle;

picoturtle_callback Turtle::init_cb = NULL;
void* Turtle::init_cb_args = NULL;

Turtle::Turtle() : Turtle(new TurtleOptions())
{
}

Turtle::Turtle(TurtleOptions *options)
{
    Options = options;
    Name = "Hello";
    Id = "";
    Width = TURTLE_DEFAULT_CANVAS_WIDTH;
    Height = TURTLE_DEFAULT_CANVAS_HEIGHT;
    Dimension = new TurtleDimension(Width, Height);
    Location = new TurtleLocation((int)(Width / 2), (int)(Height / 2));
    Angle = TURTLE_DEFAULT_HEADING;
    PenColor = new TurtleColor(128, 128, 0, 255);
    PenWidth = TURTLE_DEFAULT_PENWIDTH;

    if (init_cb != NULL)
    {
        init_cb(this, init_cb_args);
    }
};

std::string Turtle::getName() {
    return Name;
}

std::string Turtle::getId() {
    return Id;
}


int Turtle::getWidth()
{
    return Width;
}
void Turtle::setWidth(int w)
{
    Width = w;
}
int Turtle::getHeight()
{
    return Height;
}
void Turtle::setHeight(int h)
{
    Height = h;
}

float Turtle::getCanvasLocationX()
{
    return Location->getX();
}

float Turtle::getCanvasLocationY()
{
    return getHeight() - Location->getY();
}

float Turtle::getX()
{
    return Location->getX();
}

float Turtle::getY()
{
    return Location->getY();
}

float Turtle::CanvasAngle()
{
    return 360.0 - Angle;
}

float Turtle::getHeading()
{
    return Angle;
}

float Turtle::getPenWidth()
{
    return PenWidth;
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
    float ca = PenColor->getA();
    float cr = PenColor->getR();
    float cg = PenColor->getG();
    float cb = PenColor->getB();

    float cpw = getPenWidth();

    // set turtle colour
    pencolour(255u, 0u, 0u);
    penwidth(2);

    DrawTriangle(getCanvasLocationX(), getCanvasLocationY(), x2, y2, x3, y3);

    // reset turtle colour and pen width
    pencolour(cr, cg, cb);
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
    IsPenDown = false;
}

void Turtle::pendown()
{
    IsPenDown = true;
}

void Turtle::penwidth(float w)
{
    PenWidth = w;
    UpdateTurtleBrush();
}

void Turtle::stop()
{
    // does nothing for now
    DrawTurtle();
}

void Turtle::home()
{
    Location->setX(Width / 2.0f);
    Location->setY(Height / 2.0f);
}

void Turtle::forward(float d)
{
    float theta = Angle * (M_PI / 180);
    float canvas_theta = CanvasAngle() * (M_PI / 180);
    // y2 = d sin (theta) + y1
    // x2 = d cos (theta) + x1
    float y2 = d * (sin(theta)) + Location->getY();
    float x2 = d * (cos(theta)) + Location->getX();
    float cy2 = d * (sin(canvas_theta)) + getCanvasLocationY();
    float cx2 = d * (cos(canvas_theta)) + getCanvasLocationX();

    // printf("angle %f, from [%f, %f], to [%f, %f]\n", theta, Location->getX(), Location->getY(), x2, y2);
    if (IsPenDown)
    {
        DrawLine(getCanvasLocationX(), getCanvasLocationY(), cx2, cy2);
    }

    Location->setX(x2);
    Location->setY(y2);
}

void Turtle::back(float d)
{
    forward(-d);
}

void Turtle::setpos(float x, float y)
{
    if (IsPenDown)
    {
        DrawLine(getCanvasLocationX(), getCanvasLocationY(), x, Height - y);
    }

    Location->setX(x);
    Location->setY(y);
}

float Turtle::getx()
{
    return Location->getX();
}

float Turtle::gety()
{
    return Location->getY();
}

void Turtle::setx(float x)
{
    return setpos(x, Location->getY());
}

void Turtle::sety(float y)
{
    return setpos(Location->getX(), y);
}

void Turtle::left(float a)
{
    Angle = Angle + a;
}

void Turtle::right(float a)
{
    Angle = Angle - a;
}

void Turtle::heading(float a)
{
    Angle = a;
}

void Turtle::pencolour(unsigned int r, unsigned int g, unsigned int b)
{
    PenColor->setR(r);
    PenColor->setG(g);
    PenColor->setB(b);
    UpdateTurtleBrush();
}

void Turtle::canvas_size(int width, int height)
{
    // printf("Dimensions are now [%d, %d].\n", width, height);
    setWidth(width);
    setHeight(height);
    UpdateCanvas();
}

void Turtle::reset()
{
    home();
    pendown();
    penwidth(TURTLE_DEFAULT_PENWIDTH);
    heading(TURTLE_DEFAULT_HEADING);
    clear();
}

TurtleColor *Turtle::getPenColor()
{
    return PenColor;
}

void Turtle::set_init_callback(picoturtle_callback fn, void* cb_args)
{
    init_cb = fn;
    init_cb_args = cb_args;
}

void Turtle::unset_init_callback()
{
    init_cb = NULL;
    init_cb_args = NULL;
}

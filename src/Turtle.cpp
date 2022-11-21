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

    Location = new TurtleLocation(0, 0);
    //Location = new TurtleLocation((int)(canvas->getWidth() / 2), (int)(canvas->getHeight() / 2));
    Angle = TURTLE_DEFAULT_HEADING;

    // TODO: change turtle default colour
    PenColor = new TurtleColor(128, 64, 32, 255);

    PenWidth = TURTLE_DEFAULT_PENWIDTH;
};

std::string Turtle::getName() {
    return Name;
}

std::string Turtle::getId() {
    return Id;
}

float Turtle::getCanvasLocationX()
{
    return Location->getX();
}

float Turtle::getCanvasLocationY()
{
    return this->canvas->getHeight() - Location->getY();
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
    IsPenDown = false;
}

void Turtle::pendown()
{
    IsPenDown = true;
}

void Turtle::penwidth(float w)
{
    PenWidth = w;
    canvas->UpdateTurtleBrush(getPenColor(), getPenWidth());
}

void Turtle::stop()
{
    // does nothing for now
    DrawTurtle();
}

void Turtle::home()
{
    Location->setX(this->canvas->getWidth() / 2.0f);
    Location->setY(this->canvas->getHeight() / 2.0f);
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
        canvas->DrawLine(getCanvasLocationX(), getCanvasLocationY(), cx2, cy2);
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
        canvas->DrawLine(getCanvasLocationX(), getCanvasLocationY(), x, this->canvas->getHeight() - y);
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

void Turtle::pencolor(unsigned int r, unsigned int g, unsigned int b)
{
    PenColor->setR(r);
    PenColor->setG(g);
    PenColor->setB(b);
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
    return PenColor;
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
    this->canvas->filltext(getCanvasLocationX(), getCanvasLocationY(), Angle, text);
}

void Turtle::stroketext(const char *text)
{
    this->canvas->stroketext(getCanvasLocationX(), getCanvasLocationY(), Angle, text);
}

void Turtle::setCanvas(Canvas* c)
{
    this->canvas = c;
}

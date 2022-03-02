#include "TurtleColor.hpp"

turtle::TurtleColor::TurtleColor()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

turtle::TurtleColor::TurtleColor(unsigned int rv, unsigned int gv, unsigned int bv, unsigned int av)
{
    setR(rv);
    setG(gv);
    setB(bv);
    setA(av);
}

turtle::TurtleColor::~TurtleColor() {}

unsigned int turtle::TurtleColor::getR()
{
    return r;
}
unsigned int turtle::TurtleColor::getG()
{
    return g;
}
unsigned int turtle::TurtleColor::getB()
{
    return b;
}
unsigned int turtle::TurtleColor::getA()
{
    return a;
}
void turtle::TurtleColor::setR(unsigned int rv)
{
    if (rv > 255)
    {
        r = 255;
    }
    else
    {
        r = rv;
    }
}
void turtle::TurtleColor::setG(unsigned int gv)
{
    if (gv > 255)
    {
        g = 255;
    }
    else
    {
        g = gv;
    }
}
void turtle::TurtleColor::setB(unsigned int bv)
{
    if (bv > 255)
    {
        b = 255;
    }
    else
    {
        b = bv;
    }
}
void turtle::TurtleColor::setA(unsigned int av)
{
    if (av > 255)
    {
        a = 255;
    }
    else
    {
        a = av;
    }
}

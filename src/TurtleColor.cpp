#include "TurtleColor.hpp"
#include "color_names.h"
#include <cstdio>

turtle::TurtleColor::TurtleColor()
{
    r = 0;
    g = 0;
    b = 0;
    a = 255;
}

int turtle::TurtleColor::get_color_by_name(TurtleColor** c, const char* color_name)
{
    uint8_t cr, cg, cb;
    int res = color_name_get_rgb(color_name, &cr, &cg, &cb);
    if (res == 1)
    {
        (*c) = new TurtleColor(cr, cg, cb, 255);
    }
    return res;
}

turtle::TurtleColor::TurtleColor(uint8_t rv, uint8_t gv, uint8_t bv, uint8_t av)
{
    setR(rv);
    setG(gv);
    setB(bv);
    setA(av);
}

turtle::TurtleColor::~TurtleColor() {}

uint8_t turtle::TurtleColor::getR()
{
    return r;
}
uint8_t turtle::TurtleColor::getG()
{
    return g;
}
uint8_t turtle::TurtleColor::getB()
{
    return b;
}
uint8_t turtle::TurtleColor::getA()
{
    return a;
}
void turtle::TurtleColor::setR(uint8_t rv)
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
void turtle::TurtleColor::setG(uint8_t gv)
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
void turtle::TurtleColor::setB(uint8_t bv)
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
void turtle::TurtleColor::setA(uint8_t av)
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

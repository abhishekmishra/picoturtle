#include "TurtleColor.hpp"
#include "color_names.h"
#include <cstdio>

using namespace turtle;

const std::string TurtleColor::DEFAULT_COLOR_NAME = "black";

void TurtleColor::init_default()
{
    name = DEFAULT_COLOR_NAME;
    r = 0;
    g = 0;
    b = 0;
    a = 255;
}

TurtleColor::TurtleColor()
{
    init_default();
}

TurtleColor::TurtleColor(std::string name)
{
    uint8_t cr, cg, cb;
    int res = color_name_get_rgb(name.c_str(), &cr, &cg, &cb);
    if (res == 1)
    {
        this->name = name;
        this->r = cr;
        this->g = cg;
        this->b = cb;
        this->a = 255;
    }
    else
    {
        init_default();
    }
}

int TurtleColor::update_color_by_name(std::string color_name)
{
    uint8_t cr, cg, cb;
    int res = color_name_get_rgb(color_name.c_str(), &cr, &cg, &cb);
    if (res == 1)
    {
        this->name = color_name;
        this->r = cr;
        this->g = cg;
        this->b = cb;
    }
    return res;
}

TurtleColor::TurtleColor(uint8_t rv, uint8_t gv, uint8_t bv, uint8_t av)
{
    set_r(rv);
    set_g(gv);
    set_b(bv);
    set_a(av);
}

TurtleColor::~TurtleColor() {}

uint8_t TurtleColor::get_r()
{
    return r;
}
uint8_t TurtleColor::get_g()
{
    return g;
}
uint8_t TurtleColor::get_b()
{
    return b;
}
uint8_t TurtleColor::get_a()
{
    return a;
}
void TurtleColor::set_r(uint8_t rv)
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
void TurtleColor::set_g(uint8_t gv)
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
void TurtleColor::set_b(uint8_t bv)
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
void TurtleColor::set_a(uint8_t av)
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

std::string TurtleColor::get_color_name()
{
    return name;
}

void TurtleColor::set_color_name(std::string color)
{
    name = color;
}
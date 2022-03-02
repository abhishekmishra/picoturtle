#include "TurtleDimension.hpp"

turtle::TurtleDimension::TurtleDimension()
{
    width = 0.0f;
    height = 0.0f;
}

turtle::TurtleDimension::TurtleDimension(double w, double h)
{
    width = w;
    height = h;
}

turtle::TurtleDimension::~TurtleDimension() {}

float turtle::TurtleDimension::getWidth()
{
    return width;
}

float turtle::TurtleDimension::getHeight()
{
    return height;
}
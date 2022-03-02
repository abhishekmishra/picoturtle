#include "TurtleLocation.hpp"

turtle::TurtleLocation::TurtleLocation()
{
    x = 0.0f;
    y = 0.0f;
}

turtle::TurtleLocation::TurtleLocation(float xv, float yv)
{
    x = xv;
    y = yv;
}

turtle::TurtleLocation::~TurtleLocation() {}

float turtle::TurtleLocation::getX()
{
    return x;
}

float turtle::TurtleLocation::getY()
{
    return y;
}

void turtle::TurtleLocation::setX(float xv)
{
    x = xv;
}

void turtle::TurtleLocation::setY(float yv)
{
    y = yv;
}
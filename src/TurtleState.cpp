#include "TurtleState.hpp"

using namespace turtle;

TurtleState::TurtleState()
{
    location = new TurtleLocation();
    heading = TURTLE_DEFAULT_HEADING;

    pen_color = new TurtleColor();
    pen_down = false;
    pen_width = TURTLE_DEFAULT_PENWIDTH;

    font_str = "";
}

TurtleState::~TurtleState()
{
    delete location;
    delete pen_color;
}

void TurtleState::copyFromTurtleState(TurtleState *other)
{
    if (other != NULL)
    {
        location->setX(other->get_location()->getX());
        location->setY(other->get_location()->getY());
        heading = other->get_heading();

        pen_color->setA(other->get_pen_color()->getA());
        pen_color->setR(other->get_pen_color()->getR());
        pen_color->setG(other->get_pen_color()->getG());
        pen_color->setB(other->get_pen_color()->getB());
        pen_down = other->is_pen_down();
        pen_width = other->get_pen_width();

        font_str = std::string(other->get_font_string());
    }
}

TurtleLocation *TurtleState::get_location() const
{
    return location;
}

double TurtleState::get_heading() const
{
    return heading;
}

void TurtleState::set_heading(double heading)
{
    this->heading = heading;
}

TurtleColor *TurtleState::get_pen_color() const
{
    return pen_color;
}

bool TurtleState::is_pen_down() const
{
    return pen_down;
}

void TurtleState::set_pen_down(bool flag)
{
    this->pen_down = flag;
}

float TurtleState::get_pen_width() const
{
    return pen_width;
}

void TurtleState::set_pen_width(float pen_width)
{
    this->pen_width = pen_width;
}

std::string TurtleState::get_font_string() const
{
    return font_str;
}

void TurtleState::set_font_string(std::string font_string)
{
    this->font_str = font_string;
}

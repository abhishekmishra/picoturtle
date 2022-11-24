#include "Canvas.hpp"

using namespace turtle;

Canvas::Canvas() : Canvas(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT)
{}

Canvas::Canvas(int width, int height)
{
    this->width = width;
    this->height = height;
    Dimension = new TurtleDimension(width, height);
};

Canvas::~Canvas(){};

int Canvas::get_width()
{
    return width;
}
void Canvas::set_width(int w)
{
    width = w;
}
int Canvas::get_height()
{
    return height;
}
void Canvas::set_height(int h)
{
    height = h;
}

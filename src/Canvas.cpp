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

int Canvas::getWidth()
{
    return width;
}
void Canvas::setWidth(int w)
{
    width = w;
}
int Canvas::getHeight()
{
    return height;
}
void Canvas::setHeight(int h)
{
    height = h;
}

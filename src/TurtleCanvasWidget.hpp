#ifndef TURTLE_CANVAS_WIDGET_H
#define TURTLE_CANVAS_WIDGET_H

#include <QWidget>
#include "PicoTurtle.hpp"
#include "TurtleGLWidget.hpp"
// #include "TurtleSDLWidget.hpp"

class TurtleCanvasWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleCanvasWidget(QWidget *parent = nullptr);
    ~TurtleCanvasWidget();
    void set_turtle(turtle::PicoTurtle* t);

private:
    TurtleGLWidget* gl_widget;
    // TurtleSDLWidget* sdl_widget;
};
#endif // TURTLE_CANVAS_WIDGET_H

#ifndef TURTLE_CANVAS_WIDGET_H
#define TURTLE_CANVAS_WIDGET_H

#include <QWidget>
#include "TurtleSDLWidget.hpp"

class TurtleCanvasWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleCanvasWidget(QWidget *parent = nullptr);
    ~TurtleCanvasWidget();

private:
    TurtleSDLWidget* sdl_widget;
};
#endif // TURTLE_CANVAS_WIDGET_H

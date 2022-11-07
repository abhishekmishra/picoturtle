#ifndef TURTLE_CANVAS_WIDGET_H
#define TURTLE_CANVAS_WIDGET_H

#include <QWidget>

class TurtleCanvasWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleCanvasWidget(QWidget *parent = nullptr);
    ~TurtleCanvasWidget();

};
#endif // TURTLE_CANVAS_WIDGET_H

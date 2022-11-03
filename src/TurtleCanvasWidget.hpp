#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QScrollArea>

class TurtleCanvasWidget : public QScrollArea
{
    Q_OBJECT

public:
    TurtleCanvasWidget(QWidget *parent = nullptr);
    ~TurtleCanvasWidget();
};
#endif // WIDGET_H

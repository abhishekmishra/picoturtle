#pragma once
#ifndef TURTLE_GL_WIDGET_H
#define TURTLE_GL_WIDGET_H

#include <QOpenGLWidget>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPaintEvent>

#include "PicoTurtle.hpp"

class TurtleGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    TurtleGLWidget(QWidget *parent = nullptr);
    void set_turtle(turtle::PicoTurtle* t);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int elapsed;
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    turtle::PicoTurtle *turtle;
};

#endif
#pragma once
#ifndef TURTLE_GL_WIDGET_H
#define TURTLE_GL_WIDGET_H

#include <QOpenGLWidget>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPaintEvent>
#include <QResizeEvent>

#include "PicoTurtle.hpp"

class TurtleGLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    static const int GLWIDGET_WIDTH = 400;
    static const int GLWIDGET_HEIGHT = 400;

public:
    TurtleGLWidget(QWidget *parent = nullptr);
    void set_turtle(turtle::PicoTurtle* t);

public slots:
    void animate();
    void draw_turtle();

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

public:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif

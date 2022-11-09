#pragma once
#ifndef TURTLE_GL_WIDGET_H
#define TURTLE_GL_WIDGET_H

#include <QOpenGLWidget>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPaintEvent>

class TurtleGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    TurtleGLWidget(QWidget *parent = nullptr);

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
};

#endif
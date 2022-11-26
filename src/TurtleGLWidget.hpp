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

namespace turtle
{
    /**
     * @brief Adapted from the Qt documentation. 
     * This widget provides a gl canvas for drawing the turtle.
     */
    class TurtleGLWidget : public QOpenGLWidget
    {
        Q_OBJECT

    private:
        static const int GLWIDGET_WIDTH = 400;
        static const int GLWIDGET_HEIGHT = 400;

    public:
        TurtleGLWidget(QWidget *parent = nullptr);
        void set_turtle(turtle::PicoTurtle *t);

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

        /**
         * @brief Extract the color type and alpha type enum values
         * for the given image as the static values sk_color_type,
         * and sk_alpha_type. These values do not change for the image
         * snapshot taken from the turtle canvas over the course of the
         * application and therefore there is no point doing this over
         * and over again.
         * 
         * @param img the turtle canvas snapshot image.
         */
        static void extract_sk_color_props(sk_sp<SkImage> img);
        
        static SkColorType sk_color_type;
        static SkAlphaType sk_alpha_type;

    public:
        virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    };
}
#endif

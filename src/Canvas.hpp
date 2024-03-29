#pragma once

#ifndef CANVAS_H
#define CANVAS_H

#include "TurtleColor.hpp"
#include "TurtleDimension.hpp"
#define TURTLE_DEFAULT_CANVAS_WIDTH 1024
#define TURTLE_DEFAULT_CANVAS_HEIGHT 1024

namespace turtle
{
    /**
     * Abstract Canvas class which declares the primitive operations
     * that a Canvas capable of turtle drawing should support.
     *
     * It should be possible to implement this class using an external
     * library for drawing and then use the rest of the turtle programs
     * without any changes.
     */
    class Canvas
    {

    private:
        // evaluate whether to switch to this class
        TurtleDimension *dimension;
        int width;
        int height;

    public:
        Canvas();
        Canvas(int width, int height);
        ~Canvas();

        int get_width();
        void set_width(int w);
        int get_height();
        void set_height(int h);

        virtual void draw_line(float x1, float y1, float x2, float y2) = 0;
        virtual void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
        virtual void draw_circle(float x, float y, float radius) = 0;
        virtual void update_turtle_brush(TurtleColor *pen_color, int pen_width) = 0;
        virtual void update_canvas() = 0;

        virtual void font(const char *f, unsigned int sz) = 0;
        virtual void filltext(int x, int y, float angle, const char *text) = 0;
        virtual void stroketext(int x, int y, float angle, const char *text) = 0;
        virtual void export_img(const char *filename) = 0;
        virtual void clear() = 0;

        // fill methods
        virtual void begin_fill() = 0;
        virtual void end_fill() = 0;
        virtual bool filling() = 0;
    };
};

#endif // CANVAS_H

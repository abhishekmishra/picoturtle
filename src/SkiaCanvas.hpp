#pragma once

#ifndef SKIACANVAS_H
#define SKIACANVAS_H

#include "Canvas.hpp"
#include "TurtleColor.hpp"

#include "core/SkSurface.h"
#include "core/SkCanvas.h"
#include "core/SkPaint.h"
#include "core/SkRect.h"
#include "core/SkPath.h"
#include "core/SkTextBlob.h"

namespace turtle
{
    class SkiaCanvas : public Canvas
    {
    private:
        sk_sp<SkSurface> rasterSurface;
        SkCanvas *rasterCanvas;
        SkPaint paint;
        SkFont skfont;

    public:
        SkiaCanvas();
        ~SkiaCanvas();

        virtual void draw_line(float x1, float y1, float x2, float y2);
        virtual void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3);
        virtual void update_turtle_brush(TurtleColor *pen_color, int pen_width);
        virtual void update_canvas();

        virtual void font(const char *f, unsigned int sz);
        virtual void filltext(int x, int y, float angle, const char *text);
        virtual void stroketext(int x, int y, float angle, const char *text);
        virtual void export_img(const char *filename);
        virtual void clear();
        
        sk_sp<SkSurface> getRasterSurface();

    private:
        void CreateCanvas();

    };
}
#endif // SKIACANVAS_H

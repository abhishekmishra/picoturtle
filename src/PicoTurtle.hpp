#pragma once

#include "core/SkSurface.h"
#include "core/SkCanvas.h"
#include "core/SkPaint.h"
#include "core/SkRect.h"
#include "core/SkPath.h"
#include "core/SkTextBlob.h"

#include "Turtle.hpp"

namespace turtle
{
    class PicoTurtle;

    typedef void (*picoturtle_callback)(PicoTurtle *, void *);

    class PicoTurtle : public Turtle
    {
    private:
        sk_sp<SkSurface> rasterSurface;
        SkCanvas *rasterCanvas;
        SkPaint paint;
        SkFont skfont;

        void CreateCanvas();

        static picoturtle_callback init_cb;
        static void *init_cb_args;

        static picoturtle_callback destroy_cb;
        static void *destroy_cb_args;

    public:
        PicoTurtle();
        ~PicoTurtle();

        virtual void DrawLine(float x1, float y1, float x2, float y2);
        virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
        virtual void UpdateTurtleBrush();
        virtual void UpdateCanvas();

        virtual void font(const char *f, unsigned int sz);
        virtual void filltext(const char *text);
        virtual void stroketext(const char *text);
        virtual void export_img(const char *filename);
        virtual void clear();

        sk_sp<SkSurface> getRasterSurface();

        static void set_init_callback(picoturtle_callback fn, void *cb_args);
        static void set_destroy_callback(picoturtle_callback fn, void *cb_args);

        static void unset_init_callback();
        static void unset_destroy_callback();
    };
};
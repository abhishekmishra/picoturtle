#include <iostream>
#include <core/SkStream.h>
#include <core/SkImageInfo.h>
#include <core/SkColorType.h>
#include <core/SkAlphaType.h>

#include "PicoTurtle.hpp"

using namespace turtle;

picoturtle_callback PicoTurtle::init_cb = NULL;
void* PicoTurtle::init_cb_args = NULL;
picoturtle_callback PicoTurtle::destroy_cb = NULL;
void* PicoTurtle::destroy_cb_args = NULL;

void PicoTurtle::CreateCanvas()
{
    //SkImageInfo image_info = SkImageInfo::Make(getWidth(), getHeight(), 
    //    SkColorType::kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    //rasterSurface = SkSurface::MakeRaster(image_info);
    rasterSurface =
        SkSurface::MakeRasterN32Premul(getWidth(), getHeight());
    rasterCanvas = rasterSurface->getCanvas();

    rasterCanvas->drawColor(SK_ColorWHITE);
}

void PicoTurtle::UpdateCanvas()
{
    CreateCanvas();
}

PicoTurtle::PicoTurtle() : Turtle()
{
    CreateCanvas();

    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(1);
    font(nullptr, 12);

    UpdateTurtleBrush();

    if (init_cb != NULL)
    {
        init_cb(this, init_cb_args);
    }
};

PicoTurtle::~PicoTurtle()
{
    if (destroy_cb != NULL)
    {
        destroy_cb(this, destroy_cb_args);
    }
}

void PicoTurtle::DrawLine(float x1, float y1, float x2, float y2)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    rasterCanvas->drawPath(path, paint);
}

void PicoTurtle::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    path.lineTo(x3, y3);
    path.lineTo(x1, y1);
    rasterCanvas->drawPath(path, paint);
}

void PicoTurtle::font(const char *f, unsigned int sz)
{
    sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(f, SkFontStyle::Normal());
    skfont = SkFont(typeface, (float)sz, 1.0f, 0.0f);
}

void PicoTurtle::filltext(const char *text)
{
    // get the current paint style
    // and then change it to fill style
    SkPaint::Style oldStyle = paint.getStyle();
    paint.setStyle(SkPaint::kFill_Style);

    // save canvas settings and rotate
    rasterCanvas->save();
    rasterCanvas->rotate(getHeading());

    sk_sp<SkTextBlob> blob =
        SkTextBlob::MakeFromString(text, skfont);
    rasterCanvas->drawTextBlob(blob.get(), getCanvasLocationX(), getCanvasLocationY(), paint);

    rasterCanvas->restore();

    // reset paint style to the old style
    paint.setStyle(oldStyle);
}

void PicoTurtle::stroketext(const char *text)
{
    // get the current paint style
    // and then change it to fill style
    SkPaint::Style oldStyle = paint.getStyle();
    paint.setStyle(SkPaint::kStroke_Style);

    // save canvas settings and rotate
    rasterCanvas->save();
    rasterCanvas->rotate(getHeading());

    sk_sp<SkTextBlob> blob =
        SkTextBlob::MakeFromString(text, skfont);
    rasterCanvas->drawTextBlob(blob.get(), getCanvasLocationX(), getCanvasLocationY(), paint);

    rasterCanvas->restore();

    // reset paint style to the old style
    paint.setStyle(oldStyle);
}

void PicoTurtle::export_img(const char *filename)
{
    sk_sp<SkImage> img(rasterSurface->makeImageSnapshot());
    if (!img)
    {
        return;
    }
    sk_sp<SkData> png(img->encodeToData());
    if (!png)
    {
        return;
    }
    SkFILEWStream out(filename);
    (void)out.write(png->data(), png->size());
}

void PicoTurtle::clear()
{
    rasterCanvas->drawColor(SK_ColorWHITE);
}

void PicoTurtle::UpdateTurtleBrush()
{
    TurtleColor *c = getPenColor();
    U8CPU a = c->getA();
    U8CPU r = c->getR();
    U8CPU g = c->getG();
    U8CPU b = c->getB();
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setStrokeWidth(getPenWidth());
}

sk_sp<SkSurface> PicoTurtle::getRasterSurface() 
{
    return rasterSurface;
}

void PicoTurtle::set_init_callback(picoturtle_callback fn, void* cb_args)
{
    init_cb = fn;
    init_cb_args = cb_args;
}

void PicoTurtle::set_destroy_callback(picoturtle_callback fn, void* cb_args)
{
    destroy_cb = fn;
    destroy_cb_args = cb_args;
}

void PicoTurtle::unset_init_callback()
{
    init_cb = NULL;
    init_cb_args = NULL;
}

void PicoTurtle::unset_destroy_callback()
{
    destroy_cb = NULL;
    destroy_cb_args = NULL;
}

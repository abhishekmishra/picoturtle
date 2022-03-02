#include <iostream>
#include "SkiaTurtle.hpp"

using namespace turtle;

void SkiaTurtle::CreateCanvas()
{
    rasterSurface =
        SkSurface::MakeRasterN32Premul(getWidth(), getHeight());
    rasterCanvas = rasterSurface->getCanvas();

    rasterCanvas->drawColor(SK_ColorWHITE);
}

void SkiaTurtle::UpdateCanvas()
{
    CreateCanvas();
}

SkiaTurtle::SkiaTurtle() : Turtle()
{
    CreateCanvas();
    
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(1);
    font(nullptr, 12);

    UpdateTurtleBrush();
};

SkiaTurtle::~SkiaTurtle()
{
}

void SkiaTurtle::DrawLine(float x1, float y1, float x2, float y2)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    rasterCanvas->drawPath(path, paint);
}

void SkiaTurtle::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    path.lineTo(x3, y3);
    path.lineTo(x1, y1);
    rasterCanvas->drawPath(path, paint);
}

void SkiaTurtle::font(const char *f, unsigned int sz)
{
    sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(f, SkFontStyle::Normal());
    skfont = SkFont(typeface, (float)sz, 1.0f, 0.0f);
}

void SkiaTurtle::filltext(const char *text)
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

void SkiaTurtle::stroketext(const char *text)
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

void SkiaTurtle::export_img(const char *filename)
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

void SkiaTurtle::clear()
{
    rasterCanvas->drawColor(SK_ColorWHITE);
}

void SkiaTurtle::UpdateTurtleBrush()
{
    TurtleColor *c = getPenColor();
    U8CPU a = c->getA();
    U8CPU r = c->getR();
    U8CPU g = c->getG();
    U8CPU b = c->getB();
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setStrokeWidth(getPenWidth());
}
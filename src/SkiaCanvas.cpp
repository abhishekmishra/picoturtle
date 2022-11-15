#include "SkiaCanvas.hpp"
#include <core/SkStream.h>
#include <core/SkImageInfo.h>
#include <core/SkColorType.h>
#include <core/SkAlphaType.h>

using namespace turtle;

void SkiaCanvas::CreateCanvas()
{
    //SkImageInfo image_info = SkImageInfo::Make(getWidth(), getHeight(), 
    //    SkColorType::kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    //rasterSurface = SkSurface::MakeRaster(image_info);
    rasterSurface =
        SkSurface::MakeRasterN32Premul(getWidth(), getHeight());
    rasterCanvas = rasterSurface->getCanvas();

    rasterCanvas->drawColor(SK_ColorWHITE);
}

void SkiaCanvas::UpdateCanvas()
{
    //TODO: cleanup existing skia objects if already created.
    
    CreateCanvas();

    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(1);
    font(nullptr, 12);

    // todo needs to be called by turtle now
    // UpdateTurtleBrush();
}

SkiaCanvas::SkiaCanvas()
{
    UpdateCanvas();
};

SkiaCanvas::~SkiaCanvas()
{
};

void SkiaCanvas::DrawLine(float x1, float y1, float x2, float y2)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    rasterCanvas->drawPath(path, paint);
}

void SkiaCanvas::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    path.lineTo(x3, y3);
    path.lineTo(x1, y1);
    rasterCanvas->drawPath(path, paint);
}

void SkiaCanvas::font(const char *f, unsigned int sz)
{
    sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(f, SkFontStyle::Normal());
    skfont = SkFont(typeface, (float)sz, 1.0f, 0.0f);
}

void SkiaCanvas::filltext(int x, int y, float angle, const char *text)
{
    // get the current paint style
    // and then change it to fill style
    SkPaint::Style oldStyle = paint.getStyle();
    paint.setStyle(SkPaint::kFill_Style);

    // save canvas settings and rotate
    rasterCanvas->save();
    rasterCanvas->rotate(angle);

    sk_sp<SkTextBlob> blob =
        SkTextBlob::MakeFromString(text, skfont);
    rasterCanvas->drawTextBlob(blob.get(), x, y, paint);

    rasterCanvas->restore();

    // reset paint style to the old style
    paint.setStyle(oldStyle);
}

void SkiaCanvas::stroketext(int x, int y, float angle, const char *text)
{
    // get the current paint style
    // and then change it to fill style
    SkPaint::Style oldStyle = paint.getStyle();
    paint.setStyle(SkPaint::kStroke_Style);

    // save canvas settings and rotate
    rasterCanvas->save();
    rasterCanvas->rotate(angle);

    sk_sp<SkTextBlob> blob =
        SkTextBlob::MakeFromString(text, skfont);
    rasterCanvas->drawTextBlob(blob.get(), x, y, paint);

    rasterCanvas->restore();

    // reset paint style to the old style
    paint.setStyle(oldStyle);
}

void SkiaCanvas::export_img(const char *filename)
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

void SkiaCanvas::clear()
{
    rasterCanvas->drawColor(SK_ColorWHITE);
}

void SkiaCanvas::UpdateTurtleBrush(TurtleColor *pen_color, int pen_width)
{
    U8CPU a = pen_color->getA();
    U8CPU r = pen_color->getR();
    U8CPU g = pen_color->getG();
    U8CPU b = pen_color->getB();
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setStrokeWidth(pen_width);
}

sk_sp<SkSurface> SkiaCanvas::getRasterSurface() 
{
    return rasterSurface;
}

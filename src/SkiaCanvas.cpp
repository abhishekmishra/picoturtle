#include "SkiaCanvas.hpp"
#include <core/SkStream.h>
#include <core/SkImageInfo.h>
#include <core/SkColorType.h>
#include <core/SkAlphaType.h>
#include <encode/SkPngEncoder.h>

using namespace turtle;

void SkiaCanvas::CreateCanvas()
{
    // SkImageInfo image_info = SkImageInfo::Make(getWidth(), getHeight(),
    //     SkColorType::kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    // rasterSurface = SkSurface::MakeRaster(image_info);
    // rasterSurface =
    //     SkSurface::MakeRasterN32Premul(get_width(), get_height());

    SkImageInfo image_info = SkImageInfo::Make(get_width(), get_height(),
        SkColorType::kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    rasterSurface =
         SkSurfaces::Raster(image_info);
    rasterCanvas = rasterSurface->getCanvas();

    rasterCanvas->drawColor(SK_ColorWHITE);
}

void SkiaCanvas::update_canvas()
{
    // TODO: cleanup existing skia objects if already created.

    CreateCanvas();

    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(1);
    font(nullptr, 12);
    in_fill = false;
    // todo needs to be called by turtle now
    // update_turtle_brush();
}

SkiaCanvas::SkiaCanvas()
{
    update_canvas();
};

SkiaCanvas::~SkiaCanvas(){};

void SkiaCanvas::draw_line(float x1, float y1, float x2, float y2)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    rasterCanvas->drawPath(path, paint);
}

void SkiaCanvas::draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    SkPath path;
    path.moveTo(x1, y1);
    path.lineTo(x2, y2);
    path.lineTo(x3, y3);
    path.lineTo(x1, y1);
    rasterCanvas->drawPath(path, paint);
}

void SkiaCanvas::draw_circle(float x, float y, float radius)
{
    rasterCanvas->drawCircle(x, y, radius, paint);
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

    rasterCanvas->rotate(angle, x, y);

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

    rasterCanvas->rotate(angle, x, y);

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
    sk_sp<SkData> png(SkPngEncoder::Encode(nullptr, img.get(), SkPngEncoder::Options()));
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

void SkiaCanvas::draw_skimage(const sk_sp<SkImage> &image, float left,
                              float top)
{
    rasterCanvas->drawImage(image, left, top);
}

void SkiaCanvas::update_turtle_brush(TurtleColor *pen_color, int pen_width)
{
    U8CPU a = pen_color->get_a();
    U8CPU r = pen_color->get_r();
    U8CPU g = pen_color->get_g();
    U8CPU b = pen_color->get_b();
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setStrokeWidth(pen_width);
}

sk_sp<SkSurface> SkiaCanvas::getRasterSurface()
{
    return rasterSurface;
}

// fill methods
void SkiaCanvas::begin_fill()
{
    paint.setStyle(SkPaint::kFill_Style);
    in_fill = true;
}

void SkiaCanvas::end_fill()
{
    paint.setStyle(SkPaint::kStroke_Style);
    in_fill = false;
}

bool SkiaCanvas::filling()
{
    return in_fill;
}

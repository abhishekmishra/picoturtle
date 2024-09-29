#include "SkiaCanvas.hpp"
#include <core/SkStream.h>
#include <core/SkImageInfo.h>
#include <core/SkColorType.h>
#include <core/SkAlphaType.h>
#include <core/SkFontMgr.h>
#include <encode/SkPngEncoder.h>

#if defined(SK_BUILD_FOR_WIN) && (defined(SK_FONTMGR_GDI_AVAILABLE) || defined(SK_FONTMGR_DIRECTWRITE_AVAILABLE))
#include "include/ports/SkTypeface_win.h"
#endif

#if defined(SK_BUILD_FOR_ANDROID) && defined(SK_FONTMGR_ANDROID_AVAILABLE)
#include "include/ports/SkFontMgr_android.h"
#endif

#if defined(SK_FONTMGR_CORETEXT_AVAILABLE) && (defined(SK_BUILD_FOR_IOS) || defined(SK_BUILD_FOR_MAC))
#include "include/ports/SkFontMgr_mac_ct.h"
#endif

#if defined(SK_FONTMGR_FONTCONFIG_AVAILABLE)
#include "include/ports/SkFontMgr_fontconfig.h"
#endif

#if defined(SK_FONTMGR_FREETYPE_DIRECTORY_AVAILABLE)
#include "include/ports/SkFontMgr_directory.h"
#endif

#if defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
#include "include/ports/SkFontMgr_empty.h"
#endif

#ifndef SK_FONT_FILE_PREFIX
#  if defined(SK_BUILD_FOR_MAC) || defined(SK_BUILD_FOR_IOS)
#    define SK_FONT_FILE_PREFIX "/System/Library/Fonts/"
#  else
#    define SK_FONT_FILE_PREFIX "/usr/share/fonts/"
#  endif
#endif

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

SkiaCanvas::~SkiaCanvas() {};

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

    /// Creates a new system font manager, empty if none is available.
        sk_sp<SkFontMgr> fontMgr;
#if defined(SK_BUILD_FOR_WIN) && defined(SK_FONTMGR_GDI_AVAILABLE)
        fontMgr = SkFontMgr_New_GDI();
#elif defined(SK_BUILD_FOR_ANDROID) && defined(SK_FONTMGR_ANDROID_AVAILABLE)
        fontMgr = SkFontMgr_New_Android(nullptr);
#elif defined(SK_BUILD_FOR_WIN) && defined(SK_FONTMGR_DIRECTWRITE_AVAILABLE)
        fontMgr = SkFontMgr_New_DirectWrite();
#elif defined(SK_FONTMGR_CORETEXT_AVAILABLE) && (defined(SK_BUILD_FOR_IOS) || defined(SK_BUILD_FOR_MAC))
        fontMgr = SkFontMgr_New_CoreText(nullptr);
#elif defined(SK_FONTMGR_FONTCONFIG_AVAILABLE)
        fontMgr = SkFontMgr_New_FontConfig(nullptr);
#elif defined(SK_FONTMGR_FREETYPE_DIRECTORY_AVAILABLE)
        // In particular, this is used on ChromeOS, which is Linux-like but doesn't have
        // FontConfig.
        fontMgr = SkFontMgr_New_Custom_Directory(SK_FONT_FILE_PREFIX);
#elif defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
        fontMgr = SkFontMgr_New_Custom_Empty();
#else
        fontMgr = SkFontMgr::RefEmpty();
#endif

    // Create typeface from font name
    sk_sp<SkTypeface> typeface = fontMgr->matchFamilyStyle(f, SkFontStyle::Normal());
    // Check if typeface creation was successful
    if (!typeface) {
        // Handle error: fallback to default typeface if necessary
        typeface = fontMgr->matchFamilyStyle(nullptr, SkFontStyle::Normal());
    }

    // sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(f, SkFontStyle::Normal());
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

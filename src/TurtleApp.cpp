#include "core/SkCanvas.h"
#include "core/SkRect.h"
#include "core/SkRRect.h"
#include "core/SkPath.h"
#include "core/SkData.h"
#include "core/SkImage.h"
#include "core/SkStream.h"
#include "core/SkSurface.h"

#include <lua.h>

#include "SkiaTurtle.hpp"
#include "SkiaTurtleLua.hpp"

void draw(SkCanvas *canvas)
{
    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(4);
    paint.setColor(SK_ColorRED);

    SkRect rect = SkRect::MakeXYWH(50, 50, 40, 60);
    canvas->drawRect(rect, paint);

    SkRRect oval;
    oval.setOval(rect);
    oval.offset(40, 60);
    paint.setColor(SK_ColorBLUE);
    canvas->drawRRect(oval, paint);

    paint.setColor(SK_ColorCYAN);
    canvas->drawCircle(180, 50, 25, paint);

    rect.offset(80, 0);
    paint.setColor(SK_ColorYELLOW);
    canvas->drawRoundRect(rect, 10, 10, paint);

    SkPath path;
    path.cubicTo(768, 0, -512, 256, 256, 256);
    paint.setColor(SK_ColorGREEN);
    canvas->drawPath(path, paint);

    // canvas->drawImage(image, 128, 128, SkSamplingOptions(), &paint);

    // SkRect rect2 = SkRect::MakeXYWH(0, 0, 40, 60);
    // canvas->drawImageRect(image, rect2, SkSamplingOptions(), &paint);

    // SkPaint paint2;
    // auto text = SkTextBlob::MakeFromString("Hello, Skia!", SkFont(nullptr, 18));
    // canvas->drawTextBlob(text.get(), 50, 25, paint2);
}

void raster(int width, int height,
            void (*draw)(SkCanvas *),
            const char *path)
{
    sk_sp<SkSurface> rasterSurface =
        SkSurface::MakeRasterN32Premul(width, height);
    SkCanvas *rasterCanvas = rasterSurface->getCanvas();
    draw(rasterCanvas);
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
    SkFILEWStream out(path);
    (void)out.write(png->data(), png->size());
}

int main(int argc, char *argv[])
{
    // raster(256, 256, draw, "out/out_raster.png");
    turtle::SkiaTurtle *skTurtle = new turtle::SkiaTurtle();

    skTurtle->pendown();
    skTurtle->forward(100);
    skTurtle->right(90);
    skTurtle->forward(100);
    skTurtle->right(90);
    skTurtle->forward(100);
    skTurtle->right(90);
    skTurtle->forward(100);
    skTurtle->right(90);
    skTurtle->stop();

    skTurtle->export_img("out/out_raster.png");

    delete skTurtle;

    lua_State *L;
    initTurtleLuaBinding(&L);
    runLuaFile(L, "lua/TurtleInit.lua");
    // runLuaScript(L, "export_img(\"./out/tinit.png\")");
    // runLuaFile(L, "lua/runSamples.lua");

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            const char *fname = argv[i];
            printf("input [%d], %s\n", i, fname);
            runLuaFile(L, fname);
        }
    }

    cleanupTurtleLuaBinding(L);

    return 0;
}
#pragma once

#include "core/SkSurface.h"

#include "Turtle.hpp"
#include "SkiaCanvas.hpp"
#include "PicoTurtleImage.hpp"

namespace turtle
{
    class PicoTurtle;

    typedef void (*picoturtle_callback)(PicoTurtle *, void *);
    typedef void (*picoturtle_delay)(PicoTurtle *, int tm);

    /**
     * @brief PicoTurtle implements the turtle, with the ability
     * to track the turtle across creation, state changes and destruction.
     */
    class PicoTurtle : public Turtle
    {
    private:
        static SkiaCanvas *turtleCanvas;

        static picoturtle_callback init_cb;
        static void *init_cb_args;

        static picoturtle_callback update_cb;
        static void *update_cb_args;

        static picoturtle_callback paint_cb;
        static void *paint_cb_args;

        static picoturtle_delay delay_cb;

        static picoturtle_callback destroy_cb;
        static void *destroy_cb_args;

        bool update_enabled;

    public:
        PicoTurtle();
        ~PicoTurtle();

        static void set_init_callback(picoturtle_callback fn, void *cb_args);
        static void set_update_callback(picoturtle_callback fn, void *cb_args);
        static void set_delay_callback(picoturtle_delay fn);
        static void set_paint_callback(picoturtle_callback fn, void *cb_args);
        static void set_destroy_callback(picoturtle_callback fn, void *cb_args);

        static void unset_init_callback();
        static void unset_update_callback();
        static void unset_delay_callback();
        static void unset_paint_callback();
        static void unset_destroy_callback();

        virtual void delay_ms(int tm);
        virtual void paint();

        virtual void enable_update();
        virtual void disable_update();
        virtual bool is_update_enabled();
        virtual void call_update();

        virtual PicoTurtleImage* load_image(const char *img_path);
        virtual void draw_image_file(const char *img_path);
        virtual void draw_image(PicoTurtleImage *img);

        sk_sp<SkSurface> getRasterSurface();

        // ---- The following methods are overridden from Turtle -----
        // They change the turtle/canvas and therefore need to
        // provide update notification.
        //
        // These methods are overridden to simply call the
        // base class implementation and then call the update callback
        // -----------------------------------------------------------
        virtual void penup();

        virtual void pendown();

        virtual void penwidth(float w);

        virtual void stop();

        virtual void home();

        virtual void forward(float d);

        virtual void back(float d);

        virtual void setpos(float x, float y);

        virtual void setx(float x);

        virtual void sety(float y);

        virtual void left(float a);

        virtual void right(float a);

        virtual void heading(float a);

        virtual void pencolor(unsigned int r, unsigned int g, unsigned int b);

        virtual int pencolor(const char *color);

        virtual void set_canvas_size(unsigned int width, unsigned int height);

        virtual void reset();

        virtual void font(const char *f, unsigned int sz);

        virtual void filltext(const char *text);

        virtual void stroketext(const char *text);

        virtual void circle(float radius);

        virtual void arc(float radius, float extent = -1, int steps = -1);
    };
};

#pragma once

#include "core/SkSurface.h"

#include "Turtle.hpp"
#include "SkiaCanvas.hpp"

namespace turtle
{
    class PicoTurtle;

    typedef void (*picoturtle_callback)(PicoTurtle *, void *);

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

        static picoturtle_callback destroy_cb;
        static void *destroy_cb_args;

    public:
        PicoTurtle();
        ~PicoTurtle();

        static void set_init_callback(picoturtle_callback fn, void *cb_args);
        static void set_destroy_callback(picoturtle_callback fn, void *cb_args);

        static void unset_init_callback();
        static void unset_destroy_callback();

        sk_sp<SkSurface> getRasterSurface();
    };
};

#pragma once

#include <string>

#include "Canvas.hpp"
#include "TurtleColor.hpp"
#include "TurtleDimension.hpp"
#include "TurtleLocation.hpp"
#include "TurtleState.hpp"
#include "TurtleOptions.hpp"

namespace turtle
{
    class Turtle
    {
    private:
        TurtleState* turtle_state;

        TurtleOptions *options;

        std::string name;
        std::string id;

        Canvas *canvas;

    public:

        /**
         * Create a Turtle with the default options.
         *
         */
        Turtle();

        /**
         * Create a Turtle with the given TurtleOptions.
         *
         * @param options TurtleOptions object
         */
        Turtle(TurtleOptions *options); //, Canvas canvas);

        void set_canvas(Canvas* c);
        Canvas *get_canvas();
        std::string get_name();
        std::string get_id();

        TurtleColor *get_pen_color();

        float get_canvas_location_x();
        float get_canvas_location_y();
        float get_x();
        float get_y();
        float canvas_heading(); // return 360.0 - Angle;
        float get_pen_width();
        float get_heading();

        void DrawTurtle();

        void penup();

        void pendown();

        void penwidth(float w);

        void stop();

        // TurtleState state();

        void home();

        void forward(float d);

        void back(float d);

        void setpos(float x, float y);

        float getx();

        float gety();

        void setx(float x);

        void sety(float y);

        void left(float a);

        void right(float a);

        void heading(float a);

        /**
         * Set the pen colour using the given rgb values
         *
         * @param r red value (0-255)
         * @param g green value (0-255)
         * @param b blue value (0-255)
         */
        void pencolor(unsigned int r, unsigned int g, unsigned int b);

        /**
         * Set the pen color using the given color name.
         * This call will fail to set a color and return a
         * non-zero code if the given color name is not found
         * in the existing list of colors in the c-color-names lib.
         * NOTE: The name of the color MUST be in lowercase.
         *
         * @param color name of the color to set.
         * @return 0 if color was found and set, non-zero otherwise.
         */
        int pencolor(const char* color);

        // TODO: change width and height to unsigned int
        void canvas_size(int width, int height);

        void reset();

        void font(const char *f, unsigned int sz);
        void filltext(const char *text);
        void stroketext(const char *text);
    };
};

#pragma once

#include <string>
#include <chrono>

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
        TurtleState *current_state;
        TurtleState *saved_state;

        TurtleOptions *options;

        std::string name;
        std::string id;

        Canvas *canvas;

        std::chrono::steady_clock::time_point start_tm;

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

        void set_canvas(Canvas *c);
        Canvas *get_canvas();

        std::string get_name();
        std::string get_id();

        /**
         * @brief get the elapsed time since the construction
         * of this turtle object.
         *
         * @return elapsed time in milliseconds
         */
        long long elapsed_time_ms();

        /**
         * @brief delay the turtle by given ms.
         * This method does noting in the base implementation.
         * Only if a non-blocking delay callback is provided,
         * will the delay work.
         *
         * @param tm time to delay in milliseconds.
         */
        virtual void delay_ms(int tm);

        /**
         * @brief The paint will draw the turtle current state
         * on a display device. This will only be supported through
         * an environment which provides a callback for paint.
         *
         * In the default implementation, with no display device,
         * this method has no effect.
         */
        virtual void paint();

        // state related functions
        /**
         * Returns a detached copy of the current turtle state.
         * Changing the state via the returned object, does
         * not change the turtle.
         *
         * @return current turtle state as a detached copy.
         */
        TurtleState *get_current_state() const;

        /**
         * Returns a detached copy of the saved turtle state.
         * Changing the state via the returned object, does
         * not change the saved state of the turtle.
         *
         * @return saved turtle state as a detached copy.
         */
        TurtleState *get_saved_state() const;

        /**
         * Save the current state as the saved state.
         * The saved stated can be restored later.
         */
        void save();

        /**
         * Restore the saved state and discard current state.
         */
        void restore();

        TurtleColor *get_pen_color();

        float get_canvas_location_x();
        float get_canvas_location_y();
        float getx();
        float gety();
        float canvas_heading(); // return 360.0 - Angle;
        float get_pen_width();
        float get_heading();

        void draw_turtle();

        /**
         * @brief raise the turtle's pen.
         */
        virtual void penup();

        /**
         * @brief lower the turtle's pen.
         */
        virtual void pendown();

        /**
         * @brief set the turtle's pen's width in pixels.
         *
         * @param w pixel width as a floating point number.
         */
        virtual void penwidth(float w);

        /**
         * @brief stop the turtle. Does nothing for now except
         * draw the turtle as a red triangle.
         */
        virtual void stop();

        /**
         * @brief send the turtle home (at the center of the screen).
         */
        virtual void home();

        /**
         * @brief move the turtle forward by d pixels.
         *
         * @param d distance to move the turtle in pixels.
         */
        virtual void forward(float d);

        /**
         * @brief move the turtle back by d pixels.
         *
         * @param d distance to move the turtle in pixels.
         */
        virtual void back(float d);

        /**
         * @brief set the position of the turtle to (x,y)
         * on the canvas. x & y are pixels.
         *
         * @param x x-coord
         * @param y y-coord
         */
        virtual void setpos(float x, float y);

        /**
         * @brief set the x-coord of the turtle.
         * on the canvas. x is in pixels.
         *
         * @param x x-coord
         */
        virtual void setx(float x);

        /**
         * @brief set the y-coord of the turtle.
         * on the canvas. y is in pixels.
         *
         * @param y y-coord
         */
        virtual void sety(float y);

        /**
         * @brief turn the turtle left by a degrees.
         *
         * @param a angle (in degrees) to turn the the turtle
         */
        virtual void left(float a);

        /**
         * @brief turn the turtle right by a degrees.
         *
         * @param a angle (in degrees) to turn the the turtle
         */
        virtual void right(float a);

        /**
         * @brief set the heading of the turtle to a degrees
         *
         * @param a angle (in degrees)
         */
        virtual void heading(float a);

        /**
         * @brief Set the pen colour using the given rgb values
         *
         * @param r red value (0-255)
         * @param g green value (0-255)
         * @param b blue value (0-255)
         */
        virtual void pencolor(unsigned int r, unsigned int g, unsigned int b);

        /**
         * @brief Set the pen color using the given color name.
         * This call will fail to set a color and return a
         * non-zero code if the given color name is not found
         * in the existing list of colors in the c-color-names lib.
         * NOTE: The name of the color MUST be in lowercase.
         *
         * @param color name of the color to set.
         * @return 0 if color was found and set, non-zero otherwise.
         */
        virtual int pencolor(const char *color);

        /**
         * Set the canvas size of the canvas assigned to the turtle object.
         *
         * @param width width of canvas in pixels.
         * @param height height of canvas in pixels.
         */
        virtual void set_canvas_size(unsigned int width, unsigned int height);

        /**
         * Get the width of the assigned canvas in pixels.
         *
         * @return width of canvas.
         */
        virtual unsigned int get_canvas_width();

        /**
         * Get the height of the assigned canvas in pixels.
         *
         * @return height of canvas.
         */
        virtual unsigned int get_canvas_height();

        /**
         * @brief Reset the turtle's pen, and location.
         */
        virtual void reset();

        /**
         * @brief Set the turtle's font to given font name and size
         *
         * @param f font name
         * @param sz font size
         */
        virtual void font(const char *f, unsigned int sz);

        /**
         * @brief Write the given text (filled) at the turtle's position,
         * in the turtle's heading as orientation of text.
         *
         * @param text text to write.
         */
        virtual void filltext(const char *text);

        /**
         * @brief Write the given text (with stroke) at the turtle's position,
         * in the turtle's heading as orientation of text.
         *
         * @param text text to write.
         */
        virtual void stroketext(const char *text);
    };
};

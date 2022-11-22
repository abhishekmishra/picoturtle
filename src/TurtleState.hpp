#pragma once

#include <string>
#include "TurtleColor.hpp"
#include "TurtleLocation.hpp"
#include "TurtleDimension.hpp"

#define TURTLE_DEFAULT_PENWIDTH 1
#define TURTLE_DEFAULT_HEADING 90.0f

namespace turtle
{
    /**
     * This class provides all the state associated
     * with a Turtle that is required to render it.
     *
     * It should be possible to clone everything about the
     * visual aspects of a turtle simply by applying a new state object.
     *
     * This state object can also be returned as a read-only
     * object to turtle programs for introspection or for debugging.
     *
     * Finally, the turtle object can keep multiple copies
     * of this object to switch between various turtle states.
     */
    class TurtleState
    {
    private:
        // location of the turtle
        TurtleLocation *location;
        // heading/angle of the turtle
        double heading;

        // colour of the pen
        TurtleColor *pen_color;
        // flag indicating whether pen is down or up
        bool pen_down;
        // width of the pen
        float pen_width;

        // font string identifying the font for writing
        std::string font_str;

    public:
        /**
         * Creates a TurtleState with default values.
         */
        TurtleState();

        /**
         * Turtle default destructor.
         */
        ~TurtleState();

        /**
         * Copy over all state value from another turtle state.
         * This is a deep copy.
         *
         * The program does nothing if other is NULL
         *
         * @param other another turtle state instance.
         */
        void copyFromTurtleState(TurtleState *other);

        /**
         * Get the current location of the turtle as a TurtleLocation object.
         * The location of the turtle can be changed by modifying the
         * turtle location object
         *
         * @return location of the turtle
         */
        TurtleLocation *get_location() const;

        /**
         * Get the heading/angle of the turtle in degrees
         *
         * @return heading of the turtle in degrees
         */
        double get_heading() const;

        /**
         * Set the heading to the given angle (in degrees)
         *
         * @param heading in degrees
         */
        void set_heading(double heading);

        /**
         * Get the color of the turtle's pen as a TurtleColor object.
         * The color of the turtle can be modified by modifying the color object.
         *
         * @return color of the turtle
         */
        TurtleColor *get_pen_color() const;

        /**
         * Get a flag indicating whether the turtle's pen is down for writing.
         *
         * @return pen down flag
         */
        bool is_pen_down() const;

        /**
         * Change the state of the pen_down flag.
         *
         * @param flag the pen down flag
         */
        void set_pen_down(bool flag);

        /**
         * Get the width of the pen as a float.
         *
         * @return pen width
         */
        float get_pen_width() const;

        /**
         * Set the pen width as a float.
         *
         * @param pen_width
         */
        void set_pen_width(float pen_width);

        /**
         * Get the font string of the pen
         *
         * @return font string (can be NULL)
         */
        std::string get_font_string() const;

        /**
         * Set the font string of the pen.
         *
         * @param font_string
         */
        void set_font_string(std::string font_string);
    };
};

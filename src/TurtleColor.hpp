#pragma once
#include <cstdint>
#include <string>

namespace turtle
{
    class TurtleColor
    {
    public:
        static const std::string DEFAULT_COLOR_NAME;

    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
        std::string name;

        void init_default();

    public:
        /**
         * @brief Construct a new Turtle Color object using the default color name.
         */
        TurtleColor();

        /**
         * Construct a new Turtle Color object by a color name.
         * The available names of colors are defined in c-color-names/color_names.h
         * 
         * If the requested color name is not found in the color_names list
         * then the default value of the color name is used.
         *
         * @param color_name name of the color
         */
        TurtleColor(std::string name);

        /**
         * @brief Construct a new Turtle Color object with given R,G,B,A 8-bit values.
         * 
         * @param rv red
         * @param gv green
         * @param bv blue
         * @param av alpha
         */
        TurtleColor(uint8_t rv, uint8_t gv, uint8_t bv, uint8_t av);

        ~TurtleColor();

        /**
         * @brief Get the red color value.
         * 
         * @return uint8_t red
         */
        uint8_t get_r();

        /**
         * @brief Get the green color value
         * 
         * @return uint8_t green
         */
        uint8_t get_g();

        /**
         * @brief Get the blue color value
         * 
         * @return uint8_t blue
         */
        uint8_t get_b();

        /**
         * @brief Get the alpha value
         * 
         * @return uint8_t alpha
         */
        uint8_t get_a();

        /**
         * @brief Get the color name
         * 
         * @return string color name
         */
        std::string get_color_name();

        /**
         * @brief Set the red value
         * 
         * @param r
         */
        void set_r(uint8_t r);

        /**
         * @brief Set the green value
         * 
         * @param g 
         */
        void set_g(uint8_t g);

        /**
         * @brief Set the blue value
         * 
         * @param b 
         */
        void set_b(uint8_t b);

        /**
         * @brief Set the alpha value
         * 
         * @param a 
         */
        void set_a(uint8_t a);

        /**
         * @brief Set the color name
         * 
         * @param color color name
         */
        void set_color_name(std::string color);

        /**
         * Update TurtleColor object by a color name.
         * The available names of colors are defined in c-color-names/color_names.h
         * 
         * If the color name is found the object is updated, and value 1 is returned.
         * Otherwise color name is not updated and value 0 is returned.
         *
         * @param color_name name of the color
         *
         * @return int flag (1=found, 0=not found) indicating whether color_name was found, in which case
         *  TurtleColor object is set.
         */
        int update_color_by_name(std::string color_name);
    };
};

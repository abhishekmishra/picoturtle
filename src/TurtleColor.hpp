#pragma once
#include <cstdint>

namespace turtle
{
    class TurtleColor
    {
    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

    public:
        TurtleColor();
        TurtleColor(uint8_t rv, uint8_t gv, uint8_t bv, uint8_t av);
        ~TurtleColor();

        uint8_t getR();
        uint8_t getG();
        uint8_t getB();
        uint8_t getA();

        void setR(uint8_t r);
        void setG(uint8_t g);
        void setB(uint8_t b);
        void setA(uint8_t a);

        /**
         * Create the TurtleColor object by a color name.
         * The available names of colors are defined in c-color-names/color_names.h
         * 
         * @param c TurtleColor object to return
         * @param color_name name of the color
         * 
         * @return int flag (1=found, 0=not found) indicating whether color_name was found, in which case
         *  TurtleColor object is set.
        */
        static int get_color_by_name(TurtleColor** c, const char* color_name);
    };
};

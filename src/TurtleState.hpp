#pragma once

#include <string>
#include "TurtleColor.hpp"
#include "TurtleLocation.hpp"
#include "TurtleDimension.hpp"

namespace turtle
{
    class TurtleState
    {
    private:
        TurtleColor* color;
        TurtleLocation* location;
        double angle;
        bool pen;
        float pen_width;
        std::string id;
        std::string name;
        int last;
        std::string font_str;
        TurtleDimension* canvas_size;

    public:
        TurtleState();
        ~TurtleState();

        TurtleColor* getColor();
        TurtleLocation* getLocation();
        double getAngle();
        bool isPenDown();
        float getPenWidth();
        std::string getId();
        std::string getName();
        int getLast();
        std::string getFont();
        TurtleDimension* getCanvasSize();
    };
};

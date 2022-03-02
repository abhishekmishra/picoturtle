#pragma once

namespace turtle
{
    class TurtleLocation
    {
    private:
        float x;
        float y;

    public:
        TurtleLocation();
        TurtleLocation(float x, float y);
        ~TurtleLocation();

        float getX();
        float getY();
        void setX(float x);
        void setY(float y);
    };

};
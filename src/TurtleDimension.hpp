#pragma once

namespace turtle
{
    class TurtleDimension
    {
    private:
        float width;
        float height;

    public:
        TurtleDimension();
        TurtleDimension(double w, double h);
        ~TurtleDimension();

        float getWidth();
        float getHeight();
    };
};
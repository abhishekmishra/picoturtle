#pragma once

namespace turtle
{
    class TurtleColor
    {
    private:
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;

    public:
        TurtleColor();
        TurtleColor(unsigned int rv, unsigned int gv, unsigned int bv, unsigned int av);
        ~TurtleColor();

        unsigned int getR();
        unsigned int getG();
        unsigned int getB();
        unsigned int getA();

        void setR(unsigned int r);
        void setG(unsigned int g);
        void setB(unsigned int b);
        void setA(unsigned int a);
    };
};
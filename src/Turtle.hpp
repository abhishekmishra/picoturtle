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
        TurtleOptions *Options;
        std::string Name;
        std::string Id;

        Canvas *canvas;

        // private List<TurtleCommand> commands;
        // public Canvas Canvas { get; set; }

        // private readonly SolidColorBrush turtleDrawingBrush;

        // public FontFamily TurtleFontFamily { get; private set; } = new FontFamily("Arial");
        // public int TurtleFontSize { get; private set; } = 12;

        // public uint dpi { get; set; }

        // public static Turtle CreateTurtle(string[] args)
        // {
        //     return new Turtle(new TurtleOptions(), null);
        // }
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

        void setCanvas(Canvas* c);
        Canvas *getCanvas();
        std::string getName();
        std::string getId();

        TurtleColor *getPenColor();

        float getCanvasLocationX();
        float getCanvasLocationY();
        float getX();
        float getY();
        float CanvasAngle(); // return 360.0 - Angle;
        float getPenWidth();
        float getHeading();

        TurtleState *CurrentState();

        void DrawTurtle();

        TurtleState TurtleRequest(std::string cmd);

        TurtleState Init();

        void penup();

        void pendown();

        void penwidth(float w);

        void stop();

        TurtleState state();

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

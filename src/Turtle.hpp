#pragma once

#include <string>

#include "Canvas.hpp"
#include "TurtleColor.hpp"
#include "TurtleDimension.hpp"
#include "TurtleLocation.hpp"
#include "TurtleState.hpp"
#include "TurtleOptions.hpp"

#define TURTLE_DEFAULT_PENWIDTH 1
#define TURTLE_DEFAULT_HEADING 90.0f

namespace turtle
{
    class Turtle
    {
    private:
        TurtleOptions *Options;
        std::string Name;
        std::string Id;

        bool IsPenDown;
        TurtleColor *PenColor;
        float PenWidth;
        // private SolidColorBrush turtleBrush;

        TurtleLocation *Location;
        TurtleColor *Colour;
        float Angle;

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
        Turtle();

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

        void pencolour(unsigned int r, unsigned int g, unsigned int b);

        // TODO: change width and height to unsigned int
        void canvas_size(int width, int height);

        void reset();

        void font(const char *f, unsigned int sz);
        void filltext(const char *text);
        void stroketext(const char *text);
    };
};

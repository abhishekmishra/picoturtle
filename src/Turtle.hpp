#pragma once

#include <string>

#include "TurtleColor.hpp"
#include "TurtleDimension.hpp"
#include "TurtleLocation.hpp"
#include "TurtleState.hpp"
#include "TurtleOptions.hpp"

#define TURTLE_DEFAULT_CANVAS_WIDTH 800
#define TURTLE_DEFAULT_CANVAS_HEIGHT 600
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

        TurtleDimension *Dimension;
        int Width;
        int Height;
        TurtleLocation *Location;
        TurtleColor *Colour;
        float Angle;

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

        TurtleColor* getPenColor();

        int getWidth();
        void setWidth(int w);
        int getHeight();
        void setHeight(int h);

        float getCanvasLocationX();
        float getCanvasLocationY();
        float getX();
        float getY();
        float CanvasAngle(); // return 360.0 - Angle;
        float getPenWidth();
        float getHeading();

        virtual void DrawLine(float x1, float y1, float x2, float y2) = 0;
        virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
        virtual void UpdateTurtleBrush() = 0;
        virtual void UpdateCanvas() = 0;

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

        virtual void clear() = 0;

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

        virtual void font(const char* f, unsigned int sz) = 0;

        virtual void filltext(const char* text) = 0;

        virtual void stroketext(const char* text) = 0;

        void pencolour(unsigned int r, unsigned int g, unsigned int b);

        //TODO: change width and height to unsigned int
        void canvas_size(int width, int height);

        virtual void export_img(const char *filename) = 0;

        void reset();
    };
};
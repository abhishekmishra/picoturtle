using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void spiral(Turtle t, double distance, 
                double angle, double pwidth, double distance_inc, 
                double angle_inc, double pwidth_inc) {
            int r = 0;
            int g = 0;
            int b = 255;
            t.pencolour(r, g, b);
            double x = distance;
            double a = angle;
            double pw = pwidth;
            for (int i = 0; i < 100; i++) {
                r += 2;
                g += 0;
                b -= 2;
                t.pencolour(r, g, b);
                t.penwidth(pw);
                t.forward(x);
                t.right(a);
                x += distance_inc;
                a += angle_inc;
                pw += pwidth_inc;
            }
        }

        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = picoturtle.Turtle.CreateTurtle(args);
            
            if (t != null) {
                // Your code goes here

                t.pendown();

                spiral(t, 1, 25, 1, 1, 0, 0.2);

                System.Console.WriteLine("Spiral done.");

                // Your code ends here

                // Always stop the turtle
                t.stop();
            }
            else {
                Console.Error.WriteLine("Error: Unable to create a turtle.");
                Environment.Exit(-1);
            }
        }
    }
}

using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void fern(Turtle t, double size, int sign) {
            if (size < 1) {
                return;
            }
            t.forward(size);

            t.right(70 * sign);
            fern(t, size/2, sign*-1);
            t.left(70 * sign);

            t.forward(size);

            t.left(70 * sign);
            fern(t, size/2, sign);
            t.right(70 * sign);

            t.right(7 * sign);
            fern(t, size - 2, sign);
            t.left(7 * sign);

            t.forward(-size);
            t.forward(-size);
        }

        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = picoturtle.Turtle.CreateTurtle(args);
            
            if (t != null) {
                // Your code goes here

                t.penup();
                t.back(200);

                t.right(90);
                t.back(20);
                t.pencolour(200, 200, 0);
                t.font("14pt Arial");
                t.filltext("Fern");
                t.forward(20);
                t.left(90);
                t.forward(50);

                t.pencolour(0, 128, 0);
                t.pendown();
                fern(t, 25, 1);


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

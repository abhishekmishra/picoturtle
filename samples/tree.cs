using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void tree(Turtle t, double size) {
            if (size < 4) {
                t.forward(size);
                t.forward(-size);
                return;
            }

            t.forward(size / 4);

            t.left(60);
            tree(t, size / 2);
            t.right(60);

            t.forward(size / 4);

            t.right(35);
            tree(t, size / 2);
            t.left(35);

            t.forward(size / 4);

            t.left(30);
            tree(t, size / 2);
            t.right(30);

            t.forward(size / 4);

            t.right(25);
            tree(t, size / 2);
            t.left(25);

            t.forward(-size);
        }

        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = picoturtle.Turtle.CreateTurtle(args);
            
            if (t != null) {
                // Your code goes here

                t.penup();
                t.back(100);
                t.pendown();
                t.pencolour(165, 42, 42);
                tree(t, 128);

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

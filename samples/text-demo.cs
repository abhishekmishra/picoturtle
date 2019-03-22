using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = picoturtle.Turtle.CreateTurtle(args);
            
            if (t != null) {
                // Your code goes here

                t.penup();
                t.back(250);
                t.right(90);
                t.forward(200);
                t.left(90);
                
                t.pendown();
                t.right(90);

                for(int i = 0; i < 40; i++) {
                    t.font("" + i + "pt Arial");
                    if(i%2 == 0) {
                        t.pencolour(255, 128, 128);
                        t.filltext("पीको टर्टल");
                    } else {
                        t.pencolour(255, 128, 255);
                        t.filltext("PicoTurtle");
                    }
                    t.penup();
                    t.left(90 + 2);
                    t.forward(i * 1.3);
                    t.right(90);
                    t.pendown();
                }

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

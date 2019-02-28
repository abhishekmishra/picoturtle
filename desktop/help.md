# PicoTurtle

PicoTurtle lets your create turtle graphics in the programming language of your choice.

You get an editor window on the left to write your program, and you get a canvas on the right to look at the results.
You also get a small window on the bottom right to look at any print statements from your program (or any errors).

A turtle is a simple painter which lives on a flat 2D world. When it moves it can paint on its world creating beautiful drawings. 
You control the turtle by writing a program which control its movements. 

To start writing a program choose a programming language.

Once you've chosen your programming language, you can use the following turtle commands in your program:
1. create_turtle -> This creates a new turtle (with a unique name, which is displayed in the statusbar). This command MUST be called before you start using the other commands.
1. penup -> Raise the pen so that the turtle does not write when it moves.
1. pendown -> Lower the pen so that the turtle writes when it moves.
1. forward (distance) -> move the turtle forward by the amount "distance" in pixels, in the direction it is facing.
1. left (angle) -> turn the turtle left by "angle" degrees.
1. right (angle) -> turn the turtle right by "angle" degrees.
1. penwidth (width) -> set the width of the pen to "width" pixels.
1. pencolour (red, blue, green) -> set the colour of the pen by providing "red", "blue" and "green" values each in the range 0-255.
1. clear -> clear the canvas.
1. stop -> stop the turtle. This MUST be called at the end of every program.
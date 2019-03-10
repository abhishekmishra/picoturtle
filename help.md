# PicoTurtle

PicoTurtle lets your create turtle graphics in the programming language of your choice.

You get an editor window on the left to write your program, and you get a canvas on the right to look at the results.
You also get a small window on the bottom right to look at any print statements from your program (or any errors).

A turtle is a simple painter which lives on a flat 2D world. When it moves it can paint on its world creating beautiful drawings. 
You control the turtle by writing a program which control its movements. 

To start writing a program choose a programming language.

Once you've chosen your programming language, you can use the following turtle commands in your program:
1. **create_turtle** -> This command creates a new turtle (with a unique name, which is displayed in the statusbar). The command MUST be called before you start using the turtle.
1. **penup** -> Raise the pen so that the turtle does not write when it moves.
1. **pendown** -> Lower the pen so that the turtle writes when it moves.
1. **forward (distance)** -> Move the turtle forward by the amount "distance" in pixels, in the direction it is facing.
1. **back (distance)** -> Move the turtle back by the amount "distance" in pixels, opposite to the direction it is facing.
1. **left (angle)** -> Turn the turtle left by "angle" degrees.
1. **right (angle)** -> Turn the turtle right by "angle" degrees.
1. **penwidth (width)** -> Set the width of the pen to "width" pixels.
1. **pencolour (red, blue, green)** -> Set the colour of the pen by providing "red", "blue" and "green" values each in the range 0-255.
1. **font (font string)** -> Set the font of the text to be drawn as a web font string. for e.g. '14pt Arial'.
1. **stroketext (text)** -> Draw the given text with a stroke (no fill), using the set font, in the direction the turtle is facing, at the turtle's location.
1. **filltext (text)** -> Draw the given text with fill, using the set font, in the direction the turtle is facing, at the turtle's location.
1. **clear** -> Clear the canvas.
1. **stop** -> Stop the turtle. This MUST be called at the end of every program.
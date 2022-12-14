---
title: PicoTurtle Docs
---

# PicoTurtle

PicoTurtle lets your create turtle graphics in the [Lua](https://lua.org) programming language.

## Application
You get an editor window on the left to write your program, and you get a canvas on the right to look at the results.
You also get a small window on the bottom right to look at any print statements from your program (or any errors).

## What is a Turtle?
A turtle is a simple painter which lives on a flat 2D world. When it moves it can paint on its world creating beautiful drawings. 
You control the turtle by writing a program which control its movements. 

# Turtle Commands

This section describes the Turtle API (application programmer's interface). The API is just a fancy name in this case for
the turtle commands available to you the programmer. This section describes how to create and use a turtle. Note that the
PicoTurtle turtle commands are written in the Lua programming language. This document does not teach you how to program in
lua.

## Turtle Creation

Since all PicoTurtle programs are Lua programs, you must create a new Lua file for every turtle program you write.
In the Lua program you must make sure to initialize the turtle before giving it commands.

### picoturtle.new

The PicoTurtle turtle is available in lua via the module `picoturtle`. This can be loaded using the standard
Lua `require` call

```
local picoturtle = require 'picoturtle'
```

Once you have loaded the `picoturtle` module, you can create a turtle object. We give it a name `t` - a single character
because you have to type this many times in a turtle program.

```
local t = picoturtle.new()
```

If you are running your PicoTurtle commands in the PicoTurtle GUI a `t` turtle object is already created for you in the Lua session. 
You can write commands to this turtle object without creating a new one.

Finally, if you wish to write a program which can run in GUI and from the PicoTurtle command-line,
you can use the following snippet, which creates a new turtle object only if does not exist.

```
local t = t or require 'picoturtle'.new()
```

## Turtle State Commands
1. **clear** -> Clear the canvas.
1. **stop** -> Stop the turtle. This MUST be called at the end of every program.
1. **home** -> Return the turtle to its starting position and heading.
1. **state** -> Return the current state of turtle.

## Turtle Pen Commands
1. **penup** -> Raise the pen so that the turtle does not write when it moves.
1. **pendown** -> Lower the pen so that the turtle writes when it moves.
1. **penwidth (width)** -> Set the width of the pen to "width" pixels.
1. **pencolour (red, blue, green)** -> Set the colour of the pen by providing "red", "blue" and "green" values each in the range 0-255.

## Turtle Text Commands
1. **font (font string)** -> Set the font of the text to be drawn as a web font string. for e.g. '14pt Arial'.
1. **stroketext (text)** -> Draw the given text with a stroke (no fill), using the set font, in the direction the turtle is facing, at the turtle's location.
1. **filltext (text)** -> Draw the given text with fill, using the set font, in the direction the turtle is facing, at the turtle's location.

## Turtle Relative Movement Commands
1. **forward (distance)** -> Move the turtle forward by the amount "distance" in pixels, in the direction it is facing.
1. **back (distance)** -> Move the turtle back by the amount "distance" in pixels, opposite to the direction it is facing.
1. **left (angle)** -> Turn the turtle left by "angle" degrees.
1. **right (angle)** -> Turn the turtle right by "angle" degrees.

## Turtle Absolute Movement Commands
1. **goto (x,y)** -> Called **setpos** in languages where goto is a keyword. Move the turtle to (x,y) on the canvas.
1. **setx (x)** -> Move the turtle along the x axis, keeping y to it's current value.
1. **sety (y)** -> Move the turtle along the y axis, keeping x to it's current value.
1. **heading (angle)** -> Change the heading of the turtle to "angle" degrees.

## Turtle Canvas Commands
1. **canvas_size (width, height)** -> Change the canvas size to [width X height] pixels.
1. **export_img (filename)** -> Export the current canvas as a PNG file to the given path.
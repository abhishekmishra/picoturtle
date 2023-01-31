---
title: Turtle API
---

# PicoTurtle API

PicoTurtle lets your create turtle graphics in the [Lua](https://lua.org) programming language. This document discusses the API exposed by PicoTurtle to facilitate turtle graphics.

## What is a Turtle?
A turtle is a simple painter which lives on a flat 2D world. When it moves it can paint on its world creating beautiful drawings. 
You control the turtle by writing a program which control its movements. 

## Turtle Commands

This section describes the Turtle API (application programmer's interface). The API is just a fancy name in this case for
the turtle commands available to you the programmer. This section describes how to create and use a turtle. Note that the
PicoTurtle turtle commands are written in the Lua programming language. This document does not teach you how to program in
lua.

### Turtle Creation

Since all PicoTurtle programs are Lua programs, you must create a new Lua file for every turtle program you write.
In the Lua program you must make sure to initialize the turtle before giving it commands.

####  picoturtle.new

The PicoTurtle turtle is available in lua via the module `picoturtle`. This can be loaded using the standard
Lua `require` call

```lua
local picoturtle = require 'picoturtle'
```

Once you have loaded the `picoturtle` module, you can create a turtle object. We give it a name `t` - a single character
because you have to type this many times in a turtle program.

```lua
local t = picoturtle.new()
```

If you are running your PicoTurtle commands in the PicoTurtle GUI a `t` turtle object is already created for you in the Lua session. 
You can write commands to this turtle object without creating a new one.

Finally, if you wish to write a program which can run in GUI and from the PicoTurtle command-line,
you can use the following snippet, which creates a new turtle object only if does not exist.

```lua
local t = t or require 'picoturtle'.new()
```
###  Calling picoturtle commands on a turtle

Once you have a Lua turtle object - lets say named `t`, you can issue commands to the turtle object.
The lua syntax for calling a method on an object is `<objectname>:<method>(<args>)`; i.e. object
name followed by a colon followed by method name and then the arguments in parenthesis.

There for to call a method/command named `foo` with a numeric argument,  on turtle `t` you can write
`t:foo(10)`.

The subsequent sections list the various commands available.

## Turtle State Commands

### clear
Clears the canvas by painting it in white. This will wipe out everything that's been drawn on the canvas. This command is helpful in starting over again.

Example:
```lua
t:clear()
```

### home
Return the turtle to its starting position and heading. The starting position is at the center of the canvas, and with a heading of 90 degrees facing upwards.

Example:
```lua
t:home()
```

### state
Returns the current state of turtle. This is helpful in figuring out where the turtle is at the moment, what is its heading and what are its pen settings.

Example:
```lua
--- print the current turtle state at the console
print(t:state())

--- this command will display something like this on the console
--- Turtle state [loc=(512.0, 512.0), col=(black, 128, 64, 32), pen(down=1, width=1.0)
--- note that loc stands for location, col for color and pen shows pen settings
--- the loc/location is the pair (x, y) coordinate of the current turtle position
--- the col/color shows the color name if available and the rgb values for the color
--- the pen shows whether the turtle has its pen pressed on the canvas, and what is the pen's width
```

## Turtle Pen Commands

### penup
Raise the pen so that the turtle does not write when it moves.

Example:

```lua
t:reset()         -- will set pen down for writing, among other things
t:forward(100)    -- move 100 px while writing with the current pen
t:penup()         -- lift the pen
t:forward(100)    -- move 100 px without writing
```

### pendown
Lower the pen so that the turtle writes when it moves.

Example:

```lua
t:pendown()       -- will set pen down for writing
t:forward(100)    -- move 100 px while writing with the current pen
```

### penwidth (width)
Set the width of the pen to "width" pixels.

Example:

```lua
t:penwidth(10)    -- will set pen thickness/width to 10px
t:forward(100)    -- move 100 px while writing with the current pen
```

### pencolour (color name) / (red, green, blue)
Set the colour of the pen by providing 
* either the color name of the pen. 
* or "red", "green" and "blue" values each in the range 0-255.

Example:

```lua
t:pencolor('red')     -- will set pen color to red
t:forward(100)        -- move 100 px while writing with the current pen

t:pencolor(0, 255, 0) -- will set pen color to green
t:forward(100)        -- move 100 px while writing with the current pen
```

## Turtle Text Commands

### font (font name, font size)
Set the font of the text to be drawn by providing a font name and a size in pixels.

Example:

```lua
t:font('Arial', 14)
```

### stroketext (text)
Draw the given text with a stroke (no fill), using the set font, in the direction the turtle is facing, at the turtle's location.

__Note__: At the moment filltext and stroketext do the same thing - filltext

Example:

```lua
t:heading(0)
t:stroketext("Hello, World!")
```

### filltext (text)
Draw the given text with fill, using the set font, in the direction the turtle is facing, at the turtle's location.

__Note__: At the moment filltext and stroketext do the same thing - filltext

Example:

```lua
t:heading(0)
t:filltext("Hello, World!")
```

## Turtle Relative Movement Commands

### forward/fd (distance)
Move the turtle forward by the amount "distance" in pixels, in the direction it is facing.

Example:

```lua
t:forward(100)  -- move the turtle 100 pixels in the direction it is facing
t:fd(100)       -- fd is alias for forward so does the same thing as previous line
```

### back (distance)
Move the turtle back by the amount "distance" in pixels, opposite to the direction it is facing.

Example:

```lua
t:back(100)     -- move the turtle 100 pixels in the opposite direction to the one it is facing
t:bk(100)       -- bk is alias for back so does the same thing as previous line
```

### left (angle)
Turn the turtle left by "angle" degrees.

Example:

```lua
t:left(90)     -- turn the heading (direction turtle is facing) left (counter-clockwise) by 90 degrees
t:lt(90)       -- lt is alias for left so does the same thing as previous line
```

### right (angle)
Turn the turtle right by "angle" degrees.

Example:

```lua
t:right(90)     -- turn the heading (direction turtle is facing) right (clockwise) by 90 degrees
t:rt(90)        -- rt is alias for right so does the same thing as previous line
```

## Turtle Absolute Movement Commands

### setpos (x,y)
Move the turtle to (x,y) on the canvas.

Example:

```lua
t:setpos(0, 0)   -- move the turtle to the bottom left of the canvas
```

### setx (x)
Move the turtle along the x axis, keeping y to it's current value.

Example:

```lua
t:setx(0)   -- if turtle is at (x, y) move the turtle to (0, y)
```

### sety (y)
Move the turtle along the y axis, keeping x to it's current value.

Example:

```lua
t:sety(0)   -- if turtle is at (x, y) move the turtle to (x, 0)
```

### heading (angle)
Change the heading of the turtle to "angle" degrees.

Example:

```lua
t:heading(90) -- turn the turtle to face upwards on the canvas.
t:heading(0) -- turn the heading to face right on the canvas.
```

## Turtle Canvas Commands

### canvas_size (width, height) | ()
This method accepts two argument or none at all.
In the two arguments mode it accepts two numeric arguments width and height, and changes the canvas size to [width X height] pixels.
In the single argument mode - no change is made to the canvas.

However in both forms of the command the current width and height of the canvas is returned as two return values.

__Note:__ Lua function calls can have multiple return values.

Example:

```lua
local w
local h

w, h = t:canvas_size(100, 200)

print(w .. ', ' .. h) -- should print 100, 200 as that is the new canvas size returned by the method.
```

### export_img (filename)
Export the current canvas as a PNG file to the given path.

Example:

```lua
t:export_img("C:\\tmp\\turtle.png") -- write the current canvas contents as a png file.
```

### clear (color) | (red, green, blue) | ()
This command has 3 argument modes:
1. **No arguments:** The entire canvas is painted the default clear color which is __white__.
2. **One argument (color)** The argument is a color name - which if part of the color-names list then the canvas is painted with this color.
3. **Three argumens (r, g, b)** The three numeric arguments in the range [0-255] specify an RGB triple and thus a color. The canvas is painted with this color.

__Note:__ this command overwrites everything on the canvas

```lua
t:clear()                      -- clear canvas with white
t:clear('red')                 -- clear canvas with red color
t:clear(0, 255, 0)             -- clear canvas with (0, 255, 0) i.e. green color
```

## Turtle Shape Commands

### circle (radius)
This command draws a circle around the current position of the turtle. The circle uses the turtle's current postion as center and provided radius.

Example:

```lua
t:circle(100)
```

### arc (radius, [extent, steps])
This command draws an arc starting at the current position of the turtle. The radius is used to set the curvature of the arc (the radius of the circle which this arc is part of). The extent is provided in degrees and determines the size of the arc. The arc is constructed using small lines that approximate the arc. The number of lines used is set using steps. If not provided, the steps are calculated internally.

Example:

```lua
t:arc(100, 90)    -- draw a 90degree arc with radius 100

t:arc(10)         -- draw a circle with radius 10

t:arc(50, 360, 6) -- draw a hexagon
```

## Other Commands

### stop
Stop the turtle.

Example:
```lua
t:stop()
```

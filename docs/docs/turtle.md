---
title: Turtle Graphics
---

# Introduction to Turtle Graphics

Turtle Graphics is one of the simplest ways to draw vector graphics on a canvas. 
It's really simple to grasp and has very few fundamental operations. 
You (the programmer) are provided a turtle and a canvas. The turtle
holds a pen - which can write on the canvas as the turtle moves.
It is commonly used as an introduction to programming for kids because of this
simplicity. 

Inspite of being simple, turtle graphics is very powerful. An infinite variety
of 2d-graphics are possible using turtle graphics. And thus it presents a
powerful mechanism of exploring generative art. Turtle graphics is also a
unique way of exploring 2D space without the use of Cartesian geometry [[1]](#1).

## Brief History

Turtle Graphics were introduced in the 1960s as part of the LOGO programming
language. Read more at Wikipedia [[2]](#2)

## Why Learn Turtle Graphics

As mentioned in the introduction - turtle graphics is a powerful but simple
means to draw on an abstract canvas. One can explore programming, maths, and
art using the same tool. I've been exploring these fields on my own too and
therefore I felt like creating this tool. However you could have your own
valid reason you are here - which I haven't heard of before :smile:.

## What Is A Turtle?

* A _Turtle_ lives and moves in a 2-dimensional world called the _Canvas_.
* The turtle can move freely on the canvas using turn and move commands. 
* The turtle carries a _Pen_ which can draw on the canvas. While
* The turtle moves it can write on the Canvas if the pen is _down_

## How-to Draw With A Turtle

To draw with a turtle one must give it commands. In any turtle graphics
environment you are provided a place to enter commands and another place to
look at the canvas with the turtle in it. Once you enter commands, you can
press the run (or equivalent) button and then the turtle must act according
to the given commands.

A turtle has the following types of commands typically:
1. **Movement commands**: To turn and move around the canvas for e.g. 
   `right(deg)` turns a turtle by the given degrees, and `forward(dist)`
   will move the turtle by the given distance in pixels.
2. **Drawing/Pen commands**: The turtle's pen can be picked up or put down to
   draw and the pen color and pen's line width can be changed. A few examples
   are `penup()` which pulls up the pen from the canvas, `pencolor(color)`
   that changes the pen's ink color to the given color.
3. **State Commands**: One can query the state of the turtle - for example its
   current position, pen color etc.
4. **Canvas Commands**: One can change the canvas settings - for example its
   size.

## Example Turtle Programs

Here's some pseudo-code of a few really simple turtle programs. For a larger
list of programs that are written in PicoTurtle see the 
[Samples](/Samples) page.

### Draw a Square

This pseudocode draws a square with sides 100 pixels in black pen with width 1
pixel.

```
pendown()
penwidth(1)
pencolor('black')

repeat 4 times:
    right(90)
    forward(100)

```

### Draw a Circle

This program draws a circle with circumference 360px and radius 360/2π. It is
also drawn in black with the pen width of 1 pixel.

```
pendown()
penwidth(1)
pencolor('black')

repeat 360 times:
    right(1)
    forward(1)
```
## Further Reading

## References

<a id="1">[1]</a> 
Abelson, H.; DiSessa, A. A. 
Turtle Geometry: The computer as a medium for exploring mathematics; 
MIT Press: Cambridge, 1992. 

<a id="2">[2]</a>
https://en.wikipedia.org/wiki/Turtle_graphics#History
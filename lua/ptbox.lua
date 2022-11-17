--[[--
Utility classes and functions to divide the Turtle's canvas into boxes (rectangular areas), and then restrict the Turtle to the rectangular areas.

Defines two classes, Box and BoxTurtle.
- The Box class provides a way to define a rectangular box inside a canvas.
- The BoxTurtle class provides a Turtle object with all the methods of a regular PicoTurtle, but which now lives inside the specified Box.

date  : 15/11/2022
	
@author: Abhishek Mishra
--]]--

local class = require 'middleclass'

---Vec2 defines a 2D vector
--The components of the vector are available with various names.
--
-- * first component: x(), w()
-- * second component: y(), h()
--
-- @type Vec2
local Vec2 = class('Vec2')

--- Construct a new 2d vector with given numeric components
--
-- @tparam number x first component
-- @tparam nubmer y second component
function Vec2:initialize(x, y)
	self._x = x or 0
	self._y = y or 0
end

--- first component
function Vec2:x()
	return self._x
end

--- second component
function Vec2:y()
	return self._y
end

--- first component
function Vec2:w()
	return self._x
end

--- first component
function Vec2:h()
	return self._y
end

--- The Box class defines a rectangular area of the canvas.
-- It will also provide a way to situate a point in the coordinate system
-- local to the Box.
-- 
-- A Box instance will have an origin at the bottom left of the rectangular 
-- area, and a given width and height.
-- 
-- @type Box
--
local Box = class('Box')

--- Construct a box with a given origin (bottom-left) and dimensions (width x height) in Canvas coordinates.
-- 
-- @tparam Vec2 orig bottom-left corner of the box in the canvas coordinates
-- @tparam Vec2 dim dimensions (width x height) of the box in the canvas coordinates
function Box:initialize(orig, dim)
	self.c_orig = orig or Vec2:new(0, 0)
	self.c_dim = dim or Vec2:new(100, 100)
end

--- Get the origin (bottom-left) of the box in canvas coords
--
-- @return Vec2 origin in canvas coords
function Box:canvas_orig()
	return self.c_orig
end

--- Get the dimensions (w x h) of the box in canvas coords
--
-- @return Vec2 dimensions in canvas coords
function Box:canvas_dim()
	return self.c_dim
end

--- Restrict a Turtle to a Box in the canvas.
-- The BoxTurtle takes a PicoTurtle instance and a Box and creates
-- a new turtle which has all the methods of the turtle - but restricted to
-- the rectangular area specified by the box.
-- 
-- This allows multiple turtles to use different areas on the same
-- canvas using different boxes.
-- 
-- The BoxTurtle does the requisite coordinate transforms
-- for making sure the turtle's movements are in the coordinate
-- system of the box, not the canvas.
-- 
-- At the movement rotated and scaled Boxes are not supported,
-- therefore a Box is just translated in space. Therefore only the
-- absolute movement commands are translated as appropriate. These
-- commands are reset() and setpos()
--
-- @type BoxTurtle
local BoxTurtle = class('BoxTurtle')

--- Initialize a BoxTurtle with the given turtle and given box.
--
-- @tparam PicoTurtle turtle
-- @tparam Box box
function BoxTurtle:initialize(turtle, box)
	self.turtle = turtle or require'picoturtle'.new()
	self.box = box or Box:new()
end

--- Clear the Box (fill with white)
function BoxTurtle:clear()
	print('clear this dude')
	local orig = self.box:canvas_orig()
	print(orig:x() .. ', ' .. orig:y())
	local dim = self.box:canvas_dim()
	local pos = Vec2:new(orig:x() + (dim:w()/2), orig:y())
	self.turtle:penup()
	self.turtle:setpos(pos:x(), pos:y())
	self.turtle:heading(90)
	self.turtle:penwidth(dim:w())
	self.turtle:pencolour(128, 128, 128)
	self.turtle:pendown()
	self.turtle:fd(dim:h())
end

--- Stop the turtle
function BoxTurtle:stop()
	self.turtle:stop()
end

--- Return the turtle to its starting position and heading
function BoxTurtle:home()
	local orig = self.box:canvas_orig()
	local dim = self.box:canvas_dim()
	self.turtle:setpos(orig:x() + (dim:w()/2), orig:y() + (dim:h()/2))
	self.turtle:heading(90)
end

--- Raise the pen so that the turtle does not write when it moves.
function BoxTurtle:penup()
	self.turtle:penup()
end

--- Lower the pen so that the turtle writes when it moves.
function BoxTurtle:pendown()
	self.turtle:pendown()
end

--- Set the width of the pen to "width" pixels.
-- @tparam number width
function BoxTurtle:penwidth(width)
	self.turtle:penwidth(width)
end

--- Set the colour of the pen by providing "red", "green" and "blue" values in the range of 0-255.
-- @tparam number red
-- @tparam number green
-- @tparam number blue
function BoxTurtle:pencolour(red, green, blue)
	self.turtle:pencolour(red, green, blue)
end

--- Move the turtle forward by "dist" pixels.
-- @tparam number dist
function BoxTurtle:fd(dist)
	self.turtle:fd(dist)
end

--- Set the font of the text to be drawn as a font string
-- @tparam string font
function BoxTurtle:font(font_string)
	self.turtle:font(font_string)
end

--- Draw the text with a stroke (no fill), using the set font, in the direction the turtle is facing, at the turtle's location.
-- @tparam string text
function BoxTurtle:stroketext(text)
	self.turtle:stroketext(text)
end

--- Draw the given text with fill, using the set font, in the direction the turtle is facing, at the turtle's location
-- @tparam string text
function BoxTurtle:filltext(text)
	self.turtle:filltext(text)
end

--- Move the turtle forward by "dist" pixels.
-- @tparam number dist
function BoxTurtle:forward(dist)
	self.turtle:fd(dist)
end

--- Move the turtle back by "dist" pixels.
-- @tparam number dist
function BoxTurtle:back(dist)
	self.turtle:back(dist)
end

--- Turn the turtle left by "angle" degrees.
-- @tparam number angle
function BoxTurtle:left(angle)
	self.turtle:left(angle)
end

--- Turn the turtle right by "angle" degrees.
-- @tparam number angle
function BoxTurtle:right(angle)
	self.turtle:right(angle)
end


--- Turn the turtle left by "angle" degrees.
-- @tparam number angle
function BoxTurtle:lt(angle)
	self.turtle:left(angle)
end

--- Turn the turtle right by "angle" degrees.
-- @tparam number angle
function BoxTurtle:rt(angle)
	self.turtle:right(angle)
end

--- Move the turtle to (x, y) in the box.
-- @tparam number x
-- @tparam number y
function BoxTurtle:setpos(x, y)
	local orig = self.box:canvas_orig()
	local dim = self.box:canvas_dim()
	-- TODO: check that the position is within bounds
	-- otherwise throw an error.
	self.turtle:setpos(orig:x() + x, orig:y() + y)
end

--- Move the turtle along the x axis, keeping y to its current value.
-- @taparam number x
function BoxTurtle:setx(x)
	local orig = self.box:canvas_orig()
	local dim = self.box:canvas_dim()
	self.turtle:setx(orig:x() + x)
end

--- Move the turtle along the y axis, keeping x to its current value.
-- @tparam number y
function BoxTurtle:sety(y)
	local orig = self.box:canvas_orig()
	local dim = self.box:canvas_dim()
	self.turtle:sety(orig:y() + y)
end

--- Change the heading of the turtle to "angle" degrees.
-- @tparam number angle in degrees
function BoxTurtle:heading(angle)
	self.turtle:heading(angle)
end

--- Change the box size to [width x height]
-- @tparam number width
-- @tparam number height
function BoxTurtle:box_size(width, height)
	error('not implemented yet')
end

--- Export the current box as a PNG file to the given path
-- @tparam string filename
function BoxTurtle:export_img(filename)
	-- TODO: proper implementation of exporting only the box.
	error('not implemented yet')
end
 
--- @export
return {
	Box = Box,
	BoxTurtle = BoxTurtle,
	Vec2 = Vec2,
}
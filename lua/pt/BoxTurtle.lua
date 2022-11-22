--- BoxTurtle changes the coord system of a turtle to a a rectangular area on the canvas.
-- Allows use of multiple turtles on the same canvas but in different boxed areas.
-- @modclass

local class = require 'middleclass'
local Vec2 = require'pt/Vec2'
local Box = require'pt/Box'

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
	local dim = self.box:dim()
	local pos = self.box:to_parent_coords(dim:w()/2, 0)
	-- print('going to ' .. pos:x() .. ', ' .. pos:y())
	self.turtle:penup()
	self.turtle:setpos(pos:x(), pos:y())
	self.turtle:heading(90)
	self.turtle:penwidth(dim:w())
	self.turtle:pencolor(128, 128, 128)
	self.turtle:pendown()
	self.turtle:fd(dim:h())
	self.turtle:penup()
end

--- Stop the turtle
function BoxTurtle:stop()
	self.turtle:stop()
end

--- Return the turtle to its starting position and heading
function BoxTurtle:home()
	local half_dim = self.box:dim():scale(0.5)
	local newpos = self.box:to_parent_coords(half_dim)
	self.turtle:setpos(newpos:x(), newpos:y())
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
function BoxTurtle:pencolor(red, green, blue)
	if type(red) == 'string' then
		self.turtle:pencolor(red)
	else
		self.turtle:pencolor(red, green, blue)
	end
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
	-- TODO: check that the position is within bounds
	-- otherwise throw an error.
	self.turtle:setpos(self.box:to_parent_coords(x, y))
end

--- Move the turtle along the x axis, keeping y to its current value.
-- @taparam number x
function BoxTurtle:setx(x)
	self.turtle:setx(self.box:to_parent_coords(x, 0))
end

--- Move the turtle along the y axis, keeping x to its current value.
-- @tparam number y
function BoxTurtle:sety(y)
	self.turtle:sety(self.box:to_parent_coords(y, 0))
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

--- tostring
function BoxTurtle:__tostring()
	return "BoxTurtle -> " .. tostring(self.box)
end

function BoxTurtle:draw_border()
	self.box:draw_border(self.turtle)
end

return BoxTurtle
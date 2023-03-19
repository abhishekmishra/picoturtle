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
-- If *check_bounds is true* then the BoxTurtle will check if the
-- relative and absolute movement commands take the turtle out
-- of the Box - and raise an error if it does.
--
-- @tparam PicoTurtle turtle (creates a new turtle if nil)
-- @tparam Box box (creates a new box if nil)
-- @tparam boolean check_bounds (default is false)
function BoxTurtle:initialize(turtle, box, check_bounds)
	self.turtle = turtle or require'picoturtle'.new()
	self.box = box or Box:new()
	self.check_bounds = check_bounds or false
end

-- TODO this should return a state with box coords
--- Get the turtle state
--
-- @tparam State the detached turtle state
function BoxTurtle:state()
	local state = self.turtle:state()
	return state
end

--- Clear the Box (fill with color)
--
-- @tparam color the color name
-- @tparam rest if color is r, then g and b can be supplied.
function BoxTurtle:clear(color, ...)
	local color = color or 'white'
	local dim = self.box:dim()
	local pos = self.box:to_parent_coords(dim:w()/2, 0)
	-- print('going to ' .. pos:x() .. ', ' .. pos:y())
	self.turtle:penup()
	self.turtle:setpos(pos:x(), pos:y())
	self.turtle:heading(90)
	self.turtle:penwidth(dim:w())
	self.turtle:pencolor(color, ...)
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
-- @treturn boolean whether forward was successful
function BoxTurtle:fd(dist)
	if self.check_bounds then
		-- save the turtle state before trial move
		self.turtle:save()
		local state = self.turtle:state()

		self.penup(self)
		self.turtle:fd(dist)

		local new_state = self.turtle:state()
		local new_pos = self.box:to_box_coords(new_state:x(), new_state:y())
		local oob = self.box:out_of_bounds(new_pos)

		-- restore turtle
		self.turtle:restore()

		if oob then
			return false
		else
			self.turtle:fd(dist)
			return true
		end
	else
		self.turtle:fd(dist)
		return true
	end
end

--- Move the turtle back by "dist" pixels.
-- @tparam number dist
-- @treturn boolean whether back was successful
function BoxTurtle:bk(dist)
	if self.check_bounds then
		-- save the turtle state before trial move
		self.turtle:save()
		local state = self.turtle:state()

		self.penup(self)
		self.turtle:bk(dist)

		local new_state = self.turtle:state()
		local new_pos = self.box:to_box_coords(new_state:x(), new_state:y())
		local oob = self.box:out_of_bounds(new_pos)

		-- restore turtle
		self.turtle:restore()

		if oob then
			return false
		else
			self.turtle:bk(dist)
			return true
		end
	else
		self.turtle:bk(dist)
		return true
	end
end

--- Set the font of the text to be drawn as a font string
-- @tparam string font
function BoxTurtle:font(font_string, ...)
	self.turtle:font(font_string, ...)
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
-- @treturn boolean whether forward moved out of bounds
function BoxTurtle:forward(dist)
	return self.fd(self, dist)
end

--- Move the turtle back by "dist" pixels.
-- @tparam number dist
-- @treturn boolean whether back was successful
function BoxTurtle:back(dist)
	return self.bk(self, dist)
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
	local coords = self.box:to_parent_coords(x, y)
	self.turtle:setpos(coords:x(), coords:y())
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
-- @return current box size
function BoxTurtle:box_size(width, height)
	if(width ~= nil and height ~= nil) then
		error('setting widthxheight is not implemented yet')
	else
		local sz = self.box:dim()
		return sz:x(), sz:y()
	end
end

function BoxTurtle:canvas_size(width, height)
	return self:box_size(width, height)
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

--- Draw a border around the given Box
-- This will only work if border width of the Box
-- is non-zero.
function BoxTurtle:draw_border()
	self.box:draw_border(self.turtle)
end

--- Test if the current position of the turtle is
-- out of bounds of the current box.
--
-- @treturn boolean flag indicating whether turtle is out of bounds
function BoxTurtle:out_of_bounds()
	local state = self.turtle:state()
	local box_coords = self.box:to_box_coords(state:x(), state:y())
	local oob = self.box:out_of_bounds(box_coords)
	return oob
end

function BoxTurtle:getpos()
	local state = self.turtle:state()
	local box_coords = self.box:to_box_coords(state:x(), state:y())
	return box_coords:x(), box_coords:y()
end

function BoxTurtle:save()
	self.turtle:save()
end

function BoxTurtle:restore()
	self.turtle:restore()
end

function BoxTurtle:circle(radius)
	self.turtle:circle(radius)
end

return BoxTurtle
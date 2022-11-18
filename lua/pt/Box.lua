--- Box defines an abstract rectangular area on the canvas
-- used to create a smaller rectangular and run a turtle program in that area
-- @modclass

local class = require 'middleclass'

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

--- tostring
function Box:__tostring()
	return "Box [orig=" .. tostring(self.c_orig) .. ", dim=" .. tostring(self.c_dim) .. "]"
end

return Box
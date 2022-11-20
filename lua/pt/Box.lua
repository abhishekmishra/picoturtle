--- Box defines an abstract rectangular area on the canvas
-- used to create a smaller rectangular and run a turtle program in that area
-- @modclass

local class = require 'middleclass'
local Vec2 = require 'pt/Vec2'
local Vec4 = require 'pt/Vec4'

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
	self.border_width = Vec4:new()
	self.margin_width = Vec4:new()
	self.padding_width = Vec4:new()
	self.update_internal_coords(self)
end

function Box:update_internal_coords()
	self.d_orig = self.c_orig:add(
		Vec2:new(
			self.border_width:w() + self.margin_width:w() + self.padding_width:w(),
			self.border_width:s() + self.margin_width:s() + self.padding_width:s()
		)
	)
	self.d_dim = self.c_dim:subtract(
		Vec2:new(
			self.border_width:w() + self.margin_width:w() + self.padding_width:w() +
			self.border_width:e() + self.margin_width:e() + self.padding_width:e(),
			self.border_width:s() + self.margin_width:s() + self.padding_width:s() +
			self.border_width:n() + self.margin_width:n() + self.padding_width:n()
		)
	)
end

--- Get the origin (bottom-left) of the box in canvas coords
--
-- @return Vec2 origin in canvas coords
function Box:canvas_orig()
	return self.c_orig
end

--- Get the outer dimensions (w x h) of the box in canvas coords
--
-- @return Vec2 dimensions in canvas coords
function Box:canvas_dim()
	return self.c_dim
end

--- Get the dimensions (w x h) of the inner drawable area of the box
--
-- @return Vec2 dim drawable area dimensions
function Box:dim()
	return self.d_dim
end

--- Set the border width (n, e, w, s)
--
-- @tparam number n north border, (sets all borders if the rest of the params are nil)
-- @tparam number e east border
-- @tparam number w west border
-- @tparam number s south border
function Box:set_border_width(n, e, w, s)
	if n ~= nil and e == nil and w == nil and s == nil then
		self.border_width = Vec4:new(n, n, n, n)
	else
		local n = n or 0
		local e = e or 0
		local w = w or 0
		local s = s or 0
		self.border_width = Vec4:new(n, e, w, s)
	end
	self.update_internal_coords(self)
end

--- Set the margin width (n, e, w, s)
--
-- @tparam number n north margin, (sets all margins if the rest of the params are nil)
-- @tparam number e east margin
-- @tparam number w west margin
-- @tparam number s south margin
function Box:set_margin_width(n, e, w, s)
	if n ~= nil and e == nil and w == nil and s == nil then
		self.margin_width = Vec4:new(n, n, n, n)
	else
		local n = n or 0
		local e = e or 0
		local w = w or 0
		local s = s or 0
		self.margin_width = Vec4:new(n, e, w, s)
	end
	self.update_internal_coords(self)
end

--- Set the padding width (n, e, w, s)
--
-- @tparam number n north padding, (sets all paddings if the rest of the params are nil)
-- @tparam number e east padding
-- @tparam number w west padding
-- @tparam number s south padding
function Box:set_padding_width(n, e, w, s)
	if n ~= nil and e == nil and w == nil and s == nil then
		self.padding_width = Vec4:new(n, n, n, n)
	else
		local n = n or 0
		local e = e or 0
		local w = w or 0
		local s = s or 0
		self.padding_width = Vec4:new(n, e, w, s)
	end
	self.update_internal_coords(self)
end

--- Draw border using the canvas turtle object
-- (desired colour of the border should be set beforehand on
-- the turtle pen.)
-- Width of the pen will be set by this method.
--
-- @tparam turtle t
function Box:draw_border(t)
	self.draw_west_border(self, t)
	self.draw_north_border(self, t)
	self.draw_east_border(self, t)
	self.draw_south_border(self, t)
end

--- Draw the west border of the box
--
-- @tparam turtle t
function Box:draw_west_border(t)
	t:penup()
	t:penwidth(self.border_width:w())
	local pos = self.c_orig:add(Vec2:new(self.margin_width:w() + (self.border_width:w()/2), self.margin_width:s()))
	t:setpos(pos:x(), pos:y())
	t:heading(90)
	t:pendown()
	t:forward(self.c_dim:h() - self.margin_width:s() - self.margin_width:n())
	t:penup()
end

--- Draw the north border of the box
--
-- @tparam turtle t
function Box:draw_north_border(t)
	t:penup()
	t:penwidth(self.border_width:n())
	pos = self.c_orig:add(Vec2:new(self.margin_width:w(), self.c_dim:w() - self.margin_width:n() - (self.border_width:n()/2)))
	t:setpos(pos:x(), pos:y())
	t:heading(0)
	t:pendown()
	t:forward(self.c_dim:w() - self.margin_width:w() - self.margin_width:e())
	t:penup()
end

--- Draw the east border of the box
--
-- @tparam turtle t
function Box:draw_east_border(t)
	t:penup()
	t:penwidth(self.border_width:e())
	pos = self.c_orig:add(Vec2:new(self.c_dim:w() - self.margin_width:e() - (self.border_width:e()/2), self.c_dim:h() - self.margin_width:n()))
	t:setpos(pos:x(), pos:y())
	t:heading(-90)
	t:pendown()
	t:forward(self.c_dim:h() - self.margin_width:n() - self.margin_width:s())
	t:penup()
end

--- Draw the south border of the box
--
-- @tparam turtle t
function Box:draw_south_border(t)
	t:penup()
	t:penwidth(self.border_width:s())
	pos = self.c_orig:add(Vec2:new(self.c_dim:w() - self.margin_width:e(), self.margin_width:s() +  (self.border_width:s()/2)))
	t:setpos(pos:x(), pos:y())
	t:heading(180)
	t:pendown()
	t:forward(self.c_dim:w() - self.margin_width:w() - self.margin_width:e())
	t:penup()
end

--- Convert the given box coords to the corresponding container coords
--
-- @tparam number|Vec2 x either the x coord or a Vec2
-- @tparam number y y coord
-- @treturn Vec2 result coordinates
function Box:to_parent_coords(x, y)
	if type(x) == "table" and x.class == Vec2 then
		return self.d_orig:add(x)
	else
		return self.d_orig:add(Vec2:new(x, y))
	end
end

--- tostring
function Box:__tostring()
	return "Box [orig=" .. tostring(self.c_orig) .. ", dim=" .. tostring(self.c_dim) .. "]"
end

return Box

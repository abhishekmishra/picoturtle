--- Vec2 is a 2D vector class
-- useful for using as points or dimensions in the turtle space.
-- @modclass
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

--- tostring
function Vec2:__tostring()
	return "Vec2(" .. self._x .. "," .. self._y .. ")"
end

return Vec2
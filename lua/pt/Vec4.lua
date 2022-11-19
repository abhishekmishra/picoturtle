--- Vec4 is a 4D vector class
-- useful for using as values of four directions - margin, border, padding etc.
-- @modclass
local class = require 'middleclass'

---Vec4 defines a 4D vector
--The components of the vector are available with various names.
--
-- * first component: a(), w()
-- * second component: y(), h()
--
-- @type Vec4
local Vec4 = class('Vec4')

--- Construct a new 4d vector with given numeric components
--
-- @tparam number a first component
-- @tparam nubmer b second component
-- @tparam nubmer c third component
-- @tparam nubmer d fourth component
function Vec4:initialize(a, b, c, d)
	self._a = a or 0
	self._b = b or 0
	self._c = c or 0
	self._d = d or 0
end

--- first component
function Vec4:a()
	return self._a
end

--- second component
function Vec4:b()
	return self._b
end

--- third component
function Vec4:c()
	return self._c
end

--- fourth component
function Vec4:d()
	return self._d
end

--- first component
function Vec4:n()
	return self._a
end

--- second component
function Vec4:e()
	return self._b
end

--- third component
function Vec4:w()
	return self._c
end

--- fourth component
function Vec4:s()
	return self._d
end

--- tostring
function Vec4:__tostring()
	return "Vec4(" .. self._a .. "," .. self._b .. "," .. self._c .. "," .. self._d")"
end

return Vec4
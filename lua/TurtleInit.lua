-- ------Turtle Lua Init Started ------
-- If you can see this, lua print is redirected to the Turtle Console.

local _turtle_functions_names = {
	"getwidth",
	"setwidth",
	"getheight",
	"setheight",
	"penup",
	"pendown",
	"penwidth",
	"pencolour",
	"stop",
-- 	"state",
	"home",
	"clear",
	"forward",
	"back",
	"setpos",
	"getx",
	"gety",
	"setx",
	"sety",
	"left",
	"right",
	"heading",
	"font",
	"filltext",
	"stroketext",
	"canvas_size",
	"export_img",
 	"reset"
}

print(picoturtle)
for k, v in pairs(picoturtle) do
	print(k)
	print(v)
end
_turtle = picoturtle.new()

-- For each turtle function in the function names list
-- create a corresponding global function which calles the
-- function on the global turtle object
for k, v in ipairs(_turtle_functions_names) do
	_G[v] = function (...)
		return _turtle[v](_turtle, ...) 
	end
end

-- Registered turtle functions in global
fd = forward
bk = back
rt = right
lt = left
pu = penup
pd = pendown

-- Two-letter aliases created for oft-used functions.
-- ------Turtle Lua Init Complete------
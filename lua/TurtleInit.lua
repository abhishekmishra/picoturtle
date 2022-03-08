-- ------Turtle Lua Init Started ------
-- If you can see this, lua print is redirected to the Turtle Console.

local _turtle_functions_names = {
	"getwidth",
	"setwidth",
	"getheight",
	"setheight",
	"penup",
	"pu",
	"pendown",
	"pd",
	"penwidth",
	"pencolour",
	"stop",
-- 	"state",
	"home",
	"clear",
	"forward",
	"fd",
	"back",
	"bk",
	"setpos",
	"getx",
	"gety",
	"setx",
	"sety",
	"left",
	"lt",
	"right",
	"rt",
	"heading",
	"font",
	"filltext",
	"stroketext",
	"canvas_size",
	"export_img",
 	"reset"
}

_turtle = picoturtle.new()

-- For each turtle function in the function names list
-- create a corresponding global function which calles the
-- function on the global turtle object
for k, v in ipairs(_turtle_functions_names) do
	_G[v] = function (...)
		return _turtle[v](_turtle, ...) 
	end
end

-- ------Turtle Lua Init Complete------
--[[--
Utility classes and functions to divide the Turtle's canvas into boxes (rectangular areas), and then restrict the Turtle to the rectangular areas.

Defines two classes, Box and BoxTurtle.
- The Box class provides a way to define a rectangular box inside a canvas.
- The BoxTurtle class provides a Turtle object with all the methods of a regular PicoTurtle, but which now lives inside the specified Box.

date  : 15/11/2022
	
@author: Abhishek Mishra
--]]--

local Vec2 = require'pt/Vec2'
local Vec4 = require'pt/Vec4'
local Box = require'pt/Box'
local BoxTurtle = require'pt/BoxTurtle'
local create_box_grid = require'pt/BoxUtils'.create_box_grid

--- @export
return {
	Box = Box,
	BoxTurtle = BoxTurtle,
	Vec2 = Vec2,
	Vec4 = Vec4,
	create_box_grid = create_box_grid
}
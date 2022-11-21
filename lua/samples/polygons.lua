-- polygons.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- Draws a polygon
--
local t = t or require'picoturtle'.new()
t:canvas_size(512, 512)
t:reset()

-- a simple turtle program to print a polygon
function poly(num, side, angle)
    for i = 1, num, 1 do
        t:forward(side)
        t:right(angle)
    end
end

-- draw a hexagon in red
t:penwidth(2)
t:pendown()
t:pencolor('red')
poly(6, 50, 60)

-- move up 100
t:penup()
t:forward(-100)
t:right(90)

-- draw a pentagon in blue
t:pendown()
t:pencolor('blue')
poly(5, 50, 72)
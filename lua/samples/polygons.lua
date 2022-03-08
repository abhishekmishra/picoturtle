-- polygons.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- Draws a polygon
--
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
t:pencolour(255, 0, 0)
poly(6, 50, 60)

-- move up 100
t:penup()
t:forward(-100)
t:right(90)

-- draw a pentagon in blue
t:pendown()
t:pencolour(0, 0, 255)
poly(5, 50, 72)
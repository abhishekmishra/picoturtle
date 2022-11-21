-- sierpinski.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- Sierpinski triangles
--
local t = t or require'picoturtle':new()
t:reset()

function draw_sierpinski(length, depth)
    if depth == 0 then
        t:pendown()
        for i = 1,3,1 do
            t:forward(length)
            t:left(120)
        end
        t:penup()
    else
        half = length/2.0
        draw_sierpinski(half, depth - 1)
        t:forward(half)
        draw_sierpinski(half, depth - 1)
        t:forward(-half)
        t:left(60)
        t:forward(half)
        t:right(60)
        draw_sierpinski(half, depth - 1)
        t:left(60)
        t:forward(-half)
        t:right(60)
    end
end


t:penup()
t:left(90)
t:forward(200)
t:right(90)
t:forward(-200)
t:pencolor(255, 69, 0)
t:penwidth(2)
t:pendown()
t:right(90)
draw_sierpinski(6000, 5)

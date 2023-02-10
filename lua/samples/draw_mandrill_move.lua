-- draw_mandrill.lua
-- Author: Abhishek Mishra
-- Date: 10/03/2023
--
-- Draws the mandrill png test image on the canvas
--

local t = t or require'picoturtle'.new()

t:pu()

for i = 1, 3, 1 do
	t:setpos(100, i * 100)
	t:clear()
	t:pic("res/images/mandrill.png")
	t:delay(100)
end
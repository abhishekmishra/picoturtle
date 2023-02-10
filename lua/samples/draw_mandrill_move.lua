-- draw_mandrill.lua
-- Author: Abhishek Mishra
-- Date: 10/03/2023
--
-- Draws the mandrill png test image on the canvas
--

local t = t or require'picoturtle'.new()

local mandrill = "res/images/mandrill.png"
t:pu()

for i = 1, 5, 1 do
	t:setpos(256, (i - 1) * 128)
	t:clear()
	t:pic(mandrill)
	t:delay(100)
end

for i = 5, 1, -1 do
	t:setpos(256, (i - 1) * 128)
	t:clear()
	t:pic(mandrill)
	t:delay(100)
end
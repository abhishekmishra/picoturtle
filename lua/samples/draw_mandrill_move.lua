-- draw_mandrill.lua
-- Author: Abhishek Mishra
-- Date: 10/03/2023
--
-- Draws the mandrill png test image on the canvas
--

local t = t or require'picoturtle'.new()

-- load mandrill pic for reuse
local mandrill_pic = t:loadpic("res/images/mandrill.png")
print(mandrill_pic)

t:pu()

for i = 1, 5, 1 do
	t:setpos(256, (i - 1) * 128)
	t:clear()
	t:pic(mandrill_pic)
	t:delay(100)
end

for i = 5, 1, -1 do
	t:setpos(256, (i - 1) * 128)
	t:clear()
	t:pic(mandrill_pic)
	t:delay(100)
end
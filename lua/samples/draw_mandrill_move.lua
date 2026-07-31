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

local num_steps = 10
local _, height = t:canvas_size()
local image_height = 512
local delta = (height - image_height)/(num_steps - 1)
local delay = 100

t:disable_update()

for i = 1, num_steps, 1 do
	t:setpos(256, image_height + (i - 1) * delta)
	t:clear()
	t:pic(mandrill_pic)
	t:paint()
	t:delay(delay)
end

for i = num_steps, 1, -1 do
	t:setpos(256, image_height + (i - 1) * delta)
	t:clear()
	t:pic(mandrill_pic)
	t:paint()
	t:delay(delay)
end

t:enable_update()

-- draw_mandrill.lua
-- Author: Abhishek Mishra
-- Date: 10/03/2023
--
-- Draws the mandrill png test image on the canvas
--

local t = t or require'picoturtle'.new()

t:pu()
t:setpos(256, 256)
t:pic("res/images/mandrill.png")
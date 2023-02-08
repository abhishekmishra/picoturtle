-- gif_basic.lua
-- Author: Abhishek Mishra
-- Date: 08/02/2023
--
-- create a gif with the images captured from multiple turtle states

local pt = require'picoturtle'
local t = t or pt.new()

t:pendown()
t:penwidth(10)
t:pencolor('red')

local num = 5

for i = 1, 5, 1 do
    t:fd((num - i) * 100)
    t:rt(90)
    t:snap('out/basic' .. tostring(i) .. '.png')
end

pt.makegif()
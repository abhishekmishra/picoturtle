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

local snaps = {}

for i = 1, 5, 1 do
    t:fd((num - i) * 100)
    t:rt(90)
    local imgname = 'out/basic' .. tostring(i) .. '.png'
    -- snaps[i] = imgname
    snaps[i] = t:snap(imgname)
end

for k, v in ipairs(snaps) do
    print('snap#' .. tostring(k) .. ' = ' .. tostring(v))
end

local gif_fname = "out/basic.gif"

pt.makegif(snaps, gif_fname)

print('Gif written to '.. gif_fname)
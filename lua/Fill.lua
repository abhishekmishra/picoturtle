local picoturtle = require "picoturtle"

local w = 40
local h = w

t = picoturtle.new()
t:canvas_size(w, h)
t:pencolour(128, 64, 32)
t:pu()
t:setpos(0, h/2)
t:rt(90)
t:pd()
t:penwidth(h)
t:fd(w)
t:export_img('out/fill.png')
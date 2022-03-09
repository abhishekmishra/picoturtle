-- simple turtle program to draw a square
-- press F5 to run
local picoturtle = require "picoturtle"

local sz = 300

t = picoturtle.new()
t:canvas_size(sz * 1.25, sz * 1.25)
t:pu()
t:setpos(4, 4)
t:penwidth(4)
t:pd()
t:fd(sz)
t:rt(90)
t:fd(sz)
t:rt(90)
t:fd(sz)
t:rt(90)
t:fd(sz)
t:rt(180)

t:pu()
t:setpos(16, 16)
t:pd()
t:font("Consolas", 32)
t:filltext("PicoTurtle")
print('done.')
t:export_img('out/test.png')
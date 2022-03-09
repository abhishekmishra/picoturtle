-- simple turtle program to draw a square
-- press F5 to run
local picoturtle = require "picoturtle"

t = picoturtle.new()
t:canvas_size(100, 100)
t:pu()
t:setpos(4, 4)
t:penwidth(4)
t:pd()
t:fd(40)
t:rt(90)
t:fd(40)
t:rt(90)
t:fd(40)
t:rt(90)
t:fd(40)
t:rt(180)
t:font("Consolas", 8)
t:filltext("PicoTurtle")
print('done.')
t:export_img('out/test.png')
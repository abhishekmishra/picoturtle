-- text-demo.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- text demo
--
local t = t or require'picoturtle'.new()
t:reset()

t:penup()
t:forward(200)
    
t:pendown()
t:heading(0)
--t:right(90)

for i = 10, 80, 1 do
    local fs = i
	t:save()
	t:heading(0)
    if i%2 == 0 then
        t:font('Maku', fs)
        t:pencolor(255, 128, 128)
        t:filltext('पीको टर्टल')
    else
        t:font('Calibri', fs)
        t:pencolor(255, 128, 255)
        t:filltext('PicoTurtle')
    end
	t:restore()
    t:penup()
    t:left(10)
    local mv = i * 1.5 
    t:forward(mv)
    t:right(0)
    t:pendown()
end
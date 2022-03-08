-- text-demo.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- text demo
--

t:reset()

t:penup()
t:back(250)
t:right(90)
t:forward(200)
t:left(90)
    
t:pendown()
t:right(90)

for i = 1, 40, 1 do
    local fs = i
    if i%2 == 0 then
        t:font('Mangal', fs)
        t:pencolour(255, 128, 128)
        t:filltext('पीको टर्टल')
    else
        t:font('Calibri', fs)
        t:pencolour(255, 128, 255)
        t:filltext('PicoTurtle')
    end
    t:penup()
    t:left(90 + 2)
    local mv = i * 0.5
    t:forward(mv)
    t:right(90)
    t:pendown()
end
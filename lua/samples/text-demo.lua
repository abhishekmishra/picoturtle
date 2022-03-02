-- text-demo.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- text demo
--

reset()

penup()
back(250)
right(90)
forward(200)
left(90)
    
pendown()
right(90)

for i = 1, 40, 1 do
    local fs = i
    if i%2 == 0 then
        font('Mangal', fs)
        pencolour(255, 128, 128)
        filltext('पीको टर्टल')
    else
        font('Calibri', fs)
        pencolour(255, 128, 255)
        filltext('PicoTurtle')
    end
    penup()
    left(90 + 2)
    local mv = i * 0.5
    forward(mv)
    right(90)
    pendown()
end
-- fern.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- Draws a fern
--

-- fern: function which
-- calls itself recursively
-- till the size is less than 1
-- to draw the fern.

reset()

function fern(size, sign)
    if (size < 1) then
        return
    end
    
    forward(size)

    right(70 * sign)
    fern(size/2, sign*-1)
    left(70 * sign)

    forward(size)

    left(70 * sign)
    fern(size/2, sign)
    right(70 * sign)

    right(7 * sign)
    fern(size - 2, sign)
    left(7 * sign)

    forward(-size)
    forward(-size)
end

penup()
back(200)

right(90)
back(20)
pencolour(200, 200, 0)
font('Calibri', 50)
filltext('Fern')
forward(20)
left(90)
forward(50)

pencolour(128, 128, 0)
pendown()
fern(25, 1)

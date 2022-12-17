--- tiny turtle lua program

-- reuse a turtle named t if already created or create a new one
local t = t or require 'picoturtle'.new()

-- Set the pencolour and penwidth of the turtle
t:pencolor('ninja turtle')      -- set color to "ninja turtle"
t:penwidth(4)                   -- 2 pixels

-- Set the pen down and draw for 100 pixels
t:pendown()
t:forward(200)                  -- 100 pixels

-- write some text on the canvas
t:heading(0)                    -- face right
t:font('arial', 32)             -- set font to Arial size 32pt
t:pencolor('black')             -- set pen color to 'black'
t:filltext('i am a turtle')     -- write 'i am a turtle' on the canvas

-- Print the state of the turtle to the console.
print('Done -> ' .. t:state())
-- basic turtle lua program

-- This magical incantation helps reuse a turtle named t if already created
-- Or create a new one if one doesn't
local t = t or require 'picoturtle'.new()

-- set canvas size to width=512px, height=512px
t:canvas_size(512, 512)
t:reset()

-- Set the pencolour and penwidth of the turtle
t:pencolor('ninja turtle') -- set color to "ninja turtle"
t:penwidth(2)              -- 2 pixels

-- Set the pen down and draw for 100 pixels
t:pendown()
t:forward(100)             -- 100 pixels

-- write some text on the canvas
t:heading(0)
t:font('arial', 20)
t:pencolor('black')
t:filltext('i am a turtle')

-- Stop the turtle.
t:stop()

-- Print the state of the turtle to the console.
print(t:state())

-- Done.
print('Turtle done.')

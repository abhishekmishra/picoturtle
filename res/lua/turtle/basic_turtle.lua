-- basic turtle lua program

-- This magical incantation helps reuse a turtle named t if already created
-- Or create a new one if one doesn't
local t = t or require 'picoturtle'.new()

-- Set the pencolour and penwidth of the turtle
t:pencolour(255, 0, 0) -- red = 255, green = 0, blue = 0
t:penwidth(5)          -- 5 pixels

-- Set the pen down and draw for 100 pixels
t:pendown()
t:forward(100)         -- 100 pixels

-- Stop the turtle.
t:stop()

-- Print to console
print('Turtle done.')
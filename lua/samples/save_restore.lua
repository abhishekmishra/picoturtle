-- save_restore.lua
-- Author: Abhishek Mishra
-- Date: 20/03/2023
--
-- Demonstrates use of the Turtle state stack via the save() and restore() methods.
-- The state stack is last in first out structure.
-- save() stores the latest turtle state to the top of the stack.
-- restore() restores the topmost turtle state from the stack.
-- restore() does nothing if there are no entries in the state stack.
--

local t = t or require 'picoturtle'.new()

local w, h = t:canvas_size(800, 800)

-- initial pen settings and draw a black circle
t:pencolor('black')
t:penwidth(10)
t:penup()
t:setpos(w/2, h/2)
t:pendown()
t:circle(300)

-- save state and draw circle with red
t:save()
t:pencolor('red')
t:circle(200)

-- save state and draw circle with blue
t:save()
t:pencolor('blue')
t:circle(100)

-- restore state (top of state stack - last in first out)
-- therefore red pen is restored
t:restore()
t:circle(150)

-- restore state, to black pen now
t:restore()
t:circle(250)
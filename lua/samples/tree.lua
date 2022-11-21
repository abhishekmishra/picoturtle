-- tree.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- a recursive tree
--
local t = t or require'picoturtle'.new()
t:reset()

function tree(size)
    if size < 4 then
        t:forward(size)
        t:forward(-size)
        return
    end
    
    t:forward(size/4)

    t:left(60)
    tree(size/2)
    t:right(60)

    t:forward(size/4)

    t:right(35)
    tree(size/2)
    t:left(35)

    t:forward(size/4)

    t:left(30)
    tree(size/2)
    t:right(30)

    t:forward(size/4)

    t:right(25)
    tree(size/2)
    t:left(25)

    t:forward(-size)
end

t:penwidth(1)
t:penup()
t:back(100)
t:pendown()
t:pencolor(165, 42, 42)
tree(128)
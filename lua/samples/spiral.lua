-- spiral.lua
-- Author: Abhishek Mishra
-- Date: 15/01/2022
--
-- A spiral
--
t:reset()

function spiral(distance, angle, pwidth, distance_inc, angle_inc, pwidth_inc)
    r = 0
    g = 0
    b = 255
    t:pencolour(r, g, b)
    x = distance
    a = angle
    pw = pwidth
    for i = 1, 100, 1 do
        r = r + 2
        g = g + 0
        b = b - 2
        t:pencolour(r, g, b)
        t:penwidth(pw)
        t:forward(x)
        t:right(a)
        x = x + distance_inc
        a = a + angle_inc
        pw = pw + pwidth_inc
    end
end

t:pendown()
spiral(1, 25, 1, 1, 0, 0.2)

print('Spiral done.')

local turtle = t or require("picoturtle").new()

assert(math.abs(turtle:getx() - 512) < 0.001)
assert(math.abs(turtle:gety() - 512) < 0.001)

turtle:penup()
turtle:setpos(412, 412)
turtle:pendown()
turtle:right(90)

for _ = 1, 4 do
    turtle:forward(200)
    turtle:left(90)
end

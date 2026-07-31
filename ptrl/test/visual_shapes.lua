local turtle = t or require("picoturtle").new()

turtle:reset()
turtle:penwidth(4)

turtle:penup()
turtle:setpos(350, 512)
turtle:pendown()
turtle:pencolor("red")
turtle:circle(100)

turtle:penup()
turtle:setpos(600, 412)
turtle:heading(90)
turtle:pendown()
turtle:pencolor("blue")
turtle:arc(100, 180, 36)

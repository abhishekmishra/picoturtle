from picoturtle import *
create_turtle()

### Your code goes here ###

def spiral(distance, angle, pwidth, distance_inc, angle_inc, pwidth_inc):
    r = 0
    g = 0
    b = 255
    pencolour(r, g, b)
    x = distance
    a = angle
    pw = pwidth
    for i in range(100):
        r += 2
        g += 0
        b -= 2
        pencolour(r, g, b)
        penwidth(pw)
        forward(x)
        right(a)
        x += distance_inc
        a += angle_inc
        pw += pwidth_inc

pendown()
spiral(1, 25, 1, 1, 0, 0.2)

### Your code ends here ###

# Always stop the turtle
stop()
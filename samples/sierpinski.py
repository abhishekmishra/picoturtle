from picoturtle import *

def draw_sierpinski(length, depth):
    if depth == 0:
        pendown()
        for i in range(3):
            forward(length)
            left(120)
        penup()
    else:
        half = length/2.0
        draw_sierpinski(half, depth - 1)
        forward(half)
        draw_sierpinski(half, depth - 1)
        forward(-half)
        left(60)
        forward(half)
        right(60)
        draw_sierpinski(half, depth - 1)
        left(60)
        forward(-half)
        right(60)


if __name__ == "__main__":
    # Create the turtle before using
    create_turtle()

    # Your code goes here #

    penup()
    left(90)
    forward(200)
    right(90)
    forward(-200)
    pencolour(255, 69, 0)
    penwidth(2)
    pendown()
    right(90)
    draw_sierpinski(400, 5)

    # Your code ends here #

    # Always stop the turtle
    stop()

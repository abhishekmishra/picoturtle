from picoturtle import *

def fern(size, sign):
    if (size < 1):
        return
    
    forward(size)

    right(70 * sign)
    fern(size/2, sign*-1)
    left(70 * sign)

    forward(size)

    left(70 * sign)
    fern(size/2, sign)
    right(70 * sign)

    right(7 * sign)
    fern(size - 2, sign)
    left(7 * sign)

    forward(-size)
    forward(-size)


if __name__ == "__main__":
    create_turtle()

    ### Your code goes here ###

    penup()
    back(200)

    right(90)
    back(20)
    pencolour(200, 200, 0)
    font('14pt Arial')
    filltext('Fern')
    forward(20)
    left(90)
    forward(50)

    pencolour(0, 128, 0)
    pendown()
    fern(25, 1)

    ### Your code ends here ###

    ### Always stop the turtle
    stop()
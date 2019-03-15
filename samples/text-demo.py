from picoturtle import *


if __name__ == "__main__":
    # Create the turtle before using
    create_turtle()

    # Your code goes here #

    penup()
    back(250)
    right(90)
    forward(200)
    left(90)
    
    pendown()
    right(90)

    for i in range(40):
        font('' + str(i) + 'pt Arial')
        if i%2 == 0:
            pencolour(255, 128, 128)
            filltext('पीको टर्टल')
        else:
            pencolour(255, 128, 255)
            filltext('PicoTurtle')
        penup()
        left(90 + 2)
        forward(i * 1.3)
        right(90)
        pendown()

    # Your code ends here #

    # Always stop the turtle
    stop()

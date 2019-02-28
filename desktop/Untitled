const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('../picoturtle');

create_turtle();

/* a simple turtle program to print a polygon */
function poly(num, side, angle) {
    for(var i = 0; i < num; i++) {
        forward(side);
        right(angle);
    }
}

//draw a hexagon in red
penwidth(2);
pendown();
pencolour(255, 0, 0);
poly(6, 50, 60);

//move up 100
penup();
forward(-100);
right(90);

//draw a pentagon in blue
pendown();
pencolour(0, 0, 255);
poly(5, 50, 72);

stop();

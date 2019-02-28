const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('./picoturtle-sync');
create_turtle();

/* Your code goes here */

function spiral(distance, angle, pwidth, distance_inc, angle_inc, pwidth_inc) {
    let r = 0;
    let g = 0;
    let b = 255;
    pencolour(r, g, b);
    let x = distance;
    let a = angle;
    let pw = pwidth;
    for (var i = 0; i < 100; i++) {
        r += 2;
        g += 0;
        b -= 2;
        pencolour(r, g, b);
        penwidth(pw);
        forward(x);
        right(a);
        x += distance_inc;
        a += angle_inc;
        pw += pwidth_inc;
    }
}

pendown();
spiral(1, 25, 1, 1, 0, 0.2);

print('Spiral done.');

/* Your code ends here */

// Always stop the turtle
stop();

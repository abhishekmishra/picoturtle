const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    await create_turtle();
    /* Your code goes here */

    async function draw_sierpinski(length, depth) {
        if(depth == 0) {
            await pendown();
            for(var i = 0; i < 3; i++) {
                await forward(length);
                await left(120);
            }
            await penup();
        }
        else {
            let half = length/2.0;
            await draw_sierpinski(half, depth - 1);
            await forward(half);
            await draw_sierpinski(half, depth - 1);
            await forward(-half);
            await left(60);
            await forward(half);
            await right(60);
            await draw_sierpinski(half, depth - 1);
            await left(60);
            await forward(-half);
            await right(60);
        }
    }

    await penup();
    await left(90);
    await forward(200);
    await right(90);
    await forward(-200);
    await pencolour(255, 69, 0);
    await penwidth(2);
    await pendown();
    await right(90);
    await draw_sierpinski(400, 5);

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};
main();
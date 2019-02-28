const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    await create_turtle();

    /* a simple turtle program to print a polygon */
    async function poly(num, side, angle) {
        for (var i = 0; i < num; i++) {
            await forward(side);
            await right(angle);
        }
    }

    //draw a hexagon in red
    await penwidth(2);
    await pendown();
    await pencolour(255, 0, 0);
    await poly(6, 50, 60);

    //move up 100
    await penup();
    await forward(-100);
    await right(90);

    //draw a pentagon in blue
    await pendown();
    await pencolour(0, 0, 255);
    await poly(5, 50, 72);

    await stop();
};

main();
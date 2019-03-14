const { create_turtle, home, clear, stop, print, state } = require('@picoturtle/picoturtle-nodejs-client');
const { font, filltext, stroketext } = require('@picoturtle/picoturtle-nodejs-client');
const { penup, pendown, penwidth, pencolour } = require('@picoturtle/picoturtle-nodejs-client');
const { left, right, forward, back } = require('@picoturtle/picoturtle-nodejs-client');
const { setpos, setx, sety, heading } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    /* Create the turtle before using */
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

main()
.catch((err) => {
    console.error(err);
    process.exit(1);
});
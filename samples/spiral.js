const { create_turtle, home, clear, stop, print, state } = require('@picoturtle/picoturtle-nodejs-client');
const { font, filltext, stroketext } = require('@picoturtle/picoturtle-nodejs-client');
const { penup, pendown, penwidth, pencolour } = require('@picoturtle/picoturtle-nodejs-client');
const { left, right, forward, back } = require('@picoturtle/picoturtle-nodejs-client');
const { setpos, setx, sety, heading } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    /* Create the turtle before using */
    await create_turtle();

    /* Your code goes here */

    async function spiral(distance, angle, pwidth, distance_inc, angle_inc, pwidth_inc) {
        let r = 0;
        let g = 0;
        let b = 255;
        await pencolour(r, g, b);
        let x = distance;
        let a = angle;
        let pw = pwidth;
        for (var i = 0; i < 100; i++) {
            r += 2;
            g += 0;
            b -= 2;
            await pencolour(r, g, b);
            await penwidth(pw);
            await forward(x);
            await right(a);
            x += distance_inc;
            a += angle_inc;
            pw += pwidth_inc;
        }
    }

    await pendown();

    await spiral(1, 25, 1, 1, 0, 0.2);

    await print('Spiral done.');

    /* Your code ends here */

    // Always stop the turtle
    await stop();
};

main()
.catch((err) => {
    console.error(err);
    process.exit(1);
});
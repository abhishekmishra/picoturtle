const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, back, font, filltext, stroketext, right, left, print } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    await create_turtle();
    /* Your code goes here */

    async function fern(size, sign) {
        if (size < 1) {
            return;
        }
        await forward(size);

        await right(70 * sign);
        await fern(size/2, sign*-1);
        await left(70 * sign);

        await forward(size);

        await left(70 * sign);
        await fern(size/2, sign);
        await right(70 * sign);

        await right(7 * sign);
        await fern(size - 2, sign);
        await left(7 * sign);

        await forward(-size);
        await forward(-size);
    }

    await penup();
    await back(200);

    await right(90);
    await back(20);
    await pencolour(200, 200, 0);
    await font('14pt Arial');
    await filltext('Fern');
    await forward(20);
    await left(90);
    await forward(50);

    await pencolour(0, 128, 0);
    await pendown();
    await fern(25, 1);

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};
main();
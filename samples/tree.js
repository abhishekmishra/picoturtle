const { create_turtle, home, clear, stop, print, state } = require('@picoturtle/picoturtle-nodejs-client');
const { font, filltext, stroketext } = require('@picoturtle/picoturtle-nodejs-client');
const { penup, pendown, penwidth, pencolour } = require('@picoturtle/picoturtle-nodejs-client');
const { left, right, forward, back } = require('@picoturtle/picoturtle-nodejs-client');
const { setpos, setx, sety, heading } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    /* Create the turtle before using */
    await create_turtle();

    /* Your code goes here */

    async function tree(size) {
        if (size < 4) {
            await forward(size);
            await forward(-size);
            return;
        }

        await forward(size / 4);

        await left(60);
        await tree(size / 2);
        await right(60);

        await forward(size / 4);

        await right(35);
        await tree(size / 2);
        await left(35);

        await forward(size / 4);

        await left(30);
        await tree(size / 2);
        await right(30);

        await forward(size / 4);

        await right(25);
        await tree(size / 2);
        await left(25);

        await forward(-size);
    }

    await penup();
    await back(100);
    await pendown();
    await pencolour(165, 42, 42);
    await tree(128);

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};

main()
.catch((err) => {
    console.error(err);
    process.exit(1);
});
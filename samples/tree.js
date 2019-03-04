const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
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

    await pendown();
    await pencolour(165, 42, 42);
    await tree(64);

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};
main();
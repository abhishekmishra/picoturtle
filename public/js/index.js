import TurtleProxy from './turtle_proxy.js';
import { track_turtle } from './turtle_canvas.js';

async function square(t, side) {
    for (var i = 0; i < 4; i++) {
        await t.forward(side);
        await t.right(90);
    }
}

async function poly(t, side, angle, incs, inca) {
    for (var i = 0; i < 100; i++) {
        await t.forward(side);
        await t.right(angle);
        side += incs;
        angle += inca;
    }
}

async function my_turtle() {
    var t = new TurtleProxy();
    let state = await t.init();
    // for(var i = 0; i < 4; i++) {
    //     await t.penup();
    //     await t.forward(20);
    //     await t.pendown();
    //     await square(t, 10);
    // }
    await t.pencolour(255, 0, 0);
    await t.pendown();
    await poly(t, 5, 120, 3, 0);
    await t.stop();
    return t.name;
}

async function run_turtle() {
    let turtle_name = await my_turtle();
    let x = await track_turtle(turtle_name);
};
run_turtle();


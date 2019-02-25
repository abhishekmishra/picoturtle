import TurtleProxy from './turtle_proxy.js';
import { list_turtles, track_turtle } from './turtle_canvas.js';

list_turtles();

async function square(t, side) {
    for (var i = 0; i < 4; i++) {
        await t.forward(side);
        await t.right(90);
    }
}

async function poly(t, side, angle, incs, inca) {
    for (var i = 0; i < 10; i++) {
        await t.forward(side);
        await t.right(angle);
        side += incs;
        angle += inca;
    }
}

async function my_turtle() {
    var t = new TurtleProxy();
    let state = await t.init();
    await t.pencolour(255, 0, 0);
    await t.pendown();
    // for(var i = 0; i < 2; i++) {
    //     await t.penup();
    //     await t.forward(60);
    //     await t.pendown();
    //     await square(t, 50);
    // }
    await poly(t, 5, 120, 3, 0);
    await t.stop();
    return t.name;
}

async function run_turtle() {
    let turtle_name = await my_turtle();
    await track_turtle(turtle_name);
};

/**
 * Check if details and list columns are to be shown.
 * 
 * If there is a name in the url, track the turtle with that name
 */
var url = new URL(window.location.href);
var name = url.searchParams.get("name");
var show_details = url.searchParams.get('details');
var show_list = url.searchParams.get('list');

if(show_details != null && show_details == 0) {
    let details_container = document.getElementById('turtle_details_container');
    details_container.hidden = true;
}

if(show_list != null && show_list == 0) {
    let list_container = document.getElementById('turtle_list_container');
    list_container.hidden = true;
}

if (name != null) {
    track_turtle(name);
} else {
    run_turtle();
}

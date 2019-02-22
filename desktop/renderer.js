// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const Turtle = require('./turtle_canvas').Turtle;
const axios = require('axios');
const TurtleProxy = require('./turtle_proxy').TurtleProxy;
const sleep = require('./utils').sleep;

const TURTLE_SERVER_URL = 'http://localhost:3000';

async function list_turtles() {
    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/list');
    let ls = await req.data;
    let turtle_list = document.getElementById('turtle_list');
    turtle_list.innerHTML = '';
    ls.forEach(element => {
        turtle_list.innerHTML += '<li id="' + element + '">' + element + '</li>';
    });
    ls.forEach(element => {
        let el = document.getElementById(element);
        el.className += " name-li";
        el.onclick = function () {
            track_turtle(element);
        };
    });
}

function mark_selected(name) {
    let els = document.getElementsByClassName("name-li selected");
    for (var i = 0; i < els.length; i++) {
        var e = els[i];
        e.className = "name-li";
    }

    let sel = document.getElementById(name);
    if (sel != null) {
        sel.className += " selected";
    }
    let turtle_name = document.getElementById('turtle_name');
    turtle_name.innerHTML = name;
}

function add_object_code_line(cmd_args) {
    let oc = document.getElementById('object_code');
    oc.innerText += '\n';
    for (var i = 0; i < cmd_args.length; i++) {
        if (typeof cmd_args[i] == 'object') {
            if ('hex' in cmd_args[i]) {
                oc.innerText += ' ' + cmd_args[i].hex;
            } else {
                oc.innerText += ' ' + JSON.stringify(cmd_args[i]);
            }
        } else {
            oc.innerText += ' ' + cmd_args[i];
        }
    }
}

async function track_turtle(name) {
    await list_turtles();
    mark_selected(name);

    let oc = document.getElementById('object_code');
    oc.innerText = '';

    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/' + name + '/start_state');
    let t = await req.data;
    var local_turtle = new Turtle("turtle_canvas", t);
    await fetch_commands(local_turtle, 0);
}

async function fetch_commands(local_turtle, cmd_id) {
    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/' + local_turtle.name + '/command?id=' + cmd_id);
    let cmd = await req.data;
    //console.log(cmd);
    if ('cmd' in cmd) {
        let args = [cmd.cmd];
        if (cmd.args != null) {
            Array.prototype.push.apply(args, cmd.args);
        }
        // console.log(args);
        local_turtle.reset();
        local_turtle.batchStart();
        local_turtle.exec.apply(local_turtle, args);
        local_turtle.batchEnd();
        add_object_code_line(args);
    }
    if (cmd.hasmore) {
        // console.log('there are more commands pending, after ' + cmd_id);
        fetch_commands(local_turtle, cmd_id += 1);
    } else {
        if (cmd.turtle.last == -1 || cmd.turtle.last > cmd_id) {
            await sleep(5000);
            fetch_commands(local_turtle, cmd_id);
        } else {
            //console.log(local_turtle);
        }
    }
}

list_turtles();

// async function square(t, side) {
//     for (var i = 0; i < 4; i++) {
//         await t.forward(side);
//         await t.right(90);
//     }
// }

// async function poly(t, side, angle, incs, inca) {
//     for (var i = 0; i < 100; i++) {
//         await t.forward(side);
//         await t.right(angle);
//         side += incs;
//         angle += inca;
//     }
// }

// async function my_turtle() {
//     try {
//         var t = new TurtleProxy();
//         let state = await t.init();
//         await t.pencolour(255, 0, 0);
//         await t.pendown();
//         // for(var i = 0; i < 2; i++) {
//         //     await t.penup();
//         //     await t.forward(60);
//         //     await t.pendown();
//         //     await square(t, 50);
//         // }
//         await poly(t, 5, 120, 3, 0);
//         await t.stop();
//         return t.name;
//     } catch (error) {
//         console.log(error);
//     }
// }

// async function run_turtle() {
//     let turtle_name = await my_turtle();
//     await track_turtle(turtle_name);
// };

/**
 * Check if details and list columns are to be shown.
 * 
 * If there is a name in the url, track the turtle with that name
 */
// var url = new URL(window.location.href);
// var name = url.searchParams.get("name");
// var show_details = url.searchParams.get('details');
// var show_list = url.searchParams.get('list');

// if(show_details != null && show_details == 0) {
//     let details_container = document.getElementById('turtle_details_container');
//     details_container.hidden = true;
// }

let show_list = 0;
if (show_list != null && show_list == 0) {
    let list_container = document.getElementById('turtle_list_container');
    list_container.hidden = true;
}

// if (name != null) {
//     track_turtle(name);
// } else {
//     run_turtle();
// }

// run_turtle();

let start_text = `async function square(t, side) {
    for (var i = 0; i < 4; i++) {
        await t.forward(side);
        await t.right(90);
    }
}

async function my_turtle(t) {
    await t.penup();
    await t.pencolour(255, 0, 0);
    await t.pendown();
    for(var i = 0; i < 2; i++) {
        await t.penup();
        await t.forward(60);
        await t.pendown();
        await square(t, 50);
    }
    await t.stop();
    return t.name;
}

return await my_turtle(t);
`;

class TurtleEditor {
    constructor() {
        this.editor = monaco.editor.create(document.getElementById('turtle_code'), {
            value: [
                start_text
            ].join('\n'),
            language: 'javascript'
        });
    }

    async run_turtle() {
        let text = this.editor.getValue();
        var t = new TurtleProxy();
        let state = await t.init();
        // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
        const AsyncFunction = Object.getPrototypeOf(async function(){}).constructor;
        var f = new AsyncFunction('t', text);
        let turtle_name = await f(t);
        return await track_turtle(turtle_name);
    }
}

// (function () {
const path = require('path');
const amdLoader = require('./node_modules/monaco-editor/min/vs/loader.js');
const amdRequire = amdLoader.require;
const amdDefine = amdLoader.require.define;
function uriFromPath(_path) {
    var pathName = path.resolve(_path).replace(/\\/g, '/');
    if (pathName.length > 0 && pathName.charAt(0) !== '/') {
        pathName = '/' + pathName;
    }
    return encodeURI('file://' + pathName);
}
amdRequire.config({
    baseUrl: uriFromPath(path.join(__dirname, './node_modules/monaco-editor/min'))
});
// workaround monaco-css not understanding the environment
self.module = undefined;
amdRequire(['vs/editor/editor.main'], async function () {
    let t = new TurtleEditor();
    await t.run_turtle();
});
// })();
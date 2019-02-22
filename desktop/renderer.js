// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const list_turtles = require('./turtle_canvas').list_turtles;
const track_turtle = require('./turtle_canvas').track_turtle;
const TurtleProxy = require('./turtle_proxy').TurtleProxy;
const { spawn } = require('child_process');

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



const {dialog} = require('electron').remote;

document.querySelector('#open_button').addEventListener('click', function (event) {
    dialog.showOpenDialog({
        properties: ['openFile', 'multiSelections']
    }, function (files) {
        if (files !== undefined) {
            // handle files
        }
    });
});


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

let py_start_text = `# a simple turtle program to print a polygon
def poly(num, side, angle):
    for i in range(num):
        forward(side)
        right(angle)

#draw a hexagon in red
pendown()
pencolour(255, 0, 0)
poly(6, 50, 60)

#move up 100
penup()
forward(-100)
right(90)

#draw a pentagon in blue
pendown()
pencolour(0, 0, 255)
poly(5, 50, 72)

stop()
`;

class TurtleEditor {
    constructor() {
        this.language = 'python';
        this.editor = monaco.editor.create(document.getElementById('turtle_code'), {
            value: [
                py_start_text
            ].join('\n'),
            //language: 'javascript'
            language: this.language
        });
    }

    async run_turtle() {
        let text = this.editor.getValue();
        var t = new TurtleProxy();
        let state = await t.init();
        track_turtle(state.name);
        if (this.language == 'javascript') {
            // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
            const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
            var f = new AsyncFunction('t', text);
            await f(t);
        } else if (this.language == 'python') {
            //const ls = spawn('ls', ['-lh', '/usr']);
            let options = {
                env: {
                    'PYTHONPATH': __dirname + '/../client/python'
                }
            };
            console.log(options);
            const ls = spawn('python3',
                ['-c', 'from picoturtle import *;t = Turtle("' + state.name + '");__builtins__.t = t;' + text],
                options);

            ls.stdout.on('data', (data) => {
                console.log(`stdout: ${data}`);
            });

            ls.stderr.on('data', (data) => {
                console.log(`stderr: ${data}`);
            });

            ls.on('close', (code) => {
                console.log(`child process exited with code ${code}`);
            });
        }
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
// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const list_turtles = require('./turtle_canvas').list_turtles;
const track_turtle = require('./turtle_canvas').track_turtle;
const TurtleProxy = require('./turtle_proxy').TurtleProxy;
const { spawn } = require('child_process');
const fs = require('fs');
const { dialog } = require('electron').remote;

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

let START_TEMPLATES = {};

START_TEMPLATES['javascript'] = `async function square(t, side) {
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

START_TEMPLATES['python'] = `from picoturtle import *
create_turtle()`;

class TurtleEditor {
    constructor() {
        this.editor = monaco.editor.create(document.getElementById('turtle_code'), {
            value: [
                ''
            ].join('\n'),
            //language: 'javascript'
            // language: this.language
        });
        this.setSelectedFile();
        this.setLanguage('python');
        $('#open_button').on('click', { editor: this }, this.openFile);
        $('#run_button').on('click', { editor: this }, this.run_turtle);
        $('#editor_language_select').on('change', { editor: this }, function(event) {
            event.data.editor.setLanguage(this.value);
        });
    }

    setLanguage(name) {
        this.language = name;
        monaco.editor.setModelLanguage(this.editor.getModel(), name);
        $('#editor_language_select').val(this.language);
        if (this.file != 'Untitled') {
            $('#editor_language_select').prop('disabled', 'disabled');
        } else {
            $('#editor_language_select').prop('disabled', false);
            this.editor.setValue(START_TEMPLATES[this.language]);
        }
    }

    setSelectedFile(selected_file = null) {
        if(selected_file == null) {
            this.file = 'Untitled';
        } else {
            this.file = selected_file;
        }
        $('#editor_file').html(this.file);

        if (this.file.endsWith('.js')) {
            this.setLanguage('javascript');
        }
        if (this.file.endsWith('.py')) {
            this.setLanguage('python');
        }
    }

    openFile(event) {
        dialog.showOpenDialog({
            properties: ['openFile']
        }, (files) => {
            if (files !== undefined) {
                if (files.length == 1) {
                    let editor = event.data.editor;
                    let selected_file = files[0];
                    editor.setSelectedFile(selected_file);
                    let text = fs.readFileSync(selected_file, { encoding: 'utf-8' });
                    console.log(editor.language);
                    editor.editor.setValue(text);
                }
            }
        });
    }

    async run_turtle(event) {
        let text = event.data.editor.editor.getValue();
        
        var t = new TurtleProxy();
        let state = await t.init();
        track_turtle(state.name);

        let editor = event.data.editor;

        if (editor.language == 'javascript') {
            // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
            const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
            var f = new AsyncFunction('t', text);
            await f(t);
        } else if (editor.language == 'python') {
            //const ls = spawn('ls', ['-lh', '/usr']);
            let options = {
                env: {
                    'PYTHONPATH': __dirname + '/../client/python'
                }
            };
            console.log(options);
            let command_args = [editor.file, state.name];
            if (editor.file == 'Untitled') {
                command_args = ['-c', text, state.name]
            }
            const ls = spawn('python3',
                command_args,
                options);

            ls.stdout.on('data', (data) => {
                console.log(`stdout: ${data}`);
            });

            ls.stderr.on('data', (data) => {
                console.log(`stderr: ${data}`);
                t.stop();
            });

            ls.on('close', (code) => {
                console.log(`child process exited with code ${code}`);
                t.stop();
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
    // await t.run_turtle();
});
// })();
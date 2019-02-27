// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const list_turtles = require('./turtle_canvas').list_turtles;
const track_turtle = require('@picoturtle/picoturtle-web-canvas').track_turtle_node;
const TurtleCanvas = require('@picoturtle/picoturtle-web-canvas').Turtle;
const Turtle = require('./picoturtle').Turtle;
const { spawn } = require('child_process');
const fs = require('fs');
const { dialog, app } = require('electron').remote;
const ipcRenderer = require('electron').ipcRenderer;
const path = require('path');
const appenv = require('./env');
const getTurtlePort = require('./utils').getTurtlePort;
const { BrowserWindow } = require('electron').remote

console.log(appenv.env);
process.env.NODE_ENV = appenv.env;
console.log(process.execPath);

let port = getTurtlePort();
const TURTLE_SERVER_URL = 'http://localhost:' + port;

function getEnv() {
    return process.env.NODE_ENV;
}

function isProd() {
    return getEnv() == 'prod';
}

function isDev() {
    return getEnv() == 'dev';
}

function getPicoTurtleServer() {
    let execName = process.platform === "win32" ? "picoturtle-server.exe" : "picoturtle-server";
    if (isDev()) {
        return path.join(__dirname, '..', 'server', 'dist', execName);
    }
    if (isProd()) {
        return path.join(__dirname, '..', execName);
    }
}

// fs.readdirSync(path.join(__dirname, '..')).forEach(file => {
//   console.log(file);
// });

//in windows server is at ../picoturtle-server.exe

// Non-Editor Messages are handled here
// All Editor Message Callbacks are registered in the TurtleEditor Class
let about_msg = `PicoTurtle is a tiny turtle graphics program.

You can write turtle graphics programs in your favourite programming language.

Author: Abhishek Mishra
Version: 0.0.1a
Homepage: https://github.com/abhishekmishra/picoturtle
`;

ipcRenderer.on('help.about', (event, message) => {
    dialog.showMessageBox(require('electron').remote.getCurrentWindow(), {
        type: "info",
        title: 'About PicoTurtle',
        message: about_msg,
        buttons: ['ok']
    }, (response, checkboxChecked) => { });
});

list_turtles();

let show_list = 0;
let show_obj_code = 0;
if (show_list != null && show_list == 0) {
    let list_container = document.getElementById('turtle_list_container');
    list_container.hidden = true;
}

if (show_obj_code != null && show_obj_code == 0) {
    let obj_code_container = document.getElementById('turtle_details_container');
    obj_code_container.hidden = true;
}

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
    await t.penwidth(3);
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
create_turtle()

### Your code goes here ###

def square(side):
    for i in range(4):
        forward(side)
        right(90)

pendown()
penwidth(4)
square(50)

### Your code ends here ###

# Always stop the turtle
stop()`;

class TurtleEditor {
    constructor() {
        this.editor = monaco.editor.create(document.getElementById('turtle_code'), {
            value: [
                ''
            ].join('\n')
        });
        this.markVersion();
        this.editor.getModel().onDidChangeContent((event) => {
            if (this.isDirty()) {
                document.title = '* PicoTurtle';
            } else {
                document.title = 'PicoTurtle';
            }
        });
        this.setSelectedFile();
        this.setLanguage('python');
        $('#open_button').on('click', { editor: this }, this.openFile);
        $('#run_button').on('click', { editor: this }, this.run_turtle);
        $('#save_button').on('click', { editor: this }, this.saveFile);
        $('#save_as_button').on('click', { editor: this }, this.saveAsFile);
        $('#new_button').on('click', { editor: this }, this.newFile);
        $('#export_button').on('click', { editor: this }, this.export);
        $('#editor_language_select').on('change', { editor: this }, function (event) {
            event.data.editor.setLanguage(this.value);
        });
        $('#help_button').on('click', { editor: this }, this.help);

        ipcRenderer.on('file.new', (event, message) => {
            event.data = {
                editor: this
            };
            this.newFile(event);
        });

        ipcRenderer.on('file.open', (event, message) => {
            event.data = {
                editor: this
            };
            this.openFile(event);
        });

        ipcRenderer.on('file.save', (event, message) => {
            event.data = {
                editor: this
            };
            this.saveFile(event);
        });

        ipcRenderer.on('file.save_as', (event, message) => {
            event.data = {
                editor: this
            };
            this.saveAsFile(event);
        });

        ipcRenderer.on('turtle.run', (event, message) => {
            event.data = {
                editor: this
            };
            this.run_turtle(event);
        });

        ipcRenderer.on('turtle.export', (event, message) => {
            event.data = {
                editor: this
            };
            this.export(event);
        });

        ipcRenderer.on('help.docs', (event, message) => {
            event.data = {
                editor: this
            };
            this.help(event);
        });

        $(window).resize(() => {
            //$( "#log" ).append( "<div>Handler for .resize() called.</div>" );
            this.editor.layout();
        });
        this.local_turtle = new TurtleCanvas("turtle_canvas");
        this.local_turtle.drawTurtle();
    }

    markVersion() {
        this.lastSavedVersionId = this.editor.getModel().getAlternativeVersionId();
        document.title = 'PicoTurtle';
    }

    isDirty() {
        return (this.lastSavedVersionId !== this.editor.getModel().getAlternativeVersionId());
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
        if (selected_file == null) {
            this.file = 'Untitled';
            $('#save_button').prop('disabled', 'disabled');
        } else {
            this.file = selected_file;
            $('#save_button').prop('disabled', false);
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
                    editor.markVersion();
                }
            }
        });
    }

    writeContents() {
        let text = this.editor.getValue();
        fs.writeFile(this.file, text, (err) => {
            if (err) {
                return console.log(err);
            }
            console.log("The file was saved!");
            this.markVersion();
        });
    }

    saveFile(event) {
        let editor = event.data.editor;
        if (this.file != 'Untitled') {
            editor.writeContents();
        } else {
            editor.saveAsFile(event);
        }
    }

    saveAsFile(event) {
        let editor = event.data.editor;
        try {
            dialog.showSaveDialog({
                // no options
            }, (selected_file) => {
                editor.setSelectedFile(selected_file);
                console.log('Selected file ' + selected_file);
                editor.writeContents();
            });
        } catch (error) {
            console.log('Error selecting file to save - ' + error);
        }
    }

    newFile(event) {
        let editor = event.data.editor;
        editor.editor.setValue(``);
        editor.setSelectedFile();
    }

    async run_turtle(event) {
        let editor = event.data.editor;
        let text = editor.editor.getValue();

        var t = new Turtle(port = port);
        let state = await t.init();
        if (editor.language == 'javascript') {
            // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
            const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
            var f = new AsyncFunction('t', text);
            await f(t);
        } else if (editor.language == 'python') {
            //const ls = spawn('ls', ['-lh', '/usr']);
            let penv = JSON.parse(JSON.stringify(process.env));
            penv['PYTHONPATH'] = path.join(__dirname, '..', 'client', 'python');
            let options = {
                cwd: path.join(__dirname, '..'),
                env: penv
            };
            // console.log(options);
            // console.log( process.env.PATH );
            let command_args = [editor.file, state.name, port];
            if (editor.file == 'Untitled' || editor.isDirty()) {
                command_args = ['-c', text, state.name, port];
            }

            try {
                $('#turtle_console').html('');
                // console.log(command_args)
                let python_exec = 'python3';
                let isWin = process.platform === "win32";
                if (isWin) {
                    python_exec = 'python';
                }
                //console.log('will spawn ' + python_exec + ' with options ' + JSON.stringify(options));
                const py_proc = spawn(python_exec,
                    command_args,
                    options);

                py_proc.stdout.on('data', (data) => {
                    console.log(`stdout: ${data}`);
                    $('#turtle_console').append(`<pre>${data}</pre>`);
                });

                py_proc.stderr.on('data', (data) => {
                    console.log(`stderr: ${data}`);
                    $('#turtle_console').append(`<p>${data}</p>`);
                    t.stop();
                });

                py_proc.on('close', (code) => {
                    console.log(`child process exited with code ${code}`);
                    $('#turtle_console').append(`<p>child process exited with code ${code}</p>`);
                    t.stop();
                });
            } catch (error) {
                console.log(error);
            }
        }
        track_turtle(TURTLE_SERVER_URL, editor.local_turtle, state.name);
    }

    export(event) {
        let editor = event.data.editor;
        try {
            dialog.showSaveDialog({
                defaultPath: path.join(app.getPath('pictures'), 'canvas.png'),
                filters: [
                    { name: 'PNG', extensions: ['png'] }
                ]
            }, (selected_file) => {
                console.log('Selected file ' + selected_file);
                if (selected_file) {
                    editor.local_turtle.export(selected_file);
                }
            });
        } catch (error) {
            console.log('Error selecting file to save - ' + error);
        }
    }

    help(event) {
        let editor = event.data.editor;
        let win = new BrowserWindow({
            width: 800,
            height: 600,
            show: false,
            backgroundColor: 'whitesmoke',
            parent: require('electron').remote.getCurrentWindow(),
            modal: true
        });
        win.on('closed', () => {
            win = null
        });

        win.loadURL(`file://${__dirname}/help.html`);
        win.once('ready-to-show', () => {
            win.show()
        });
    }
}

// Here we setup the monaco editor.
// const path = require('path');
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
    //monaco.editor.setTheme('vs-dark');
    let t = new TurtleEditor();
});

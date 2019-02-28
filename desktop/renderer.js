// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const list_turtles = require('./turtle_canvas').list_turtles;
const track_turtle = require('@picoturtle/picoturtle-web-canvas').track_turtle_node;
const TurtleCanvas = require('@picoturtle/picoturtle-web-canvas').Turtle;
const { Turtle } = require('@picoturtle/picoturtle-nodejs-client/picoturtle-async');
const fs = require('fs');
const { dialog, app } = require('electron').remote;
const ipcRenderer = require('electron').ipcRenderer;
const path = require('path');
const appenv = require('./env');
const getTurtlePort = require('./utils').getTurtlePort;
const { BrowserWindow } = require('electron').remote;
const { NodeJSBinding } = require('./lang/node-binding');
const { PythonBinding } = require('./lang/python-binding');

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

// list_turtles();

// let show_list = 0;
// let show_obj_code = 0;
// if (show_list != null && show_list == 0) {
//     let list_container = document.getElementById('turtle_list_container');
//     list_container.hidden = true;
// }

// if (show_obj_code != null && show_obj_code == 0) {
//     let obj_code_container = document.getElementById('turtle_details_container');
//     obj_code_container.hidden = true;
// }

let START_TEMPLATES = {};
START_TEMPLATES['python'] = ``;

function turtle_console_out(data) {
    if (data != null) {
        let lines = `${data}`.match(/[^\r\n]+/g);
        lines.forEach(element => {
            $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">${element}</li>`);
        });
    }
}

global.turtle_console_out = turtle_console_out;

class TurtleEditor {
    constructor() {
        this.editor = monaco.editor.create(document.getElementById('turtle_code'), {
            value: [
                ''
            ].join('\n')
        });

        this.bindings = {
            'javascript': new NodeJSBinding(),
            'python': new PythonBinding()
        };

        this.sampleSelected = false;

        this.markVersion();
        this.editor.getModel().onDidChangeContent((event) => {
            if (this.isDirty()) {
                document.title = '* PicoTurtle';
            } else {
                document.title = 'PicoTurtle';
            }
        });
        this.setSelectedFile();
        this.setLanguage('javascript');
        $('#open_button').on('click', { editor: this }, this.openFile);
        $('#run_button').on('click', { editor: this }, this.run_turtle);
        $('#save_button').on('click', { editor: this }, function (event) {
            event.data.editor.saveFile();
        });
        $('#save_as_button').on('click', { editor: this }, function (event) {
            event.data.editor.saveAsFile();
        });
        $('#new_button').on('click', { editor: this }, this.newFile);
        $('#export_button').on('click', { editor: this }, this.export);
        $('#editor_language_select').on('change', { editor: this }, function (event) {
            event.data.editor.setLanguage(this.value);
        });
        $('#editor_sample_select').on('change', { editor: this }, function (event) {
            event.data.editor.setSample(this.value);
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
            this.saveFile();
        });

        ipcRenderer.on('file.save_as', (event, message) => {
            event.data = {
                editor: this
            };
            this.saveAsFile();
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
        if (this.language !== name) {
            this.language = name;

            monaco.editor.setModelLanguage(this.editor.getModel(), name);
            $('#editor_language_select').val(this.language);
            if (this.file != 'Untitled') {
                $('#editor_language_select').prop('disabled', 'disabled');
            } else {
                $('#editor_language_select').prop('disabled', false);
                //this.editor.setValue(START_TEMPLATES[this.language]);
            }

            $('#editor_sample_select').find('option')
                .remove()
                .end();

            this.bindings[this.language].getSamples().forEach(element => {
                $('#editor_sample_select').append('<option value="' + element.file + '">' + element.name + '</option>').val('');
            });
        }
    }

    setSample(name) {
        $('#editor_sample_select').val(name);
        this.setSelectedFile(name);
        this.sampleSelected = true;
    }

    setSelectedFile(selected_file = null) {
        this.sampleSelected = false;
        if (selected_file === null) {
            this.file = 'Untitled';
            $('#save_button').prop('disabled', 'disabled');
        }
        else if (!fs.existsSync(selected_file)) {
            this.file = selected_file;
        }
        else {
            this.file = selected_file;
            $('#save_button').prop('disabled', false);
            let text = fs.readFileSync(this.file, { encoding: 'utf-8' });
            this.editor.setValue(text);
            this.markVersion();
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
                }
            }
        });
    }

    writeContents() {
        if (this.sampleSelected) {
            dialog.showErrorBox('Cannot save sample', 'Saving the sample file is not allowed!');
            return;
        }

        let text = this.editor.getValue();
        fs.writeFileSync(this.file, text, (err) => {
            if (err) {
                return console.log(err);
            }
            console.log("The file was saved!");
            this.markVersion();
        });
    }

    saveFile() {
        if (this.file != 'Untitled' && !this.sampleSelected) {
            this.writeContents();
        } else {
            this.saveAsFile();
        }
    }

    saveAsFile() {
        try {
            dialog.showSaveDialog({
                // no options
            }, (selected_file) => {
                console.log(selected_file);
                if (selected_file) {
                    this.setSelectedFile(selected_file);
                    console.log('Selected file ' + selected_file);
                    this.writeContents();
                }
            });
        } catch (error) {
            console.log('Error saving file - ' + error);
        }
    }

    newFile(event) {
        let editor = event.data.editor;
        editor.editor.setValue(``);
        editor.setSelectedFile();
    }

    getCurrentBinding() {
        return this.bindings[this.language];
    }

    async run_turtle(event) {
        $('#turtle_console').html('');

        let editor = event.data.editor;
        let text = editor.editor.getValue();

        var t = new Turtle({
            port: port
        });
        let state = await t.init(null);
        $('#turtle_name').html(state.name);
        global.t = t;
        track_turtle(TURTLE_SERVER_URL, editor.local_turtle, state.name);

        if (editor.language == 'javascript') {
            ipcRenderer.send('exec-node', [
                editor.file,
                null,
                null,
                null,
                {
                    name: state.name,
                    port: port
                }]);
        } else {
            let binding = editor.getCurrentBinding();
            let exec = binding.execFile;
            if (editor.file == 'Untitled' || editor.isDirty()) {
                if (binding.canExecText()) {
                    exec = binding.execText();
                }
                else {
                    dialog.showErrorBox('File not saved', 'You need to save the file to run it!');
                    return Promise.resolve();
                }
            }

            exec(
                editor.file,
                (data) => {
                    console.log(`stdout: ${data}`);
                    turtle_console_out(data);
                },
                (data) => {
                    console.log(`stderr: ${data}`);
                    turtle_console_out(data);
                },
                (code) => {
                    console.log(`child process exited with code ${code}`);
                    if (parseInt(code) == 0) {
                        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program completed successfully.</li>`);
                    } else {
                        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program encountered an error [${code}].</li>`);
                    }
                    t.stop();
                },
                {
                    name: state.name,
                    port: port
                }
            ).then(() => {
                console.log('done');
            }).catch((err) => {
                console.log('Error running program -> ' + err);
            });
        }
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
            win.setMenuBarVisibility(false);
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

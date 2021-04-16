// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const track_turtle = require('@picoturtle/picoturtle-web-canvas').track_turtle_node;
const TurtleCanvas = require('@picoturtle/picoturtle-web-canvas').Turtle;
const { Turtle } = require('@picoturtle/picoturtle-nodejs-client');
const fs = require('fs');
const { Menu, dialog, app } = require('electron').remote;
const ipcRenderer = require('electron').ipcRenderer;
const path = require('path');
const appenv = require('./env');
const getTurtlePort = require('./utils').getTurtlePort;
const setTheme = require('./utils').setTheme;
const { BrowserWindow } = require('electron').remote;
const { NodeJSBinding } = require('./lang/node-binding');
const { PythonBinding } = require('./lang/python-binding');
const { CSharpBinding } = require('./lang/csharp-binding');
const { JavaBinding } = require('./lang/java-binding');
const { shell } = require('electron');
const Store = require('electron-store');
const store = require('./utils').store;

process.env.NODE_ENV = appenv.env;

let port = getTurtlePort();
const TURTLE_SERVER_URL = 'http://localhost:' + port;

// Non-Editor Messages are handled here
// All Editor Message Callbacks are registered in the TurtleEditor Class
let about_msg = `PicoTurtle is a tiny turtle graphics program.

You can write turtle graphics programs in your favourite programming language.

Author: Abhishek Mishra
Version: ${app.getVersion()}
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

$('#homelink').on('click', () => {
    shell.openExternal('https://abhishekmishra.github.io/picoturtle');
});

$('#homelink').html('PicoTurtle ' + app.getVersion());// + ' © 2019 Abhishek Mishra');

// set theme
setTheme(store.get('appearance.theme'));

store.onDidChange('appearance.theme', (newval, oldval) => {
    setTheme(newval);
});

let menuItems = Menu.getApplicationMenu().items;
menuItems.forEach((mi, i) => {
    if (mi.label == 'View') {
        console.log(mi);
        mi.submenu.items.forEach((smi) => {
            if (smi.label == 'Theme') {
                smi.submenu.items.forEach((ssmi) => {
                    if (ssmi.label.toLowerCase() == store.get('appearance.theme')) {
                        ssmi.checked = true;
                    }
                })
            }
        })
    }
});

console.log(store.path);

function turtle_console_out(data) {
    if (data !== null) {
        let lines = `${data}`.match(/[^\r\n]+/g);
        // console.log('Lines -> ' + lines);
        if (lines !== null) {
            lines.forEach(element => {
                $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">${element}</li>`);
            });
        }
    }
}

function turtle_console_error(data) {
    if (data != null) {
        let lines = `${data}`.match(/[^\r\n]+/g);
        // console.log('Lines -> ' + lines);
        if (lines !== null) {
            lines.forEach(element => {
                $('#turtle_console').append(`<li class="stderrln m-0 p-0 pl-1">${element}</li>`);
            });
        }
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
            'javascript': new NodeJSBinding(store),
            'python': new PythonBinding(store),
            'csharp': new CSharpBinding(store),
            'java': new JavaBinding(store),
        };

        this.sampleSelected = false;
        this.canRun = true;
        this.status_icons = ['starting', 'running', 'fail', 'success'];

        this.markVersion();
        this.editor.getModel().onDidChangeContent((event) => {
            if (this.isDirty()) {
                ipcRenderer.send('dirty', true);
                document.title = '* PicoTurtle';
            } else {
                ipcRenderer.send('dirty', false);
                document.title = 'PicoTurtle';
            }
        });
        this.setSelectedFile();
        this.changeLanguage('javascript');
        $('#open_button').on('click', { editor: this }, function (event) {
            event.data.editor.openFile();
        });
        $('#run_button').on('click', { editor: this }, function (event) {
            event.data.editor.run_turtle();
        });
        $('#save_button').on('click', { editor: this }, function (event) {
            event.data.editor.saveFile();
        });
        $('#save_as_button').on('click', { editor: this }, function (event) {
            event.data.editor.saveAsFile();
        });
        $('#new_button').on('click', { editor: this }, function (event) {
            event.data.editor.newFile();
        });
        $('#export_button').on('click', { editor: this }, function (event) {
            event.data.editor.export();
        });
        $('#editor_language_select').on('change', { editor: this }, function (event) {
            let canChange = event.data.editor.setLanguage(this.value);
            if (!canChange) {
                $(this).val(event.data.editor.language);
            }
        });
        $('#editor_sample_select').on('change', { editor: this }, function (event) {
            event.data.editor.setSample(this.value);
        });
        $('#help_button').on('click', { editor: this }, function (event) {
            event.data.editor.help();
        });
        $('#pref_button').on('click', { editor: this }, function (event) {
            event.data.editor.preferences();
        });

        this.turtle_options = {
            turtle_colour: 'MediumOrchid',
            draw_turtle: true,
            animate: true,
            draw_on_stop: false,
            limit: 1000,
            cmd_cb: (cmd) => {
                // do notthing for the moment
                if (cmd[0] == 'canvas_size') {
                    $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Canvas resized.</li>`);
                }
                if (cmd[0] == 'export_img') {
                    $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Canvas exported at ` + cmd[1] + `</li>`);
                }
                if (cmd[0] == 'stop') {
                    this.canRun = true;
                    $('#run_button').prop('disabled', false);
                    this.setStatusBarStatus('success');
                    $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Drawing complete.</li>`);
                }
            }
        };

        $("#animate").prop("checked", this.turtle_options.animate);
        $("#draw_on_stop").prop("checked", this.turtle_options.draw_on_stop);
        $("#draw_turtle").prop("checked", this.turtle_options.draw_turtle);

        //Turtle toolbar items
        $('#animate').on('click', { editor: this }, function (event) {
            event.data.editor.turtle_options['animate'] = $(this).prop("checked");
        });
        $('#draw_turtle').on('click', { editor: this }, function (event) {
            event.data.editor.turtle_options['draw_turtle'] = $(this).prop("checked");
        });
        $('#draw_on_stop').on('click', { editor: this }, function (event) {
            event.data.editor.turtle_options['draw_on_stop'] = $(this).prop("checked");
        });

        ipcRenderer.on('file.new', (event, message) => {
            this.newFile();
        });

        ipcRenderer.on('file.open', (event, message) => {
            this.openFile(event);
        });

        ipcRenderer.on('file.save', (event, message) => {
            this.saveFile();
        });

        ipcRenderer.on('file.save_as', (event, message) => {
            this.saveAsFile();
        });

        ipcRenderer.on('turtle.run', (event, message) => {
            this.run_turtle();
        });

        ipcRenderer.on('turtle.export', (event, message) => {
            this.export(event);
        });

        ipcRenderer.on('help.docs', (event, message) => {
            this.help(event);
        });

        ipcRenderer.on('file.preferences', (event, message) => {
            this.preferences(event);
        });

        ipcRenderer.on('view.theme', (event, message) => {
            store.set('appearance.theme', message);
            setTheme(message);
        });

        $(window).resize(() => {
            const code_div = $('#turtle_code');
            // resize code div to the size of container
            code_div.height(code_div.parent().height());
            code_div.width(code_div.parent().width()/2);
            this.editor.layout();
        });

        this.local_turtle = new TurtleCanvas("turtle_canvas");
        this.local_turtle.initOptions(this.turtle_options);
        this.local_turtle.drawTurtle();
        this.local_turtle.resetOptions();
        this.newFile();
        this.setStatusBarStatus(null);
    }

    setStatusBarStatus(status) {
        this.status_icons.forEach((val) => {
            $('#status_' + val).hide();
        });
        if (status !== null & this.status_icons.includes(status)) {
            $('#status_' + status).show();
        }
    }

    markVersion() {
        this.lastSavedVersionId = this.editor.getModel().getAlternativeVersionId();
        document.title = 'PicoTurtle';
        ipcRenderer.send('dirty', false);
    }

    isDirty() {
        return (this.lastSavedVersionId !== this.editor.getModel().getAlternativeVersionId());
    }

    getFileExtensions() {
        return this.getCurrentBinding().getFileExtensions();
    }

    getLanguageForCurrentFile() {
        if (this.file != null) {
            for (var language in this.bindings) {
                let b = this.bindings[language];
                let xts = b.getFileExtensions();
                for (var i = 0; i < xts.length; i++) {
                    console.log(this.file + ' and extension ' + xts[i]);
                    if (this.file.endsWith('.' + xts[i])) {
                        return language;
                    }
                };
            }
        }
        return null;
    }

    setLanguage(name) {
        let ok = this.confirmCloseIfEditorDirtySync();
        if (ok) {
            return this.changeLanguage(name);
        }
        return false;
    }

    changeLanguage(name) {
        if (this.language !== name) {
            let available = this.bindings[name].available();
            if (available[0] == true) {
                console.log(available[1]);
                let lang_for_current = this.getLanguageForCurrentFile();
                if (lang_for_current == null || lang_for_current == name || !this.isDirty()) {
                    this.language = name;

                    monaco.editor.setModelLanguage(this.editor.getModel(), name);
                    $('#editor_language_select').val(this.language);

                    $('#editor_sample_select').find('option')
                        .remove()
                        .end();

                    this.bindings[this.language].getSamples().forEach(element => {
                        $('#editor_sample_select').append('<option value="' + element.file + '">' + element.name + '</option>').val('');
                    });
                    return true;
                } else {
                    // $('#editor_language_select').val(this.language);
                    dialog.showErrorBox(
                        'Error changing language',
                        'Cannot change language to ' + name + ' because the file being edited is not a ' + name + ' file.'
                    );
                    return false;
                }
            }
            else {
                console.error(available[1]);
                dialog.showErrorBox(
                    'Language ' + name + ' is not available.',
                    'Cannot change language to ' + name + ' because ' + name + ' is not available.'
                    + '\n\nDetails: ' + available[1]
                    + '\n\nSetup Instructions: ' + this.bindings[name].getSetupInstructions()
                );
                return false;
            }
        }
        return true;
    }

    setSample(name) {
        this.confirmCloseIfEditorDirty(() => {
            $('#editor_sample_select').val(name);
            this.setSelectedFile(name);
            this.sampleSelected = true;
        });
    }

    setSelectedFile(selected_file = null) {
        this.sampleSelected = false;
        if (selected_file === null) {
            this.file = null;
            $('#save_button').prop('disabled', 'disabled');
        }
        else if (!fs.existsSync(selected_file)) {
            $('#save_button').prop('disabled', false);
            this.file = selected_file;
        }
        else {
            this.file = selected_file;
            $('#save_button').prop('disabled', false);
            let text = fs.readFileSync(this.file, { encoding: 'utf-8' });
            this.editor.setValue(text);
        }
        if (this.file == null) {
            $('#editor_file').html('No Name');
        } else {
            $('#editor_file').html(this.file);

            for (var lang in this.bindings) {
                if (this.file.endsWith(this.bindings[lang].getFileExtensions()[0])) {
                    this.changeLanguage(lang);
                }
            }
            // if (this.file.endsWith('.js')) {
            //     this.changeLanguage('javascript');
            // }
            // if (this.file.endsWith('.py')) {
            //     this.changeLanguage('python');
            // }
        }
        this.markVersion();
    }

    openFile() {
        this.confirmCloseIfEditorDirty(() => {
            dialog.showOpenDialog({
                properties: ['openFile']
            }, (files) => {
                if (files !== undefined) {
                    if (files.length == 1) {
                        let selected_file = files[0];
                        this.setSelectedFile(selected_file);
                    }
                }
            });
        });
    }

    writeContents() {
        if (this.sampleSelected) {
            dialog.showErrorBox('Cannot save sample', 'Saving the sample file is not allowed!');
            return;
        }

        let text = this.editor.getValue();
        try {
            fs.writeFileSync(this.file, text);
        } catch (err) {
            if (err) {
                return console.log('Error writing file ' + this.file + ', ' + err);
            }
        }
        console.log("The file was saved!");
        this.markVersion();
    }

    saveFile() {
        if (this.file !== null && !this.sampleSelected) {
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

    newFile() {
        this.confirmCloseIfEditorDirty(() => {
            this.editor.setValue(this.getCurrentBinding().getNewFileTemplate());
            this.setSelectedFile();
        });
    }

    getCurrentBinding() {
        return this.bindings[this.language];
    }

    async run_turtle() {
        this.setStatusBarStatus('starting');
        if (!this.canRun) {
            dialog.showErrorBox('Another turtle is running.', 'A turtle program is still running. You can run once it finishes.');
            return;
        }
        this.canRun = false;
        $('#run_button').prop('disabled', 'disabled');
        $('#turtle_console').html('');

        let text = this.editor.getValue();

        // Check if the file is dirty and if the current binding
        // supports running text snippets.
        // If not, the user needs to save the file before running.
        let execFile = true;
        let execText = false;
        let binding = this.getCurrentBinding();
        let exec = binding.execFile;
        let fvalue = this.file;
        if (this.file == null || this.isDirty()) {
            execFile = false;
            if (binding.canExecText()) {
                exec = binding.execText;
                fvalue = text;
                execText = true;
            }
            else {
                dialog.showErrorBox('File not saved', 'You need to save the file to run it!');
                this.canRun = true;
                $('#run_button').prop('disabled', false);
                return Promise.resolve();
            }
        }

        var t = new Turtle({
            port: port
        });
        let state = await t.init(null);
        $('#turtle_name').html(state.name);
        global.t = t;
        console.log('Turtle options');
        console.log(this.turtle_options);
        track_turtle(TURTLE_SERVER_URL, this.local_turtle, state.name, this.turtle_options);

        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program started...</li>`);
        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Drawing started...</li>`);
        this.setStatusBarStatus('running');

        //TODO: remove duplication in the following run calls
        if (execFile) {
            binding.execFile(
                fvalue,
                (data) => {
                    if (data !== null) {
                        console.log(`stdout: ${data}`);
                        turtle_console_out(data);
                    }
                },
                (data) => {
                    if (data !== null) {
                        console.log(`stderr: ${data}`);
                        turtle_console_error(data);
                    }
                },
                (code) => {
                    console.log(`child process exited with code ${code}`);
                    if (parseInt(code) == 0) {
                        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program completed successfully.</li>`);
                    } else {
                        this.setStatusBarStatus('fail');
                        $('#turtle_console').append(`<li class="stderrln m-0 p-0 pl-1">Program encountered an error [${code}].</li>`);
                        t.stop();
                    }
                },
                {
                    name: state.name,
                    port: port
                }
            ).then(() => {
                console.log('done');
            }).catch((err) => {
                console.log('Error running program -> ' + err);
                t.stop();
                this.canRun = true;
                $('#run_button').prop('disabled', false);
                this.setStatusBarStatus('fail');
            });
        }
        if (execText) {
            binding.execText(
                fvalue,
                (data) => {
                    console.log(`stdout: ${data}`);
                    turtle_console_out(data);
                },
                (data) => {
                    console.log(`stderr: ${data}`);
                    turtle_console_error(data);
                },
                (code) => {
                    console.log(`child process exited with code ${code}`);
                    if (parseInt(code) == 0) {
                        $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program completed successfully.</li>`);
                    } else {
                        this.setStatusBarStatus('fail');
                        $('#turtle_console').append(`<li class="stderrln m-0 p-0 pl-1">Program encountered an error [${code}].</li>`);
                        t.stop();
                    }
                },
                {
                    name: state.name,
                    port: port
                }
            ).then(() => {
                console.log('done');
            }).catch((err) => {
                console.log('Error running program -> ' + err);
                t.stop();
                this.canRun = true;
                $('#run_button').prop('disabled', false);
                this.setStatusBarStatus('fail');
            });
        }
    }

    export() {
        try {
            dialog.showSaveDialog({
                defaultPath: path.join(app.getPath('pictures'), 'canvas.png'),
                filters: [
                    { name: 'PNG', extensions: ['png'] }
                ]
            }, (selected_file) => {
                console.log('Selected file ' + selected_file);
                if (selected_file) {
                    this.local_turtle.export(selected_file);
                }
            });
        } catch (error) {
            console.log('Error selecting file to save - ' + error);
        }
    }

    help() {
        let win = new BrowserWindow({
            width: 800,
            height: 600,
            show: false,
            backgroundColor: 'whitesmoke',
            parent: require('electron').remote.getCurrentWindow(),
            modal: true,
            webPreferences: {
                nodeIntegration: true,
                enableRemoteModule: true,
            }          
        });

        win.on('closed', () => {
            win = null;
        });

        win.loadURL(`file://${__dirname}/help.html`);
        win.once('ready-to-show', () => {
            win.setMenuBarVisibility(false);
            win.show();
        });
    }

    preferences() {
        let win = new BrowserWindow({
            width: 800,
            height: 600,
            show: false,
            backgroundColor: 'whitesmoke',
            parent: require('electron').remote.getCurrentWindow(),
            modal: true,
            webPreferences: {
                nodeIntegration: true,
                enableRemoteModule: true,
            }          
        });

        win.on('closed', () => {
            win = null;
        });

        win.loadURL(`file://${__dirname}/preferences.html`);
        win.once('ready-to-show', () => {
            win.setMenuBarVisibility(false);
            win.show();
        });
    }

    confirmCloseIfEditorDirty(callback_on_continue) {
        if (this.isDirty()) {
            dialog.showMessageBox(require('electron').remote.getCurrentWindow(), {
                type: 'question',
                buttons: ['Yes', 'No', 'Cancel'],
                defaultId: 2,
                title: 'Save current file?',
                message: 'Your changes in the editor are not saved. Would you like to save the file before continuing?' +
                    '\nIf you choose "No" you will lose all unsaved work!.',
            }, (choice) => {
                console.log(choice);
                if (choice == 2) {
                    //Cancel and return
                    return;
                } else {
                    if (choice == 0) {
                        //Save and continue
                        this.saveFile();
                    }
                    callback_on_continue();
                }
            });
        } else {
            callback_on_continue();
        }
    }

    confirmCloseIfEditorDirtySync() {
        if (this.isDirty()) {
            let choice = dialog.showMessageBox(require('electron').remote.getCurrentWindow(), {
                type: 'question',
                buttons: ['Yes', 'No', 'Cancel'],
                defaultId: 2,
                title: 'Save current file?',
                message: 'Your changes in the editor are not saved. Would you like to save the file before continuing?' +
                    '\nIf you choose "No" you will lose all unsaved work!.',
            });
            console.log(choice);
            if (choice == 2) {
                //Cancel and return
                return false;
            } else {
                if (choice == 0) {
                    //Save and continue
                    this.saveFile();
                }
                return true;
            }
        } else {
            return true;
        }
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
    monaco.editor.setTheme('vs-' + store.get('appearance.theme'));
    let t = new TurtleEditor();
});

const { spawn, spawnSync } = require('child_process');
const path = require('path');
const { getSampleFilePath } = require('../utils');
const { env } = require('../env');

const TEMPLATE = `from picoturtle import *


if __name__ == "__main__":
    # Create the turtle before using
    create_turtle()

    # Your code goes here #


    # Your code ends here #

    # Always stop the turtle
    stop()
`;

function getPythonPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', 'client', 'python');
    } else {
        return path.join(__dirname, '..', '..', 'client', 'python');
    }
}

class PythonBinding {
    constructor(store) {
        this.store = store;
        this.initPythonPath();
    }

    initPythonPath() {
        this.pathToPython3 = this.store.get('python.pathToPython3');
        if (this.pathToPython3 === undefined) {
            let isWin = process.platform === "win32";
            if (isWin) {
                this.pathToPython3 = 'python';
            } else {
                this.pathToPython3 = 'python3';
            }
        }
        this.pythonVersionStr = '';
    }

    available() {
        this.initPythonPath();
        let is_available = false;
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            let options = {
                env: penv,
                encoding: 'utf-8'
            };
            let command_args = ['-V'];

            let python_exec = this.pathToPython3;
            console.log('will spawn ' + python_exec + ' with args ' + command_args);
            const output = spawnSync(python_exec,
                command_args,
                options);
            console.log(output);
            this.pythonVersionStr = output.stdout + output.stderr;
            let reason = 'PicoTurtle python support requires Python version 3. ';
            if (output.error) {
                reason += output.error;
            } else {
                reason += 'Current version is ' + this.pythonVersionStr;
                if (this.pythonVersionStr.startsWith('Python 3')) {
                    is_available = true;
                }
            }

            return [is_available, reason];
        } catch (error) {
            return [false, 'PicoTurtle Python support requires Python version 3. ' + error];
        }
    }

    canExecFile() {
        return true;
    }

    canExecText() {
        return true;
    }

    async execFile(file, output_cb, error_cb, complete_cb, args) {
        this.initPythonPath();
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            penv['PYTHONPATH'] = getPythonPath();
            let options = {
                cwd: path.dirname(file),
                env: penv
            };
            let command_args = [file, args.name, args.port];

            let python_exec = this.pathToPython3;
            // console.log('will spawn ' + python_exec + ' with options ' + JSON.stringify(options));
            // console.log(penv['PYTHONPATH']);
            const py_proc = spawn(python_exec,
                command_args,
                options);

            py_proc.stdout.on('data', output_cb);
            py_proc.stderr.on('data', error_cb);
            py_proc.on('close', complete_cb);
            return Promise.resolve();
        } catch (error) {
            error_cb(error);
            return Promise.reject(error);
        }
    }

    async execText(text, output_cb, error_cb, complete_cb, args) {
        this.initPythonPath();
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            penv['PYTHONPATH'] = getPythonPath();
            let options = {
                cwd: path.join(__dirname, '..'),
                env: penv
            };

            let command_args = ['-c', text, args.name, args.port];

            let python_exec = this.pathToPython3;
            //console.log('will spawn ' + python_exec + ' with options ' + JSON.stringify(options));
            const py_proc = spawn(python_exec,
                command_args,
                options);

            py_proc.stdout.on('data', output_cb);
            py_proc.stderr.on('data', error_cb);
            py_proc.on('close', complete_cb);
            return Promise.resolve();
        } catch (error) {
            error_cb(error);
            return Promise.reject(error);
        }
    }

    getSamples() {
        return [
            {
                name: "Polygons",
                file: getSampleFilePath('polygons.py')
            },
            {
                name: "Spiral",
                file: getSampleFilePath('spiral.py')
            },
            {
                name: "Tree",
                file: getSampleFilePath('tree.py')
            },
            {
                name: "Text",
                file: getSampleFilePath('text-demo.py')
            },
            {
                name: "Fern",
                file: getSampleFilePath('fern.py')
            },
            {
                name: "Sierpinski",
                file: getSampleFilePath('sierpinski.py')
            }
        ];
    }

    getNewFileTemplate() {
        return TEMPLATE;
    }

    getFileExtensions() {
        return ['py'];
    }

    getSetupInstructions() {
        return 'PicoTurtle Python support requires Python3 installed.' +
            '\nInstallation instructions vary for operating systems. Please consult someone who has worked with python to help you set it up.'
            + '\nOnce you have python3 setup. You can either add it to the system path OR you can edit the "Python->Python 3 Path" setting in the preferences dialog.'
            + '\n\nNOTE: MacOS does not allow setting paths for apps, therefore the paths have to be setup in the preferences dialog.';
    }
}

module.exports.PythonBinding = PythonBinding;

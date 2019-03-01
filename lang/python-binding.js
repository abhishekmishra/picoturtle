const { spawn } = require('child_process');
const path = require('path');
const { getSampleFilePath } = require('../utils');
const { env } = require('../env');

function getPythonPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', 'client', 'python');
    } else {
        return path.join(__dirname, '..', '..', 'client', 'python');
    }
}

class PythonBinding {
    constructor() {
        //do nothing
    }

    async available() {
        return Promise.resolve(true);
    }

    canExecFile() {
        return true;
    }

    canExecText() {
        return true;
    }

    async execFile(file, output_cb, error_cb, complete_cb, args) {
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
            let command_args = [file, args.name, args.port];

            let python_exec = 'python3';
            let isWin = process.platform === "win32";
            if (isWin) {
                python_exec = 'python';
            }
            // console.log('will spawn ' + python_exec + ' with options ' + JSON.stringify(options));
            console.log(penv['PYTHONPATH']);
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

            let python_exec = 'python3';
            let isWin = process.platform === "win32";
            if (isWin) {
                python_exec = 'python';
            }
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
                name: "Spiral",
                file: getSampleFilePath('spiral.py')
            },
            {
                name: "Polygons",
                file: getSampleFilePath('polygons.py')
            }
        ];
    }
}

module.exports.PythonBinding = PythonBinding;

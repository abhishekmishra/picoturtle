const { spawn, fork } = require('child_process');
const { getSampleFilePath } = require('../utils');
const path = require('path');
const { env } = require('../env');

var isWin = process.platform === "win32";
var isLinux = process.platform === "linux";
var isMacos = process.platform === "darwin";

const TEMPLATE = `const { create_turtle, home, clear, stop, print, state } = require('@picoturtle/picoturtle-nodejs-client');
const { font, filltext, stroketext } = require('@picoturtle/picoturtle-nodejs-client');
const { penup, pendown, penwidth, pencolour } = require('@picoturtle/picoturtle-nodejs-client');
const { left, right, forward, back } = require('@picoturtle/picoturtle-nodejs-client');
const { setpos, setx, sety, heading } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    /* Create the turtle before using */
    await create_turtle();

    /* Your code goes here */
    

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};

main()
.catch((err) => {
    console.error(err);
    process.exit(1);
});`;

function getNodejsClientExecutable() {
    let nodejs_client_exec = '';
    if (isWin) {
        nodejs_client_exec = "picoturtle-nodejs-client-win.exe";
    }
    if (isLinux) {
        nodejs_client_exec = "picoturtle-nodejs-client-linux";
    }
    if (isMacos) {
        nodejs_client_exec = "picoturtle-nodejs-client-macos";
    }
    return nodejs_client_exec;
}

function getNodejsPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', 'dist', getNodejsClientExecutable());
    } else {
        return path.join(__dirname, '..', '..', getNodejsClientExecutable());
    }
}

function getNodeModulesPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', 'node_modules');
    } else {
        return path.join(__dirname, '..', 'node_modules');
    }
}

class NodeJSBinding {
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
        return false;
    }

    async execFile(file, output_cb, error_cb, complete_cb, args) {
        await new NodeJSBinding().execFileFork(file, output_cb, error_cb, complete_cb, args);
    }

    async execFileFork(file, output_cb, error_cb, complete_cb, args) {
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';
        try {
            // let penv = JSON.parse(JSON.stringify(process.env));
            // penv['ELECTRON_RUN_AS_NODE'] = 1;
            let options = {
                env: {
                    NODE_PATH: getNodeModulesPath()
                },
                silent: false,
                detached: false,
                stdio: ['pipe', 'pipe', 'pipe', 'ipc']
            };

            // options['cwd'] = path.join(__dirname, '..', '..');

            let command_args = ['-n', args.name, '-p', args.port];

            console.log('Executing ' + file + ' with args -> ' + command_args + ' and options -> ' + options);

            const js_proc = fork(file, command_args, options);

            js_proc.stdout.on('data', output_cb);
            js_proc.stderr.on('data', error_cb);
            js_proc.on('message', output_cb);
            js_proc.on('error', error_cb);
            js_proc.on('close', complete_cb);
            return Promise.resolve();
        } catch (error) {
            error_cb(error);
            return Promise.reject(error);
        }
    }


    async execFileSpawn(file, output_cb, error_cb, complete_cb, args) {
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            penv['ELECTRON_RUN_AS_NODE'] = 1;
            let options = {
                // env: penv
            };

            options['cwd'] = path.join(__dirname, '..', '..');

            let command_args = ['-n', args.name, '-p', args.port];

            // const js_proc = fork(file, command_args, options);

            // js_proc.on('message', output_cb);
            // js_proc.on('error', error_cb);
            // js_proc.on('close', complete_cb);
            // return Promise.resolve();
            command_args.unshift(file);
            command_args.unshift('-f')
            console.log('will spawn with args ' + command_args + ', and with options ' + JSON.stringify(options));
            const js_proc = spawn(getNodejsPath(),
                command_args,
                options);

            js_proc.stdout.on('data', output_cb);
            js_proc.stderr.on('data', error_cb);
            js_proc.on('close', complete_cb);
            return Promise.resolve();
        } catch (error) {
            error_cb(error);
            return Promise.reject(error);
        }
    }

    async execText(text, output_cb, error_cb, complete_cb, args) {
        // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
        const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
        var f = new AsyncFunction('t', text);
        return await f(t);
    }

    getSamples() {
        return [
            {
                name: "Polygons",
                file: getSampleFilePath('polygons.js')
            },
            {
                name: "Spiral",
                file: getSampleFilePath('spiral.js')
            },
            {
                name: "Tree",
                file: getSampleFilePath('tree.js')
            },
            {
                name: "Text",
                file: getSampleFilePath('text-demo.js')
            },
            {
                name: "Fern",
                file: getSampleFilePath('fern.js')
            },
            {
                name: "Sierpinski",
                file: getSampleFilePath('sierpinski.js')
            }
        ];
    }

    getNewFileTemplate() {
        return TEMPLATE;
    }

    getFileExtensions() {
        return ['js'];
    }
}

module.exports.NodeJSBinding = NodeJSBinding;

// see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
// const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
// var f = new AsyncFunction('t', text);
// await f(t);

// let command_args = ['./spiral-sync.js', '-n', state.name, '-p', port];

// const js_proc = spawn('node', command_args);

// js_proc.stdout.on('data', (data) => {
//     console.log(`stdout: ${data}`);
//     turtle_console_out(data);
// });

// js_proc.stderr.on('data', (data) => {
//     console.log(`stderr: ${data}`);
//     $('#turtle_console').append(`<p class="stderrln">${data}</p>`);
//     // t.stop();
// });

// js_proc.on('close', (code) => {
//     console.log(`child process exited with code ${code}`);
//     if (parseInt(code) == 0) {
//         $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program completed successfully.</li>`);
//     } else {
//         $('#turtle_console').append(`<li class="stdoutln m-0 p-0 pl-1">Program encountered an error [${code}].</li>`);
//     }
//     t.stop();
// });


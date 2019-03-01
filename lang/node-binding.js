const { spawn, fork } = require('child_process');
const { getSampleFilePath } = require('../utils');

const TEMPLATE = `const { create_turtle, penup, pendown, penwidth, clear, stop, pencolour, forward, right, left, print } = require('@picoturtle/picoturtle-nodejs-client');

let main = async () => {
    await create_turtle();
    /* Your code goes here */
    

    /* Your code ends here */

    /* Always stop the turtle */
    await stop();
};
main();`;

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
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';
        try {
            let command_args = ['-n', args.name, '-p', args.port];

            const js_proc = fork(file, command_args);

            js_proc.on('message', output_cb);
            js_proc.on('error', error_cb);
            js_proc.on('close', complete_cb);
            return Promise.resolve();
            // command_args.shift(file);
            // console.log(command_args);
            // const js_proc = spawn(process.execPath,
            //     command_args,
            //     {
            //         detached: true,
            //         env: {
            //             ELECTRON_RUN_AS_NODE: 1
            //         }
            //     });

            // js_proc.stdout.on('data', (data) => {
            //     console.log(data);
            // });
            // js_proc.stderr.on('data', (data) => {
            //     console.log(data);
            // });
            // js_proc.on('close', (data) => {
            //     console.log(data);
            // });
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
                name: "Spiral",
                file: getSampleFilePath('spiral.js')
            },
            {
                name: "Polygons",
                file: getSampleFilePath('polygons.js')
            }
        ];
    }

    getNewFileTemplate() {
        return TEMPLATE;
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


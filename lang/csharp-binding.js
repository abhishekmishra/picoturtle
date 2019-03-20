const { spawn } = require('child_process');
const path = require('path');
const { getSampleFilePath } = require('../utils');
const { env } = require('../env');
const tmp = require('tmp');
const fs = require('fs');

const TEMPLATE = `using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = new picoturtle.Turtle();
            t.Init(250, 250);

            // Your code goes here


            // Your code ends here

            // Always stop the turtle
            t.stop();
        }
    }
}
`;

function getCSharpPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', 'client', 'csharp');
    } else {
        return path.join(__dirname, '..', '..', 'client', 'csharp');
    }
}

function getDotNetBinPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', '..', 'picoturtle-dotnet', 'picoturtle-dotnet', 'picoturtle', 'bin', 'Debug');
    } else {
        return null;
    }
}

class CSharpBinding {
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

        // create temp directory
        var tmpobj = tmp.dirSync();
        console.log('Dir: ', tmpobj.name);

        // transfer project files
        fs.readdirSync(getCSharpPath()).forEach(file => {
            console.log('copying sample file -> ', file);
            if (file.endsWith('sln') || file.endsWith('csproj')) {
                fs.copyFileSync(path.join(getCSharpPath(), file), path.join(tmpobj.name, path.basename(file)));
            }
        });

        // change paths in project file
        let inputFile = path.join(tmpobj.name, 'cs-pico.csproj');
        fs.readFile(inputFile, 'utf8', function (err, data) {
            if (err) {
                return console.log(err);
            }
            var result = data.replace(/PICOTURTLE_DOTNET_BIN/g, getDotNetBinPath());
            result = result.replace(/PROGRAM_FILE/g, path.basename(file));
            // console.log(result);
            fs.writeFileSync(inputFile, result);
        });

        // copy program file
        fs.copyFileSync(file, path.join(tmpobj.name, path.basename(file)));

        console.log(tmpobj.name);

        // execute
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            let options = {
                cwd: path.dirname(file),
                env: penv
            };
            let command_args = [file, args.name, args.port];

            let python_exec = 'python3';
            let isWin = process.platform === "win32";
            if (isWin) {
                python_exec = 'python';
            }
            console.log('will spawn ' + python_exec + ' with options ' + JSON.stringify(options));
            console.log(penv['PYTHONPATH']);
            const py_proc = spawn(python_exec,
                command_args,
                options);

            py_proc.stdout.on('data', output_cb);
            py_proc.stderr.on('data', error_cb);
            py_proc.on('close', complete_cb);

            // cleanup temp directory
            //tmpobj.removeCallback();

            return Promise.resolve();
        } catch (error) {
            error_cb(error);
            return Promise.reject(error);
        }
    }

    async execText(text, output_cb, error_cb, complete_cb, args) {
        return Promise.reject(error);
    }

    getSamples() {
        return [
            {
                name: "Polygons",
                file: getSampleFilePath('polygons.cs')
            },
            {
                name: "Spiral",
                file: getSampleFilePath('spiral.cs')
            },
            {
                name: "Tree",
                file: getSampleFilePath('tree.cs')
            },
            {
                name: "Text",
                file: getSampleFilePath('text-demo.cs')
            },
            {
                name: "Fern",
                file: getSampleFilePath('fern.cs')
            },
            {
                name: "Sierpinski",
                file: getSampleFilePath('sierpinski.cs')
            }
        ];
    }

    getNewFileTemplate() {
        return TEMPLATE;
    }

    getFileExtensions() {
        return ['cs'];
    }
}

module.exports.CSharpBinding = CSharpBinding;

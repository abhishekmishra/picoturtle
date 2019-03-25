const { spawn, execFileSync, spawnSync } = require('child_process');
const path = require('path');
const { getSampleFilePath } = require('../utils');
const { env } = require('../env');
const tmp = require('tmp');
const fs = require('fs-extra');
const rimraf = require('rimraf');

var isWin = process.platform === "win32";

const TEMPLATE = `using System;
using picoturtle;

namespace cspico
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            // Create the turtle before using
            Turtle t = picoturtle.Turtle.CreateTurtle(args);
            
            if (t != null) {
                // Your code goes here


                
                // Your code ends here

                // Always stop the turtle
                t.stop();
            }
            else {
                Console.Error.WriteLine("Error: Unable to create a turtle.");
                Environment.Exit(-1);
            }
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

// function getDotNetBinPath() {
//     if (env == 'dev') {
//         return path.join(__dirname, '..', '..', 'picoturtle-dotnet', 'picoturtle-dotnet', 'picoturtle', 'bin', 'Debug').replace(/\//g, '\\');
//     } else {
//         return null;
//     }
// }

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
        // fs.readdirSync(getCSharpPath()).forEach(file => {
        //     console.log('copying sample file -> ', file);
        //     if (file.endsWith('sln') || file.endsWith('csproj')) {
        //         fs.copyFileSync(path.join(getCSharpPath(), file), path.join(tmpobj.name, path.basename(file)));
        //     }
        // });

        fs.copySync(getCSharpPath(), tmpobj.name);

        // change paths in project file
        let inputFile = path.join(tmpobj.name, 'cs-pico.csproj');
        var data = fs.readFileSync(inputFile, 'utf8');
        var result = data.replace(/PROGRAM_FILE/g, path.basename(file));
        // console.log(result);
        fs.writeFileSync(inputFile, result);

        // copy program file
        fs.copyFileSync(file, path.join(tmpobj.name, path.basename(file)));

        console.log(tmpobj.name);

        // build project in temp folder

        let build_good = true;
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            let options = {
                cwd: tmpobj.name,
                env: penv
            };
            let command_args = ['cs-pico.csproj'];

            let msbuild_exec = 'msbuild';
            console.log('will exec ' + msbuild_exec + ' with options ' + JSON.stringify(options));
            const msbuild_proc = spawnSync(msbuild_exec,
                command_args,
                options);

            console.log(`${msbuild_proc.stdout}`);
            if (msbuild_proc.stderr != null) {
                console.log(`${msbuild_proc.stderr}`);
            }
            if (msbuild_proc.status != 0) {
                build_good = false;
                error_cb('Build failed for ' + file);
                error_cb(`${msbuild_proc.stdout}`);
                //cleanup temp directory
                console.log('Deleting ' + tmpobj.name);
                rimraf(tmpobj.name, () => { console.log('Deleted') });
                return Promise.reject('Build failed for ' + file);
            } else {
                output_cb('Build successful.');
            }
            // msbuild_proc.stdout.on('data', output_cb);
            // msbuild_proc.stderr.on('data', error_cb);
            // msbuild_proc.on('close', complete_cb);
        } catch (error) {
            error_cb(error);
        }


        // execute
        if (build_good) {
            try {
                let penv = JSON.parse(JSON.stringify(process.env));
                let options = {
                    cwd: path.dirname(file),
                    env: penv
                };
                let command_args = [path.join(tmpobj.name, 'bin', 'Debug', 'cs-pico.exe'), '-n', args.name, '-p', args.port];
                let dotnet_exec = 'mono';

                // we don't need mono to run if we are on windows.
                if(isWin) {
                    command_args = ['-n', args.name, '-p', args.port];
                    dotnet_exec = path.join(tmpobj.name, 'bin', 'Debug', 'cs-pico.exe');
                }

                console.log('will spawn ' + dotnet_exec);
                const cs_proc = spawn(dotnet_exec,
                    command_args,
                    options);

                cs_proc.stdout.on('data', output_cb);
                cs_proc.stderr.on('data', error_cb);
                cs_proc.on('error', (code) => {
                    //cleanup temp directory
                    error_cb(code);
                    console.log('Deleting ' + tmpobj.name);
                    rimraf(tmpobj.name, () => { console.log('Deleted') });
                });
                cs_proc.on('close', complete_cb);
                cs_proc.on('close', (code) => {
                    //cleanup temp directory
                    console.log('Deleting ' + tmpobj.name);
                    rimraf(tmpobj.name, () => { console.log('Deleted') });
                });

                return Promise.resolve();
            } catch (error) {
                error_cb(error);
                //cleanup temp directory
                console.log('Deleting ' + tmpobj.name);
                rimraf(tmpobj.name, () => { console.log('Deleted') });
                return Promise.reject(error);
            }
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

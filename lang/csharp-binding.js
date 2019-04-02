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
    constructor(store) {
        this.store = store;
        this.initPaths();
    }

    initPaths() {
        this.pathToMsBuild = this.store.get('csharp.pathToMsBuild');
        if (this.pathToMsBuild === undefined || this.pathToMsBuild === '') {
            this.pathToMsBuild = 'msbuild';
        }
        this.pathToMono = this.store.get('csharp.pathToMono');
        if (this.pathToMono === undefined || this.pathToMono === '') {
            this.pathToMono = 'mono';
        }
        this.msBuildVersionStr = '';
    }

    available() {
        this.initPaths();
        let is_available = false;
        try {
            let penv = JSON.parse(JSON.stringify(process.env));
            let options = {
                env: penv,
                encoding: 'utf-8'
            };
            let command_args = ['/ver', '/nologo'];

            let msbuild_exec = this.pathToMsBuild;
            console.log('will spawn ' + msbuild_exec + ' with args ' + command_args);
            const output = spawnSync(msbuild_exec,
                command_args,
                options);
            console.log(output);
            this.msBuildVersionStr = output.stdout + output.stderr;
            let reason = 'PicoTurtle C# support requires msbuild. Current msbuild version is ' + this.msBuildVersionStr;
            if (output.error != null) {
                is_available = false;
                reason = 'PicoTurtle C# support requires msbuild. ' + output.error;
            } else {
                is_available = true;
            }
            if (is_available && !isWin) {
                try {
                    penv = JSON.parse(JSON.stringify(process.env));
                    options = {
                        env: penv,
                        encoding: 'utf-8'
                    };
                    command_args = ['-V'];

                    let mono_exec = this.pathToMono;
                    console.log('will spawn ' + mono_exec + ' with args ' + command_args);
                    const output = spawnSync(mono_exec,
                        command_args,
                        options);
                    console.log(output);
                    let mono_version = output.stdout + output.stderr;
                    reason += '\n\nPicoTurtle C# support on MacOs/Linux requires mono. Current mono version is ' + mono_version;
                    if (output.error != null) {
                        is_available = false;
                        reason = 'PicoTurtle C# support requires mono. ' + output.error;
                    } else {
                        is_available = true;
                    }
                } catch (error) {
                    return [false, 'PicoTurtle C# on MacOs/Linux requires mono. ' + error];
                }
            }
            return [is_available, reason];
        } catch (error) {
            return [false, 'PicoTurtle C# support requires msbuild. ' + error];
        }
    }

    canExecFile() {
        return true;
    }

    canExecText() {
        return false;
    }

    async execFile(file, output_cb, error_cb, complete_cb, args) {
        this.initPaths();
        if (!args) args = {};
        if (!args.name) args.name = null;
        if (!args.port) args.port = '3000';

        // create temp directory
        var tmpobj = tmp.dirSync();
        console.log('Dir: ', tmpobj.name);

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

            let msbuild_exec = this.pathToMsBuild;
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
                let dotnet_exec = this.pathToMono;

                // we don't need mono to run if we are on windows.
                if (isWin) {
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

    getSetupInstructions() {
        return 'PicoTurtle C# support requires MsBuild, and on MacOs/Linux additionally requires Mono.'
            + '\nOn Windows/Macos you need to install Visual Studio. (https://visualstudio.microsoft.com/)'
            + '\nOn Linux you need to install Mono. (https://www.mono-project.com/)'
            + '\nOnce installed, you can either add MsBuild (and Mono) to PATH or set the path in the preferences dialog.'
            + '\n\nNOTE: MacOS does not allow setting paths for apps, therefore the paths have to be setup in the preferences dialog.';
    }
}

module.exports.CSharpBinding = CSharpBinding;

const { spawn, execFileSync, spawnSync } = require('child_process');
const path = require('path');
const { getSampleFilePath } = require('../utils');
const { env } = require('../env');
const tmp = require('tmp');
const fs = require('fs-extra');
const rimraf = require('rimraf');

var isWin = process.platform === "win32";

var pathSep = ':';
if(isWin) {
    pathSep = ';';
}

const TEMPLATE = `import in.abhishekmishra.picoturtle.Turtle;
import in.abhishekmishra.picoturtle.TurtleState;

public class <<ClassName>> //change this to match the file name
{
    public static void main(String[] args)
    {
        // Create the turtle before using
        Turtle t = Turtle.CreateTurtle(args);
        
        if (t != null) {
            // Your code goes here


            
            // Your code ends here

            // Always stop the turtle
            t.stop();
        }
        else {
            System.out.println("Error: Unable to create a turtle.");
            System.exit(-1);
        }
    }
}
`;

function getPicoturtleJavaLibPath() {
    if (env == 'dev') {
        return path.join(__dirname, '..', '..', 'picoturtle-java', 'target', 'picoturtle-java-0.0.1-SNAPSHOT-jar-with-dependencies.jar');
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

class JavaBinding {
    constructor(store) {
        this.store = store;
        this.initPaths();
    }

    initPaths() {
        this.pathToJDK = this.store.get('pathToJDK');
        if (this.pathToJDK === undefined || this.pathToJDK === '') {
            this.pathToJDK = '';
        }
        if(isWin) {
            this.pathToJavac = path.join(this.pathToJDK, 'javac.exe');
            this.pathToJava = path.join(this.pathToJDK, 'java.exe');
        } else {
            this.pathToJavac = path.join(this.pathToJDK, 'javac');
            this.pathToJava = path.join(this.pathToJDK, 'java');
        }
        this.javaVersionStr = '';
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
            let command_args = ['-version'];

            let javac_exec = this.pathToJavac;
            console.log('will spawn ' + javac_exec + ' with args ' + command_args);
            const output = spawnSync(javac_exec,
                command_args,
                options);
            console.log(output);
            //this.msBuildVersionStr = output.stdout + output.stderr;
            let reason = 'PicoTurtle Java Support Requires javac.';
            if (output.error != null) {
                is_available = false;
                reason = 'PicoTurtle Java Support Requires javac. ' + output.error;
            } else {
                is_available = true;
            }
            if (is_available) {
                try {
                    penv = JSON.parse(JSON.stringify(process.env));
                    options = {
                        env: penv,
                        encoding: 'utf-8'
                    };
                    command_args = ['-version'];

                    let java_exec = this.pathToJava;
                    console.log('will spawn ' + java_exec + ' with args ' + command_args);
                    const output = spawnSync(java_exec,
                        command_args,
                        options);
                    console.log(output);
                    let mono_version = output.stdout + output.stderr;
                    reason += '\n\nPicoTurtle Java Support Requires java. Current java version is ' + mono_version;
                    if (output.error != null) {
                        is_available = false;
                        reason = 'PicoTurtle Java Support Requires java. ' + output.error;
                    } else {
                        is_available = true;
                    }
                } catch (error) {
                    return [false, 'PicoTurtle Java Support Requires java. ' + error];
                }
            }
            return [is_available, reason];
        } catch (error) {
            return [false, 'PicoTurtle Java Support Requires javac. ' + error];
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
            let command_args = ['-encoding', 'UTF-8', '-classpath', '.' + pathSep + getPicoturtleJavaLibPath(), path.basename(file)];

            let javac_exec = this.pathToJavac;
            console.log('will exec ' + javac_exec + ' with args ' + command_args + ' and options ' + JSON.stringify(options));
            const javac_proc = spawnSync(javac_exec,
                command_args,
                options);

            if (javac_proc.stdout !== null) {
                console.log(`${javac_proc.stdout}`);
            }
            if (javac_proc.stderr !== null) {
                console.log(`${javac_proc.stderr}`);
            }
            if (javac_proc.status != 0) {
                build_good = false;
                error_cb('Build failed for ' + file);
                error_cb(javac_proc.stderr);
                error_cb(javac_proc.stdout);
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
            build_good = false;
            error_cb(error);
            console.log('Deleting ' + tmpobj.name);
            rimraf(tmpobj.name, () => { console.log('Deleted') });
            return Promise.reject('Build failed for ' + file);
    }


        // execute
        if (build_good) {
            try {
                let penv = JSON.parse(JSON.stringify(process.env));
                let options = {
                    cwd: tmpobj.name,
                    env: penv
                };
                let command_args = ['-Dfile.encoding=UTF-8', '-cp', '.' + pathSep + getPicoturtleJavaLibPath(), path.basename(file).replace('.java', ''), '-n', args.name, '-p', args.port];
                let java_exec = this.pathToJava;

                console.log('will spawn ' + java_exec + ' with args ' + command_args + ' and options ' + JSON.stringify(options));
                const java_proc = spawn(java_exec,
                    command_args,
                    options);

                java_proc.stdout.on('data', output_cb);
                java_proc.stderr.on('data', error_cb);
                java_proc.on('error', (code) => {
                    //cleanup temp directory
                    error_cb(code);
                    console.log('Deleting ' + tmpobj.name);
                    rimraf(tmpobj.name, () => { console.log('Deleted') });
                });
                java_proc.on('close', complete_cb);
                java_proc.on('close', (code) => {
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
                file: getSampleFilePath('polygons.java')
            },
            {
                name: "Spiral",
                file: getSampleFilePath('spiral.java')
            },
            {
                name: "Tree",
                file: getSampleFilePath('tree.java')
            },
            {
                name: "Text",
                file: getSampleFilePath('textdemo.java')
            },
            {
                name: "Fern",
                file: getSampleFilePath('fern.java')
            },
            {
                name: "Sierpinski",
                file: getSampleFilePath('sierpinski.java')
            }
        ];
    }

    getNewFileTemplate() {
        return TEMPLATE;
    }

    getFileExtensions() {
        return ['java'];
    }

    getSetupInstructions() {
        return 'PicoTurtle Java support requires javac (the compiler) and java (the runtime).'
            + '\nYou can install Oracle JDK for your platform (https://www.oracle.com/technetwork/java/javase/downloads/index.html).'
            + '\nOnce installed, you can either add the JDK bin folder to PATH or set the path in the preferences dialog.'
            + '\n\nNOTE: MacOS does not allow setting paths for apps, therefore the paths have to be setup in the preferences dialog.';
    }
}

module.exports.JavaBinding = JavaBinding;

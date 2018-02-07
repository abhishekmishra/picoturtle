// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

require('bootstrap');

const turtle = require('./src/turtle.js');
const turtle_editor = require('./src/turtle_editor.js');
const turtle_commands = require('./src/turtle_commands.js');
const turtle_config = require('./src/turtle_config.js');

let tconfig = new turtle_config.TurtleConfig();
let teditor = new turtle_editor.TurtleEditor();

const BiwaScheme = require("biwascheme"); 
BiwaScheme.run_file("./src/lib/scm/scheme-turtle-bindings.scm");
BiwaScheme.run_file("./src/lib/scm/basic-shapes.scm");
// BiwaScheme.run_file("./src/test/scm/basic-shapes-test.scm");

let t = turtle.t;
let tcmd = new turtle_commands.TurtleCommands(t, teditor);

module.exports.tcmd = tcmd;
module.exports.teditor = teditor;
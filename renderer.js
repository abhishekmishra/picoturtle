// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

console.log('we are in the index window file');
require('bootstrap');

// let Turtle = require('./src/turtle');
// let t = new Turtle('turtleCanvas');
// t.forward(50);
// t.right(-90);
// t.forward(50);
// t.right(-90);
// t.forward(50);
// t.right(-90);
// t.forward(50);

// function circle(t) {
//     for(var i = 0; i < 3600; i++) {
//         t.forward(.1);
//         t.right(.1);
//     }
// }

// circle(t);

let BiwaScheme = require("biwascheme"); 
//console.log(BiwaScheme.run("(+ 1 2)")); // or 
BiwaScheme.run_file("./src/scm/first.scm");
let $ = require('jquery');
let Point = require('./point');

class Turtle {

    constructor(canvas_id, name = 'picoturtle') {
        this.canvas_id = canvas_id;
        this.name = name;
        this.location = null;
        this.angle = 90;
        this.initCanvas();
        this.initTurtle();

        this.history = [];

        this.batchEnabled = false;
    }

    initCanvas() {
        this.canvas = $('#' + this.canvas_id)[0];
        this.width = this.canvas.width;
        this.height = this.canvas.height;
        this.ctx = this.canvas.getContext('2d');
    }

    initTurtle() {
        this.location = new Point(this.width / 2, this.height / 2);
        this.ctx.beginPath();
        this.ctx.moveTo(this.location.x, this.location.y);
        this.ctx.stroke();
        this.angle = 0;
        this.penDown();
        this.drawTurtle();
    }

    penUp() {
        this.penup = true;
    }

    penDown() {
        this.penup = false;
    }

    forward(d) {
        let theta = this.angle * Math.PI / 180;
        // y2 = d sin (theta) + y1
        // x2 = d cos (theta) + x1 
        let y2 = d * (Math.sin(theta)) + this.location.y;
        let x2 = d * (Math.cos(theta)) + this.location.x;

        // console.log('current point is ' + this.location.x + ', ' + this.location.y + ', new point is ' + x2 + ', ' + y2);
        this.ctx.beginPath();

        this.ctx.moveTo(this.location.x, this.location.y);
        if (this.penup) {
            this.ctx.moveTo(x2, y2);
        } else {
            this.ctx.lineTo(x2, y2);
        }

        this.location = new Point(x2, y2);
        this.ctx.stroke();
    }

    right(angle) {
        this.angle = this.angle + angle;
    }

    //style methods

    color(pencolor = 'black', turtlecolor = 'black') {
        this.ctx.strokeStyle = pencolor;
    }

    clear() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        // this.initTurtle();
    }

    reset() {
        this.clear();
        this.clearHistory();
        this.drawTurtle();
    }

    addToHistory(cmd) {
        this.history.push(cmd);
    }

    clearHistory() {
        this.history = [];
    }

    drawTurtle() {
        this.ctx.save();
        let color = this.ctx.strokeStyle;
        this.ctx.strokeStyle = 'black';
        this.ctx.fillStyle = 'black';
        this.ctx.lineWidth = 3;
        // this.ctx.fillRect(this.location.x - 5, this.location.y - 5, 10, 10);

        let d = 25;
        let theta1 = (this.angle - 145) * Math.PI / 180;
        let y2 = d * (Math.sin(theta1)) + this.location.y;
        let x2 = d * (Math.cos(theta1)) + this.location.x;

        let theta2 = (this.angle + 145) * Math.PI / 180;
        let y3 = d * (Math.sin(theta2)) + this.location.y;
        let x3 = d * (Math.cos(theta2)) + this.location.x;

        this.ctx.beginPath();
        this.ctx.moveTo(this.location.x, this.location.y);
        this.ctx.lineTo(x2, y2);
        this.ctx.lineTo(x3, y3);
        this.ctx.lineTo(this.location.x, this.location.y);
        this.ctx.stroke();

        this.color(color);
        this.ctx.restore();
    }

    exec() {
        if (arguments.length > 0) {
            if (!this.batchEnabled) {
                this.clear();

                for (let i = 0; i < this.history.length; i++) {
                    var cmdArgs = this.history[i];
                    if (cmdArgs.length == 1) {
                        this[cmdArgs[0]]();
                    } else if (cmdArgs.length == 2) {
                        this[cmdArgs[0]](cmdArgs[1]);
                    }
                }
            }

            let command = arguments[0];
            this.addToHistory(arguments);
            if (arguments.length == 1) {
                this[command]();
            } else if (arguments.length == 2) {
                this[command](arguments[1]);
            }
            if (!this.batchEnabled) {
                this.drawTurtle();
            }
        }
    }

    batchStart() {
        this.batchEnabled = true;
        this.clear();

        for (let i = 0; i < this.history.length; i++) {
            var cmdArgs = this.history[i];
            if (cmdArgs.length == 1) {
                this[cmdArgs[0]]();
            } else if (cmdArgs.length == 2) {
                this[cmdArgs[0]](cmdArgs[1]);
            }
        }
    }

    batchExec() {
        if (arguments.length > 0) {
            let command = arguments[0];
            this.addToHistory(arguments);
            if (arguments.length == 1) {
                this[command]();
            } else if (arguments.length == 2) {
                this[command](arguments[1]);
            }
        }
    }

    batchEnd() {
        this.drawTurtle();
        this.batchEnabled = false;
    }
}

let t = null;

function createTurtle(canvas_id) {
    let t = new Turtle(canvas_id);
    module.exports.t = t;
    return t;
}

module.exports.Turtle = Turtle;
module.exports.createTurtle = createTurtle;

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
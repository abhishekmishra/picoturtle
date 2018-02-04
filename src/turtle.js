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
    }

    penUp() {
        this.penup = true;
    }

    penDown() {
        this.penup = false;
    }

    forward(d) {
        let slope = Math.tan(this.angle * Math.PI / 180);
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

    color(pencolor='black', turtlecolor='black') {
        this.ctx.strokeStyle = pencolor;
    }

    reset() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.initTurtle();
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
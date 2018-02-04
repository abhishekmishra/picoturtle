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
        console.log(this.canvas_id);
        this.canvas = $('#' + this.canvas_id)[0];
        console.log(this.canvas);
        this.width = this.canvas.width;
        this.height = this.canvas.height;
        console.log('Canvas size is ' + this.width + ', ' + this.height);
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
        console.log('stroke color is now ' + pencolor);
        this.ctx.strokeStyle = pencolor;
    }
}

function createTurtle(canvas_id) {
    return new Turtle(canvas_id);
}

module.exports.Turtle = Turtle;
module.exports.createTurtle = createTurtle;

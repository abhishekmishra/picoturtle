let $ = require('jquery');

class Turtle {

    constructor(canvas_id, name = 'picoturtle') {
        this.canvas_id = canvas_id;
        this.name = name;
        this.initCanvas();
        this.initTurtle();
    }

    initCanvas() {
        console.log(this.canvas_id);
        this.canvas = $('#' + this.canvas_id)[0];
        console.log(this.canvas);
        this.width = this.canvas.width;
        this.height = this.canvas.height;
        this.ctx = this.canvas.getContext('2d');
    }

    initTurtle() {
        let c = new Point(this.width/2, this.height/2);
        this.ctx.fillStyle = 'rgb(200,0,0)';
        this.ctx.fillRect(c.x, c.y, 10, 10);
    }
}

class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

module.exports = Turtle;

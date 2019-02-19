var express = require('express');
var router = express.Router();
const uuidv4 = require('uuid/v4');
const generate = require('project-name-generator');

TURTLES = {};

class Colour {
    constructor(r, g, b, a = 1.0) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }
}

class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    toString() {
        return '[' + this.x + ', ' + this.y + ']';
    }
}

class TurtleCommand {
    constructor(cmd, args, doit) {
        this.cmd = cmd;
        this.args = args;
        this.doit = doit;
    }
}

class DummyTurtleCanvas {
    constructor() {
        this.id = uuidv4();
        this.name = generate({ number: true }).dashed;
    }

    line(p1, p2) {
        console.log('Line from ' + p1 + ' to ' + p2);
        // this.ctx.beginPath();

        // this.ctx.moveTo(this.location.x, this.location.y);
        // if (this.penup) {
        //     this.ctx.moveTo(x2, y2);
        // } else {
        //     this.ctx.lineTo(x2, y2);
        // }

        // this.ctx.stroke();
    }
}

class Turtle {
    constructor() {
        this.location = new Point(100, 100);
        this.colour = new Colour(0, 0, 0);
        this.angle = 90;
        this.pen = true;
        this.id = uuidv4();
        this.name = generate({ number: true }).dashed;
        this.history = [];
        this.canvas = new DummyTurtleCanvas();
    }

    command(name, args) {
        this.history.push(new TurtleCommand(name, args, () => {
            this._command(name, args);
        }));
        this._command(name, args);
    }

    _command(name, args) {
        this[name].apply(this, args);
    }

    penup() {
        this.pen = false;
    }

    pendown() {
        this.pen = true;
    }

    left(angle) {
        // this.history.push(new TurtleCommand('left', [angle]));
        this.angle += angle;
    }

    right(angle) {
        this.angle -= angle;
    }

    forward(distance) {
        // TODO: draw and change position
        let theta = this.angle * Math.PI / 180;
        // y2 = d sin (theta) + y1
        // x2 = d cos (theta) + x1 
        let y2 = distance * (Math.sin(theta)) + this.location.y;
        let x2 = distance * (Math.cos(theta)) + this.location.x;
        let new_location = new Point(x2, y2);
        console.log('current point is ' + this.location + ', new point is ' + new_location);
        if (this.pen) {
            this.canvas.line(this.location, new_location);
        }
        this.location = new_location;
    }
}

function create_turtle() {
    var t = new Turtle();
    TURTLES[t.name] = t;
    return t;
}

function get_turtle_by_name(name) {
    if (name in TURTLES) {
        return TURTLES[name];
    } else {
        return null;
    }
}

/* GET users listing. */
router.get('/create', function (req, res, next) {
    var t = create_turtle();
    // t.command('penup', null);
    // t.command('forward', [100]);
    // t.command('pendown', null);
    // t.command('forward', [100]);
    // t.command('right', [90]);
    // t.command('forward', [100]);

    res.send(t);
});

router.get('/:name/penup', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('penup', null);
    res.send(t);
});

router.get('/:name/pendown', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('pendown', null);
    res.send(t);
});

router.get('/:name/left/:angle', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('left', [parseFloat(req.params['angle'])]);
    res.send(t);
});

router.get('/:name/right/:angle', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('right', [parseFloat(req.params['angle'])]);
    res.send(t);
});

router.get('/:name/forward/:distance', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('forward', [parseInt(req.params['distance'])]);
    res.send(t);
});

module.exports = router;

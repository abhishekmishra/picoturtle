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
    constructor(id, cmd, args, doit) {
        this.id = id;
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
    constructor(x = 100, y = 100) {
        this.location = new Point(x, y);
        this.colour = new Colour(0, 0, 0);
        this.angle = 90;
        this.pen = true;
        this.id = uuidv4();
        this._generated_name = generate({ number: true })
        this.name = this._generated_name.dashed;
        this.history = [];
        this.canvas = new DummyTurtleCanvas();
        this.last = -1;
    }

    command(name, args) {
        this.history.push(new TurtleCommand(this.history.length - 1, name, args, () => {
            this._command(name, args);
        }));
        this._command(name, args);
    }

    _command(name, args) {
        this[name].apply(this, args);
    }

    state() {
        return {
            location: this.location,
            colour: this.colour,
            angle: this.angle,
            pen: this.pen,
            id: this.id,
            name: this.name,
            last: this.last
        }
    }

    penup() {
        this.pen = false;
    }

    pendown() {
        this.pen = true;
    }

    left(angle) {
        this.angle += angle;
    }

    right(angle) {
        this.angle -= angle;
    }

    forward(distance) {
        let theta = this.angle * Math.PI / 180;
        let y2 = distance * (Math.sin(theta)) + this.location.y;// y2 = d sin (theta) + y1
        let x2 = distance * (Math.cos(theta)) + this.location.x;// x2 = d cos (theta) + x1 
        let new_location = new Point(x2, y2);

        console.log('current point is ' + this.location + ', new point is ' + new_location);

        if (this.pen) {
            this.canvas.line(this.location, new_location);
        }

        this.location = new_location;
    }

    stop() {
        this.last = this.history.length - 1;
    }
}

function create_turtle(x = 100, y = 100) {
    var t = new Turtle(x, y);
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

router.get('/create', function (req, res, next) {
    if ('x' in req.query && 'y' in req.query) {
        var t = create_turtle(parseInt(req.query.x), parseInt(req.query.y));
        res.send(t.state());
    } else {
        var t = create_turtle();
        res.send(t.state());
    }
});

router.get('/:name/stop', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('stop', null);
    res.send(t.state());
});

router.get('/:name/penup', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('penup', null);
    res.send(t.state());
});

router.get('/:name/pendown', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('pendown', null);
    res.send(t.state());
});

router.get('/:name/left', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('left', [parseFloat(req.query.a)]);
    res.send(t.state());
});

router.get('/:name/right', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('right', [parseFloat(req.query.a)]);
    res.send(t.state());
});

router.get('/:name/forward', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('forward', [parseInt(req.query.d)]);
    res.send(t.state());
});

router.get('/:name/command', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    var id_req = parseInt(req.query.id);
    if (t.history.length <= id_req) {
        res.send({
            hasmore: false,
            turtle: t.state()
        });
    } else {
        var cmd = t.history[id_req];
        cmd['turtle'] = t.state();
        if (t.history.length <= (id_req + 1)) {
            cmd['hasmore'] = false;
        } else {
            cmd['hasmore'] = true;
        }
        res.send(cmd);
    }
});


module.exports = router;

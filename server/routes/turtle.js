var express = require('express');
var router = express.Router();
const uuidv4 = require('uuid/v4');
const generate = require('project-name-generator');
var Lock = require('lock').Lock;
var lock = Lock();

TURTLES = {};

// see https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

class Colour {
    constructor(r, g, b, a = 1.0) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
        this.hex = rgbToHex(r, g, b);
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
    }
}

class Turtle {
    constructor(x = 100, y = 100) {
        this.location = new Point(x, y);
        this.colour = new Colour(0, 0, 0);
        this.angle = 90;
        this.pen = true;
        this.pen_width = 1;
        this.id = uuidv4();
        this._generated_name = generate({ number: true })
        this.name = this._generated_name.dashed;
        this.history = [];
        this.canvas = new DummyTurtleCanvas();
        this.last = -1;
        this.start_state = this.state();
    }

    command(cmd, args) {
        lock(this.name, (release) => {
            console.log('Locked - ' + this.name);
            this.history.push(new TurtleCommand(this.history.length, cmd, args, () => {
                this._command(cmd, args);
            }));
            this._command(cmd, args);
            release(() => {
                console.log('Released - ' + this.name);
            })();
        });
    }

    _command(cmd, args) {
        this[cmd].apply(this, args);
    }

    state() {
        return {
            location: this.location,
            colour: this.colour,
            angle: this.angle,
            pen: this.pen,
            pen_width: this.pen_width,
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

    penwidth(w) {
        this.pen_width = w;
    }

    left(angle) {
        this.angle += angle;
    }

    right(angle) {
        this.angle -= angle;
    }

    pencolour(colour) {
        this.colour = colour;
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

    clear() {
        //clears the canvas
        //no state change, only side-effect
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

router.get('/list', function (req, res, next) {
    let names = [];
    for(var name in TURTLES) {
        names.push(name);
    }
    res.send(names);
});

router.get('/:name/start_state', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    res.send(t.start_state);
});

router.get('/:name/stop', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('stop', null);
    res.send(t.state());
});

router.get('/:name/clear', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('clear', null);
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

router.get('/:name/penwidth', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('penwidth', [parseFloat(req.query.w)]);
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

router.get('/:name/pencolour', function (req, res, next) {
    var t = get_turtle_by_name(req.params['name']);
    t.command('pencolour', [new Colour(parseInt(req.query.r), parseInt(req.query.g), parseInt(req.query.b))]);
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

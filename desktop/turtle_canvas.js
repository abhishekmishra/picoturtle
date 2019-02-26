const { Colour } = require('./colour_utils.js');
const axios = require('axios');
const sleep = require('./utils').sleep;
var Lock = require('lock').Lock;
const fs = require('fs');

var lock = Lock();

const port = require('./utils').getTurtlePort();
const TURTLE_SERVER_URL = 'http://localhost:' + port;

async function list_turtles() {
    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/list');
    let ls = await req.data;
    let turtle_list = document.getElementById('turtle_list');
    turtle_list.innerHTML = '';
    ls.forEach(element => {
        turtle_list.innerHTML += '<li id="' + element + '">' + element + '</li>';
    });
    ls.forEach(element => {
        let el = document.getElementById(element);
        el.className += " name-li";
        el.onclick = function () {
            track_turtle(element);
        };
    });
}

function mark_selected(name) {
    let els = document.getElementsByClassName("name-li selected");
    for (var i = 0; i < els.length; i++) {
        var e = els[i];
        e.className = "name-li";
    }

    let sel = document.getElementById(name);
    if (sel != null) {
        sel.className += " selected";
    }
    let turtle_name = document.getElementById('turtle_name');
    turtle_name.innerHTML = name;
}

function add_object_code_line(cmd_args) {
    let oc = document.getElementById('object_code');
    oc.innerText += '\n';
    for (var i = 0; i < cmd_args.length; i++) {
        if (typeof cmd_args[i] == 'object') {
            if ('hex' in cmd_args[i]) {
                oc.innerText += ' ' + cmd_args[i].hex;
            } else {
                oc.innerText += ' ' + JSON.stringify(cmd_args[i]);
            }
        } else {
            oc.innerText += ' ' + cmd_args[i];
        }
    }
}

var x = 0;
async function track_turtle(local_turtle, name) {
    console.log('running turtle -> ' + name);
    //    await list_turtles();
    //    mark_selected(name);

    let oc = document.getElementById('object_code');
    oc.innerText = '';

    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/' + name + '/start_state');
    let t = await req.data;
    //var local_turtle = new Turtle("turtle_canvas", t);
    local_turtle.init(t);

    // let y = x++;
    // console.log('started ' + y);
    fetch_commands(local_turtle, 0);
    // console.log('done ' + y);
}


async function fetch_commands(local_turtle, cmd_id) {
    let req = await axios.get(TURTLE_SERVER_URL + '/turtle/' + local_turtle.name + '/command?id=' + cmd_id);
    let cmd = await req.data;
    // console.log(cmd);
    // await lock(local_turtle.name, (release) => {
    //     console.log('Locked - ' + local_turtle.name);

    if ('cmd' in cmd) {
        let args = [cmd.cmd];
        if (cmd.args != null) {
            Array.prototype.push.apply(args, cmd.args);
        }
        // console.log(args);
        local_turtle.reset();
        local_turtle.batchStart();
        local_turtle.exec.apply(local_turtle, args);
        local_turtle.batchEnd();
        add_object_code_line(args);
    }

    //     release(() => {
    //         console.log('Released - ' + local_turtle.name);
    //     })();
    // });
    if ('cmd' in cmd) {
        cmd_id += 1;
    }
    if (cmd.hasmore) {
        // console.log('there are more commands pending, after ' + cmd_id - 1);
        // let y = x++;
        // console.log('A: started ' + y);
        fetch_commands(local_turtle, cmd_id);
        // console.log('next complete' + y);
    } else {
        if (cmd.turtle.last == -1 || cmd.turtle.last > cmd_id) {
            await sleep(1000);
            // let y = x++;
            // console.log('B: started ' + y);
            fetch_commands(local_turtle, cmd_id);
            // console.log('new complete' + y);
        } else {
            //console.log(local_turtle);
        }
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

class Turtle {

    constructor(canvas_id, t = null) {
        this.canvas_id = canvas_id;
        this.history = [];
        this.batchEnabled = false;
        if (t != null) {
            this.init(t);
        }
        this.initCanvas();
        this.location = new Point(this.width / 2, this.height / 2);
        this.location_canvas = new Point(this.location.x, this.height - this.location.y);
        this.angle = 90;
        this.canvas_angle = this.angle + 180;
        this.pen_width = 1;
        this.ctx.lineWidth = 1;
    }

    init(t) {
        this.orig_t = t;
        this.initTurtle();
        this.clearHistory();
    }

    initCanvas() {
        this.canvas = document.getElementById(this.canvas_id);
        this.width = this.canvas.width;
        this.height = this.canvas.height;
        this.ctx = this.canvas.getContext('2d');
    }

    initTurtle() {
        this.name = this.orig_t.name;
        this.location = this.orig_t.location;
        this.location_canvas = new Point(this.location.x, this.height - this.location.y);
        this.angle = this.orig_t.angle;
        this.canvas_angle = this.angle + 180;
        this.pen = this.orig_t.pen;
        this.pen_width = this.orig_t.pen_width;
        this.pencolour(new Colour(
            this.orig_t.colour.r,
            this.orig_t.colour.g,
            this.orig_t.colour.b,
            this.orig_t.colour.a));
        this.drawTurtle();
    }

    command(cmd) {
        lock(this.name, (release) => {
            console.log('Locked - ' + this.name);
            this.history.push(cmd);
            this._command(cmd.name, cmd.args);
            release(() => {
                console.log('Released - ' + this.name);
            })();
        });
    }

    _command(name, args) {
        this[name].apply(this, args);
    }

    penup() {
        this.pen = true;
    }

    pendown() {
        this.pen = false;
    }

    penwidth(w) {
        this.pen_width = w;
        this.ctx.lineWidth = w;
    }

    forward(d) {
        let theta = this.angle * Math.PI / 180;
        let canvas_theta = this.canvas_angle * Math.PI / 180;
        // y2 = d sin (theta) + y1
        // x2 = d cos (theta) + x1 
        let y2 = d * (Math.sin(theta)) + this.location.y;
        let x2 = d * (Math.cos(theta)) + this.location.x;
        let cy2 = d * (Math.sin(canvas_theta)) + this.location_canvas.y;
        let cx2 = d * (Math.cos(canvas_theta)) + this.location_canvas.x;

        // console.log('current point is ' + this.location.x + ', ' + this.location.y + ', new point is ' + x2 + ', ' + y2);
        this.ctx.beginPath();

        this.ctx.moveTo(this.location_canvas.x, this.location_canvas.y);
        if (this.pen) {
            this.ctx.moveTo(cx2, cy2);
        } else {
            this.ctx.lineTo(cx2, cy2);
        }

        this.location = new Point(x2, y2);
        this.location_canvas = new Point(cx2, cy2);
        this.ctx.stroke();
    }

    right(angle) {
        this.angle = this.angle - angle;
        this.canvas_angle = this.canvas_angle + angle;
    }

    left(angle) {
        this.angle = this.angle + angle;
        this.canvas_angle = this.canvas_angle - angle;
    }

    stop() {

    }

    //style methods

    pencolour(colour) {
        this.colour = new Colour(colour.r, colour.g, colour.b, colour.a);
        this.ctx.strokeStyle = this.colour.hex;
    }

    clear() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    }

    reset() {
        this.clear();
        this.initTurtle();
    }

    // home() {
    //     this.location = new Point(this.width / 2, this.height / 2);
    //     this.angle = 0;
    // }

    addToHistory(cmd) {
        this.history.push(cmd);
    }

    clearHistory() {
        this.history = [];
    }

    drawTurtle() {
        this.ctx.save();
        let hexColour = this.ctx.strokeStyle;
        this.ctx.strokeStyle = 'black';
        this.ctx.fillStyle = 'red';
        this.ctx.lineWidth = 3;
        // this.ctx.fillRect(this.location.x - 5, this.location.y - 5, 10, 10);

        let d = 25;
        let theta1 = (this.canvas_angle - 145) * Math.PI / 180;
        let y2 = d * (Math.sin(theta1)) + this.location_canvas.y;
        let x2 = d * (Math.cos(theta1)) + this.location_canvas.x;

        let theta2 = (this.canvas_angle + 145) * Math.PI / 180;
        let y3 = d * (Math.sin(theta2)) + this.location_canvas.y;
        let x3 = d * (Math.cos(theta2)) + this.location_canvas.x;

        this.ctx.beginPath();
        this.ctx.moveTo(this.location_canvas.x, this.location_canvas.y);
        this.ctx.lineTo(x2, y2);
        this.ctx.lineTo(x3, y3);
        this.ctx.lineTo(this.location_canvas.x, this.location_canvas.y);
        this.ctx.stroke();

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
            // console.log('command = ' + command);
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

    export(filePath) {
        // Get the DataUrl from the Canvas
        const url = this.canvas.toDataURL('image/png');
        console.log(url);
        // remove Base64 stuff from the Image
        const base64Data = url.replace(/^data:image\/png;base64,/, "");
        fs.writeFile(filePath, base64Data, 'base64', function (err) {
            console.log(err);
        });
    }
}

module.exports.list_turtles = list_turtles;
module.exports.track_turtle = track_turtle;
module.exports.Point = Point;
module.exports.Turtle = Turtle;
// export { list_turtles, track_turtle, Point, Turtle };
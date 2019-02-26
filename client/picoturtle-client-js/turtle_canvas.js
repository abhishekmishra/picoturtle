import { sleep } from './utils.js';
import { Colour } from './colour_utils.js';

async function list_turtles() {
    let req = await fetch('/turtle/list');
    let ls = await req.json();
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

async function track_turtle(name) {
    await list_turtles();
    mark_selected(name);

    let oc = document.getElementById('object_code');
    oc.innerText = '';

    let req = await fetch('/turtle/' + name + '/start_state');
    let t = await req.json();
    var local_turtle = new Turtle("turtle_canvas", t);
    await fetch_commands(local_turtle, 0);
}

async function fetch_commands(local_turtle, cmd_id) {
    let req = await fetch('/turtle/' + local_turtle.name + '/command?id=' + cmd_id);
    let cmd = await req.json();
    //console.log(cmd);
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
    if ('cmd' in cmd) {
        cmd_id += 1;
    }
    if (cmd.hasmore) {
        console.log('there are more commands pending, after ' + (cmd_id - 1));
        fetch_commands(local_turtle, cmd_id);
    } else {
        if (cmd.turtle.last == -1 || cmd.turtle.last > cmd_id) {
            await sleep(5000);
            fetch_commands(local_turtle, cmd_id);
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

    constructor(canvas_id, t) {
        this.orig_t = t;
        this.canvas_id = canvas_id;
        this.history = [];
        this.batchEnabled = false;
        this.initCanvas();
        this.initTurtle();
        this.history = [];
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
        this.angle = this.orig_t.angle;
        this.pen = this.orig_t.pen;
        this.pencolour(new Colour(
            this.orig_t.colour.r,
            this.orig_t.colour.g,
            this.orig_t.colour.b,
            this.orig_t.colour.a));
        this.drawTurtle();
    }

    command(cmd) {
        this.history.push(cmd);
        this._command(cmd.name, cmd.args);
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

    forward(d) {
        let theta = this.angle * Math.PI / 180;
        // y2 = d sin (theta) + y1
        // x2 = d cos (theta) + x1 
        let y2 = d * (Math.sin(theta)) + this.location.y;
        let x2 = d * (Math.cos(theta)) + this.location.x;

        // console.log('current point is ' + this.location.x + ', ' + this.location.y + ', new point is ' + x2 + ', ' + y2);
        this.ctx.beginPath();

        this.ctx.moveTo(this.location.x, this.location.y);
        if (this.pen) {
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

    left(angle) {
        this.angle = this.angle - angle;
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
}

export { list_turtles, track_turtle, Point, Turtle };
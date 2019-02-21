const fetch = require('node-fetch');
const url = 'http://localhost:3000';

class TurtleProxy {
    constructor(canvas_id = null) {
        this.canvas_id = canvas_id;
        if (this.canvas_id != null) {
            this.canvas = document.getElementById(this.canvas_id);
        }
    }

    async init(name) {
        if (name == null) {
            let res = await fetch(url + '/turtle/create?x=250&y=250');
            let t = await res.json();
            this.name = t.name;
            // console.log('Created turtle with name ' + t.name);
            return t;
        } else {
            this.name = name;
        }
    }

    async penup() {
        let res = await fetch(url + '/turtle/' + this.name + '/penup');
        let t = await res.json();
        // console.log('penup for - ' + t.name);
    }

    async pendown() {
        let res = await fetch(url + '/turtle/' + this.name + '/pendown');
        let t = await res.json();
        // console.log('pendown for - ' + t.name);
    }

    async stop() {
        let res = await fetch(url + '/turtle/' + this.name + '/stop');
        let t = await res.json();
        // console.log('stop for - ' + t.name);
    }

    async forward(d) {
        let res = await fetch(url + '/turtle/' + this.name + '/forward?d=' + d);
        let t = await res.json();
        // console.log('forward ' + d + ' for - ' + t.name);
    }

    async left(a) {
        let res = await fetch(url + '/turtle/' + this.name + '/left?a=' + a);
        let t = await res.json();
        // console.log('left ' + a + ' for - ' + t.name);
    }

    async right(a) {
        let res = await fetch(url + '/turtle/' + this.name + '/right?a=' + a);
        let t = await res.json();
        // console.log('right ' + a + ' for - ' + t.name);
    }

    async pencolour(r, g, b) {
        let res = await fetch(url + '/turtle/' + this.name + '/pencolour?r=' + r + '&g=' + g + '&b=' + b);
        let t = await res.json();
        // console.log('colour [' + r + ', ' + g + ', ' + b + '] for - ' + t.name);
    }
}

module.exports.TurtleProxy = TurtleProxy;
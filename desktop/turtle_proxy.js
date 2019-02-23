// const fetch = require('node-fetch');
const axios = require('axios');
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
            let res = await axios.get(url + '/turtle/create?x=250&y=250');
            let t = await res.data;
            this.name = t.name;
            // console.log('Created turtle with name ' + t.name);
            return t;
        } else {
            this.name = name;
        }
    }

    async penup() {
        let res = await axios.get(url + '/turtle/' + this.name + '/penup');
        let t = await res.data;
        // console.log('penup for - ' + t.name);
    }

    async pendown() {
        let res = await axios.get(url + '/turtle/' + this.name + '/pendown');
        let t = await res.data;
        // console.log('pendown for - ' + t.name);
    }

    async penwidth(w) {
        let res = await axios.get(url + '/turtle/' + this.name + '/penwidth?w=' + w);
        let t = await res.data;
        // console.log('penwidth ' + w + ' for - ' + t.name);
    }

    async stop() {
        let res = await axios.get(url + '/turtle/' + this.name + '/stop');
        let t = await res.data;
        // console.log('stop for - ' + t.name);
    }

    async clear() {
        let res = await axios.get(url + '/turtle/' + this.name + '/clear');
        let t = await res.data;
        // console.log('clear for - ' + t.name);
    }

    async forward(d) {
        let res = await axios.get(url + '/turtle/' + this.name + '/forward?d=' + d);
        let t = await res.data;
        // console.log('forward ' + d + ' for - ' + t.name);
    }

    async left(a) {
        let res = await axios.get(url + '/turtle/' + this.name + '/left?a=' + a);
        let t = await res.data;
        // console.log('left ' + a + ' for - ' + t.name);
    }

    async right(a) {
        let res = await axios.get(url + '/turtle/' + this.name + '/right?a=' + a);
        let t = await res.data;
        // console.log('right ' + a + ' for - ' + t.name);
    }

    async pencolour(r, g, b) {
        let res = await axios.get(url + '/turtle/' + this.name + '/pencolour?r=' + r + '&g=' + g + '&b=' + b);
        let t = await res.data;
        // console.log('colour [' + r + ', ' + g + ', ' + b + '] for - ' + t.name);
    }
}

module.exports.TurtleProxy = TurtleProxy;
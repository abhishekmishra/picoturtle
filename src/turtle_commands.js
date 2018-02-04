class TurtleCommands {
    constructor(t) {
        this.t = t;
    }

    reset() {
        this.t.reset();
    }
}

module.exports.TurtleCommands = TurtleCommands;
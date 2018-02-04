const {dialog} = require('electron').remote

class TurtleCommands {
    constructor(t) {
        this.t = t;
    }

    reset() {
        this.t.reset();
    }

    open() {
        console.log(dialog.showOpenDialog({properties: ['openFile', 'openDirectory', 'multiSelections']}));
    }

    save() {
        
    }
}

module.exports.TurtleCommands = TurtleCommands;
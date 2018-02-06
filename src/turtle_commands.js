const { dialog } = require('electron').remote;
const fs = require('fs');
const $ = require('jquery');

class TurtleCommands {
    constructor(t, teditor) {
        this.t = t;
        this.teditor = teditor;
    }

    reset() {
        this.t.reset();
    }

    open() {
        console.log(dialog.showOpenDialog({
            filters: [
                { name: 'Turtle files', extensions: ['turtle', 'scm', 'ss'] }],
            properties: ['openFile']
        }));
    }

    save() {
        if (!this.teditor.filename) {
            let saveFile = dialog.showSaveDialog({
                defaultPath: 'untitled.turtle',
                filters: [
                    { name: 'Turtle files', extentions: ['turtle'] }
                ]
            });
            console.log('Save to file -> ' + saveFile);
            this.teditor.filename = saveFile
        }
        fs.writeFileSync(this.teditor.filename, this.teditor.getText());
        $('#filename').html(this.teditor.filename);
        return this.teditor.filename;
    }
}

module.exports.TurtleCommands = TurtleCommands;
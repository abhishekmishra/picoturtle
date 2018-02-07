const { dialog } = require('electron').remote;
const fs = require('fs');
const $ = require('jquery');
const BiwaScheme = require("biwascheme"); 

class TurtleCommands {
    constructor(t, teditor) {
        this.t = t;
        this.teditor = teditor;
    }

    reset() {
        this.t.reset();
    }

    run() {
        this.reset();
        let txt = this.teditor.getText();
        this.t.batchStart();
        BiwaScheme.run(txt);
        this.t.batchEnd();
    }
    
    
    open() {
        let openFile = dialog.showOpenDialog({
            filters: [
                { name: 'Turtle files', extensions: ['turtle', 'scm', 'ss'] }],
            properties: ['openFile']
        });
        console.log('file to open ' + openFile);
        let text = fs.readFileSync('' + openFile);
        this.teditor.setText(text);
        this.teditor.filename = openFile.toString();
        $('#filename').html(this.teditor.filename);
        return openFile.toString();
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
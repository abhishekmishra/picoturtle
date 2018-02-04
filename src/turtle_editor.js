let START_VALUE = 
'(pencolor "blue")\n\
(forward 100)\n\
(left 90)\n\
(forward 100)\n\
';

class TurtleEditor {
    constructor() {
        this.editor = ace.edit("editor");
        this.editor.setTheme("ace/theme/monokai");
        this.editor.session.setMode("ace/mode/scheme");
        this.editor.setValue(START_VALUE);
    }

    getText() {
        return this.editor.getValue();
    }
}

module.exports.TurtleEditor = TurtleEditor;
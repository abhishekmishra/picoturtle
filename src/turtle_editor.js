let START_VALUE = 
'(pencolor "blue")\n\
(forward 100)\n\
(left 90)\n\
(forward 100)\n\
(left 90)\n\
\n\
(pencolor "red")\n\
(forward 100)\n\
(left 90)\n\
(forward 100)\n\
(left 90)\n\
';

class TurtleEditor {
    constructor() {
        this.editor = ace.edit("editor");
        this.editor.setTheme("ace/theme/clouds");
        this.editor.session.setMode("ace/mode/scheme");
        this.editor.setValue(START_VALUE);
    }

    getText() {
        return this.editor.getValue();
    }

    setTheme(theme) {
        return this.editor.setTheme("ace/theme/" + theme);
    }
}

module.exports.TurtleEditor = TurtleEditor;
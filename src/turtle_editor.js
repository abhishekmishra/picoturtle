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
        this.editor.setTheme("ace/theme/monokai");
        this.editor.session.setMode("ace/mode/scheme");
        this.editor.setValue(START_VALUE, 1);
        this.editor.focus();
        this.filename = null;
    }

    getText() {
        return this.editor.getValue();
    }

    setText(txt) {
        this.editor.setValue(txt.toString(), 1);
    }

    setTheme(theme) {
        return this.editor.setTheme("ace/theme/" + theme);
    }

    getFileName() {
        return this.filename ? this.filename: 'untitled';
    }
}

module.exports.TurtleEditor = TurtleEditor;
const url = require('url');
const { env } = require('./env');
const path = require('path');

const preferenceConfig = {
    "appearance": {
        label: "Appearance",
        type: "group"
    },
    "python": {
        label: "Python",
        type: "group"
    },
    "csharp": {
        label: "C#",
        type: "group"
    },
    "theme": {
        label: "Theme",
        type: "select",
        options: [
            {key: "Dark", value: "dark" },
            {key: "Light", value: "light" }
        ]
    },
    "pathToMsBuild": {
        label: "MSBuild Path",
        type: "path"
    },
    "pathToPython3": {
        label: "Python3 Path",
        type: "path"
    }
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function getTurtlePort() {
    var url_parts = url.parse(window.location.href, true);
    var query = url_parts.query;
    let port = query.port;
    // console.log('Turtle port is ' + port);
    return port;
}

function getSampleFilePath(sample_file) {
    if (env === 'dev') {
        return path.join(__dirname, 'samples', sample_file);
    }
    if (env === 'prod') {
        return path.join(__dirname, '..', 'samples', sample_file);
    }
}

function setTheme(theme) {
    $('#theme_css').attr('href', 'css/' + theme + '.css');
    if (typeof monaco !== 'undefined') {
        if (theme == 'dark') {
            monaco.editor.setTheme('vs-dark');
        }
        if (theme == 'light') {
            monaco.editor.setTheme('vs-light');
        }
    }
}

module.exports.sleep = sleep;
module.exports.getTurtlePort = getTurtlePort;
module.exports.getSampleFilePath = getSampleFilePath;
module.exports.setTheme = setTheme;
module.exports.preferenceConfig = preferenceConfig;
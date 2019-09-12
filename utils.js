const url = require('url');
const { env } = require('./env');
const path = require('path');
const Store = require('electron-store');

const PICOTURTLE_JAVA_RELEASE_VERSION='0.0.2';

var isWin = process.platform === "win32";
var isLinux = process.platform === "linux";
var isMacos = process.platform === "darwin";

const storeOptions = {
    defaults: {}
};

function storeSetDefault(key, defaultValue) {
    if (!store.has(key)) {
        store.set(key, defaultValue);
    }
}

// Application Config
const store = new Store(storeOptions);
storeSetDefault('appearance.theme', 'dark');
if (isWin) {
    storeSetDefault('python.pathToPython3', 'python');
} else {
    storeSetDefault('python.pathToPython3', 'python3');
}
storeSetDefault('csharp.pathToMsBuild', 'msbuild');
storeSetDefault('csharp.pathToMono', 'mono');
storeSetDefault('java.pathToJDK', '');

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
    "java": {
        label: "Java",
        type: "group"
    },
    "theme": {
        label: "Theme",
        type: "select",
        options: [
            { key: "Dark", value: "dark" },
            { key: "Light", value: "light" }
        ]
    },
    "pathToMsBuild": {
        label: "MSBuild Path",
        type: "path"
    },
    "pathToMono": {
        label: "Mono Path <br/><em>(Not used on Windows)</em>",
        type: "path"
    },
    "pathToPython3": {
        label: "Python3 Path",
        type: "path"
    },
    "pathToJDK": {
        label: "JDK Bin Path <br/><em>(Folder which contains javac and java)</em>",
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
module.exports.store = store;
module.exports.PICOTURTLE_JAVA_RELEASE_VERSION = PICOTURTLE_JAVA_RELEASE_VERSION;
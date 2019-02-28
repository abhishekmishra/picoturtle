const url = require('url');
const { env } = require('./env');
const path = require('path');

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

module.exports.sleep = sleep;
module.exports.getTurtlePort = getTurtlePort;
module.exports.getSampleFilePath = getSampleFilePath;
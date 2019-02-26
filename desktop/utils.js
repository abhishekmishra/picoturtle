const url = require('url');

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

module.exports.sleep = sleep;
module.exports.getTurtlePort = getTurtlePort;
// export { sleep };
function getJSON(url, success, error = null) {
    var request = new XMLHttpRequest();
    request.open('GET', url, true);
    request.onload = function () {
        if (request.status >= 200 && request.status < 400) {
            // Success!
            var data = JSON.parse(request.responseText);
            success(data);
        } else {
            // We reached our target server, but it returned an error
            if (error) {
                error();
            }
        }
    };
    if (error) {
        request.onerror = error;
    }
    request.send();
}

getJSON('/turtle/create', (t) => {
    console.log('Created turtle with name ' + t.name);
    getJSON('/turtle/' + t.name + '/penup', (t) => {
        console.log(t);
    });
    getJSON('/turtle/' + t.name + '/forward/100', (t) => {
        console.log(t);
    });
    getJSON('/turtle/' + t.name + '/pendown', (t) => {
        console.log(t);
    });
    getJSON('/turtle/' + t.name + '/forward/100', (t) => {
        console.log(t);
    });
    getJSON('/turtle/' + t.name + '/left/90', (t) => {
        console.log(t);
    });
    getJSON('/turtle/' + t.name + '/forward/100', (t) => {
        console.log(t);
    });
});
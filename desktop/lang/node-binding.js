class TurtleNodeJSBinding {
    constructor() {
        //do nothing
    }

    async available() {
        return Promise.resolve(true);
    }

    async canExecFile() {
        return Promise.resolve(true);
    }

    async execFile(output_cb, error_cb, complete_cb) {
        return Promise.reject();
    }

    async execText(output_cb, error_cb, complete_cb) {
        // see https://stackoverflow.com/questions/46118496/asyncfunction-is-not-defined-yet-mdn-documents-its-usage
        const AsyncFunction = Object.getPrototypeOf(async function () { }).constructor;
        var f = new AsyncFunction('t', text);
        return await f(t);
    }
}
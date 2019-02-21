// see https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

function hexToColour(hex) {
    var res = hexToRgb(hex);
    return new Colour(res.r, res.g, res.b);
}

class Colour {
    constructor(r, g, b, a = 1.0) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
        this.hex = rgbToHex(r, g, b);
    }
}

// module.exports.componentToHex = componentToHex;
// module.exports.rgbToHex = rgbToHex;
// module.exports.hexToRgb = hexToRgb;
// module.exports.hexToColour = hexToColour;
// module.exports.Colour = Colour;
export { componentToHex, rgbToHex, hexToRgb, hexToColour, Colour };
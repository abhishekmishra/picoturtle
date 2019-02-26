var showdown = require('showdown');
var fs = require('fs');
var path = require('path');

converter = new showdown.Converter();
let text = fs.readFileSync(path.join(__dirname, 'help.md'), {
    encoding: 'utf-8'
});
html = converter.makeHtml(text);
$("#doc").html(html);

$('#close_button').on('click', (event)=>{
    require('electron').remote.getCurrentWindow().close();
});
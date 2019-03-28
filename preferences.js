$('#close_button').on('click', (event)=>{
    require('electron').remote.getCurrentWindow().close();
});
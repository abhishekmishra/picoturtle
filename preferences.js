const { dialog } = require('electron').remote;
const setTheme = require('./utils').setTheme;
const preferenceConfig = require('./utils').preferenceConfig;
const store = require('./utils').store;

// set theme
setTheme(store.get('appearance.theme'));
store.onDidChange('appearance.theme', (newval, oldval) => {
    setTheme(newval);
});

console.log(store.path);

for (var item in store.store) {
    let label = item;
    if (item in preferenceConfig) {
        label = preferenceConfig[item].label;
    }
    $('#prefmenu').append(
        '<a id="pref_' + item + '" href="#" class="list-group-item list-group-item-action list-group-item-secondary"'
        + '>'
        + label
        + '</a>'
    );

    $('#pref_' + item).on('click', (event) => {
        $(this).addClass('active');
        let x = event.target.id.substring(5);
        showPage(x);
    });
}

$('#pref_appearance').click();

/** 
 * Show the preferences page for
 * the given top-level preference item
 */
function showPage(item) {
    $('#prefitems').html('');
    console.log(store.get(item));
    for (var x in store.get(item)) {
        console.log(x);
        let store_name = item + '.' + x;
        let id = 'pref_' + item + '_' + x;
        let label = x;
        let type = 'text';
        if (x in preferenceConfig) {
            label = preferenceConfig[x].label;
            type = preferenceConfig[x].type;
        }

        let item_content = '<div class="list-group-item list-group-item-dark">'
            + '<div class="d-flex w-100 justify-content-between">'
            // + '<form>'
            + '<div class="form-group row w-100">'
            + '<label for="staticEmail" class="col-sm-3 col-form-label text-right">' + label + '</label>'
            + '<div class="col-sm-9">';

        if (type == 'text') {
            item_content += '<input type="text" id="' + id + '" class="w-100 ml-3"></input>';
        }
        if (type == 'select') {
            let options = '';
            for (var i = 0; i < preferenceConfig[x].options.length; i++) {
                // console.log(preferenceConfig[x].options[i]);
                options += '<option value="'
                    + preferenceConfig[x].options[i].value
                    + '">'
                    + preferenceConfig[x].options[i].key
                    + '</option>';
            }
            item_content += '<select id="' + id + '" class="form-control">'
                + options
                + '</select>';
        }
        if (type == 'path') {
            item_content += '<div class="form-group row w-100">'
                + '<div class="col-sm-8 m-0 p-1"><input type="text" id="' + id + '" class="form-control mb-2"></input></div>'
                + '<div class="col-sm-4 m-0 p-1">'
                + '<button id="browse_' + id + '"class="btn btn-secondary btn-sm mb-2 form-control">Browse</button></div>'
                + '</div>';
        }
        item_content += '</div>'
            // + '</form>'
            + '</div>'
            + '</div>';

        //add the generated content
        $('#prefitems').append(item_content);

        //set the current value
        $('#' + id).val(store.get(store_name));

        //listen for change value
        $('#' + id).on('change', (event) => {
            console.log('setting ' + store_name + ' as ' + event.target.value);
            store.set(store_name, event.target.value);
        });

        //add browse handler if type is path
        if (type == "path") {
            $('#browse_' + id).on('click', (event) => {
                var selectedPath = dialog.showOpenDialog({ properties: ['openFile'] });
                if (selectedPath !== undefined) {
                    console.log(selectedPath);
                    $('#' + id).val(selectedPath);
                    $('#' + id).change();
                }
            });
        }
    }
}

$('#close_button').on('click', (event) => {
    require('electron').remote.getCurrentWindow().close();
});
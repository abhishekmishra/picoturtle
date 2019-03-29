const Store = require('electron-store');
const setTheme = require('./utils').setTheme;
const preferenceConfig = require('./utils').preferenceConfig;

// Application Config
const store = new Store({
    defaults: {
        appearance: {
            theme: 'dark'
        }
    }
});

// set theme
setTheme(store.get('appearance.theme'));

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
    for (var x in store.get(item)) {
        let label = x;
        if (x in preferenceConfig) {
            label = preferenceConfig[x].label;
        }
        $('#prefitems').append(
            '<a href="#" class="list-group-item list-group-item-dark">'
            + '<div class="d-flex w-100 justify-content-between">'
            + label
            + '<input type="text" class="w-100 ml-3"></input>'
            + '</div>'
            + '</a>'
        );
    }
}

$('#close_button').on('click', (event) => {
    require('electron').remote.getCurrentWindow().close();
});
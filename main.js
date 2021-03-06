// Make sure windows installation does not show app multiple times
// See https://github.com/electron/windows-installer#handling-squirrel-events
if (require('electron-squirrel-startup')) return;

// Check for updates on windows and macos using update-electron-app
// see https://github.com/electron/update-electron-app
// require('update-electron-app')()

// Modules to control application life and create native browser window
const { app, BrowserWindow, Menu, dialog } = require('electron');
const { ipcMain } = require('electron');
const appenv = require('./env');
const path = require('path');
const url = require('url');
const getPort = require('get-port');

let isDirty = false;

//console.log('ENV is ' + appenv.env);
process.env.NODE_ENV = appenv.env;

function getEnv() {
  return process.env.NODE_ENV;
}

function isProd() {
  return getEnv() == 'prod';
}

function isDev() {
  return getEnv() == 'dev';
}

function getPicoTurtleServer() {
  var isWin = process.platform === "win32";
  var isLinux = process.platform === "linux";
  var isMacos = process.platform === "darwin";

  let execName = '';
  if (isWin) {
    execName = "picoturtle-server-win.exe";
  }
  if (isLinux) {
    execName = "picoturtle-server-linux";
  }
  if (isMacos) {
    execName = "picoturtle-server-macos";
  }
  if (isDev()) {
    return path.join(__dirname, 'dist', execName);
  }
  if (isProd()) {
    return path.join(__dirname, '..', execName);
  }
}

let port = null;
let picoTurtleServerProc = null;
const runPicoTurtleServer = () => {

  let penv = JSON.parse(JSON.stringify(process.env));
  penv['PORT'] = port;
  let options = {
    cwd: path.join(__dirname, '..'),
    env: penv
  };

  picoTurtleServerProc = require('child_process').spawn(getPicoTurtleServer(), [], options);

  if (picoTurtleServerProc != null) {
    //console.log(picoTurtleServerProc)
    //console.log('child process success on port ' + port);
  }

  picoTurtleServerProc.stdout.on('data', (data) => {
    //console.log(`stdout: ${data}`);
  });

  picoTurtleServerProc.stderr.on('data', (data) => {
    console.log(`stderr: ${data}`);
  });

  picoTurtleServerProc.on('close', (code) => {
    //console.log(`child process exited with code ${code}`);
  });

}

const exitPicoTurtleServer = () => {
  picoTurtleServerProc.kill()
  picoTurtleServerProc = null
  pyPort = null
}

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 1200,
    minWidth: 1200,
    height: 800,
    minHeight: 800,
    webPreferences: {
      nodeIntegration: true,
      enableRemoteModule: true,
    }
  })

  // and load the index.html of the app.
  //console.log('Loading ' + 'index.html?port=' + port)
  mainWindow.loadFile('index.html', {
    query: {
      'port': port
    }
  });

  // Open the DevTools.
  //mainWindow.webContents.openDevTools();

  // Emitted when the window is closed.
  mainWindow.on('closed', function () {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow = null
  });

  mainWindow.on('close', (e) => {
    if (isDirty) {
      let choice = dialog.showMessageBox(mainWindow, {
        type: 'question',
        buttons: ['Yes', 'No'],
        defaultId: 1,
        title: 'Save current file?',
        message: 'Your changes in the editor are not saved. Quit?' +
          '\nIf you choose "Yes" you will lose all unsaved work!.',
      });
      if (choice == 1) {
        e.preventDefault();
      }
    }
  });
}

(async () => {
  port = await getPort();

  app.on('ready', runPicoTurtleServer);
  app.on('will-quit', exitPicoTurtleServer);


  // This method will be called when Electron has finished
  // initialization and is ready to create browser windows.
  // Some APIs can only be used after this event occurs.
  app.on('ready', createWindow);

  // Quit when all windows are closed.
  app.on('window-all-closed', function () {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
      app.quit()
    }
  });

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (mainWindow === null) {
      createWindow()
    }
  });
})();

ipcMain.on('dirty', (e, dirty_flag) => {
  isDirty = dirty_flag;
});

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.

const template = [
  {
    label: 'File',
    submenu: [
      {
        label: 'New',
        accelerator: 'CommandOrControl+N',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('file.new', '')
        }
      },
      {
        label: 'Open',
        accelerator: 'CommandOrControl+O',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('file.open', '')
        }
      },
      {
        label: 'Save',
        accelerator: 'CommandOrControl+S',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('file.save', '')
        }
      },
      {
        label: 'Save As',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('file.save_as', '')
        }
      },
      { type: 'separator' },
      {
        label: 'Preferences',
        accelerator: 'F12',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('file.preferences', '');
        }
      },
      { type: 'separator' },
      { role: 'close' }
    ]
  },
  {
    label: 'Turtle',
    submenu: [
      {
        label: 'Run',
        accelerator: 'CommandOrControl+,',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('turtle.run', '')
        }
      },
      {
        label: 'Export',
        accelerator: 'CommandOrControl+.',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('turtle.export', '')
        }
      }
    ]
  },
  {
    label: 'Edit',
    submenu: [
      { role: 'undo' },
      { role: 'redo' },
      { type: 'separator' },
      { role: 'cut' },
      { role: 'copy' },
      { role: 'paste' },
      { role: 'pasteandmatchstyle' },
      { role: 'delete' },
      { role: 'selectall' }
    ]
  },
  {
    label: 'View',
    submenu: [
      {
        label: 'Theme',
        submenu: [
          {
            label: 'Dark', type: 'radio',
            click: (menuItem, browserWindow, event) => {
              browserWindow.webContents.send('view.theme', 'dark')
            }
          },
          {
            label: 'Light', type: 'radio',
            checked: true,
            click: (menuItem, browserWindow, event) => {
              browserWindow.webContents.send('view.theme', 'light')
            }
          }
        ]
      },
      { role: 'reload' },
      { role: 'forcereload' },
      { role: 'toggledevtools' },
      { type: 'separator' },
      { role: 'resetzoom' },
      { role: 'zoomin' },
      { role: 'zoomout' },
      { type: 'separator' },
      { role: 'togglefullscreen' }
    ]
  },
  {
    role: 'window',
    submenu: [
      { role: 'minimize' },
      { role: 'close' }
    ]
  },
  {
    role: 'help',
    submenu: [
      {
        label: 'Docs',
        accelerator: 'F1',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('help.docs', '');
        }
      },
      {
        label: 'Learn More',
        click() { require('electron').shell.openExternal('https://electronjs.org') }
      },
      {
        role: 'about',
        click: (menuItem, browserWindow, event) => {
          browserWindow.webContents.send('help.about', '');
        }
      }
    ]
  }
]

if (process.platform === 'darwin') {
  template.unshift({
    label: app.getName(),
    submenu: [
      { role: 'about' },
      { type: 'separator' },
      { role: 'services' },
      { type: 'separator' },
      { role: 'hide' },
      { role: 'hideothers' },
      { role: 'unhide' },
      { type: 'separator' },
      { role: 'quit' }
    ]
  })

  // Edit menu
  template[3].submenu.push(
    { type: 'separator' },
    {
      label: 'Speech',
      submenu: [
        { role: 'startspeaking' },
        { role: 'stopspeaking' }
      ]
    }
  )

  // Window menu
  template[5].submenu = [
    { role: 'close' },
    { role: 'minimize' },
    { role: 'zoom' },
    { type: 'separator' },
    { role: 'front' }
  ]
}

const menu = Menu.buildFromTemplate(template);
Menu.setApplicationMenu(menu);
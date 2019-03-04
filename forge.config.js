const fs = require('fs');
const path = require('path');
const download = require('download');
const { execSync, execFileSync } = require('child_process');
var isWin = process.platform === "win32";
var isLinux = process.platform === "linux";
var isMacos = process.platform === "darwin";

function getServerExecutable() {
  let server_exec = '';
  if (isWin) {
    server_exec = "picoturtle-server-win.exe";
  }
  if (isLinux) {
    server_exec = "picoturtle-server-linux";
  }
  if (isMacos) {
    server_exec = "picoturtle-server-macos";
  }
  return server_exec;
}

function getNodejsClientExecutable() {
  let nodejs_client_exec = '';
  if (isWin) {
    nodejs_client_exec = "picoturtle-nodejs-client-win.exe";
  }
  if (isLinux) {
    nodejs_client_exec = "picoturtle-nodejs-client-linux";
  }
  if (isMacos) {
    nodejs_client_exec = "picoturtle-nodejs-client-macos";
  }
  return nodejs_client_exec;
}

let extraResource = [
  path.join('dist', getServerExecutable()), 
  path.join('dist', getNodejsClientExecutable())
];
//console.log(extraResource);

let config = {
  "packagerConfig": {
    "extraResource": extraResource,
    "asar": true,
    "icon": "./icons/pico"
  },
  "makers": [
    {
      "name": "@electron-forge/maker-squirrel",
      "config": {
        "name": "picoturtle"
      }
    },
    {
      "name": "@electron-forge/maker-zip",
      "platforms": [
        "darwin"
      ]
    },
    {
      "name": "@electron-forge/maker-deb",
      "config": {}
    },
    {
      "name": "@electron-forge/maker-rpm",
      "config": {}
    }
  ],
  publishers: [
    {
      name: '@electron-forge/publisher-github',
      config: {
        repository: {
          owner: 'abhishekmishra',
          name: 'picoturtle'
        },
        prerelease: true
      }
    }
  ],
  hooks: {
    generateAssets: async () => {
      let serverFilePath = path.join('dist', getServerExecutable());
      if (!fs.existsSync(serverFilePath)) {
        await download('https://github.com/abhishekmishra/picoturtle-server/releases/download/v0.0.2/' + getServerExecutable(), 'dist');
        fs.chmodSync(serverFilePath, '755');
      }

      let nodeJsClientFilePath = path.join('dist', getNodejsClientExecutable());
      if (!fs.existsSync(nodeJsClientFilePath)) {
        await download('https://github.com/abhishekmishra/picoturtle-nodejs-client/releases/download/v0.0.5/' + getNodejsClientExecutable(), 'dist');
        fs.chmodSync(nodeJsClientFilePath, '755');
      }
    },
    packageAfterCopy: async (config, buildPath, electronVersion, platform, arch) => {
      //copy env file
      fs.copyFileSync(path.join(buildPath, 'env-prod.js'), path.join(buildPath, 'env.js'));

      let resourcesFolder = path.join(buildPath, '..');

      //copy python client files
      fs.mkdirSync(path.join(resourcesFolder, 'client'));
      fs.mkdirSync(path.join(resourcesFolder, 'client', 'python'));
      fs.copyFileSync('./client/python/picoturtle.py', path.join(resourcesFolder, 'client', 'python', 'picoturtle.py'));

      //create js client folder outside asar
      // let target_nodejs_client = path.join(resourcesFolder, 'client', 'nodejs')
      // fs.mkdirSync(target_nodejs_client);
      // fs.copyFileSync('./client/nodejs/package.json', path.join(target_nodejs_client, 'package.json'));
      // console.log('Running npm install in ' + target_nodejs_client);
      // execSync('npm install', {
      //   cwd: target_nodejs_client
      // });

      //samples
      fs.mkdirSync(path.join(resourcesFolder, 'samples'));
      fs.readdirSync('./samples').forEach(file => {
        console.log('copying sample file -> ', file);
        fs.copyFileSync(path.join('./samples', file), path.join(resourcesFolder, 'samples', path.basename(file)));
      });
    }
  }
};

module.exports = config;
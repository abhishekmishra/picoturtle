const fs = require('fs');
const path = require('path');
const download = require('download');
var isWin = process.platform === "win32";
var isLinux = process.platform === "linux";
var isMacos = process.platform === "darwin";

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

let extraResource = [path.join('dist', server_exec)];
//console.log(extraResource);

let config = {
  "packagerConfig": {
    "extraResource": extraResource,
    "asar": true
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
      let filePath = path.join('dist', server_exec);
      if (!fs.existsSync(filePath)) {
        await download('https://github.com/abhishekmishra/picoturtle-server/releases/download/v0.0.2/' + server_exec, 'dist');
      }
      fs.chmodSync(filePath, '755');
    },
    packageAfterCopy: async (config, buildPath, electronVersion, platform, arch) => {
      fs.copyFileSync(path.join(buildPath, 'env-prod.js'), path.join(buildPath, 'env.js'));
      let resourcesFolder = path.join(buildPath, '..');
      fs.mkdirSync(path.join(resourcesFolder, 'client'));
      fs.mkdirSync(path.join(resourcesFolder, 'client', 'python'));
      fs.copyFileSync('./client/python/picoturtle.py', path.join(resourcesFolder, 'client', 'python', 'picoturtle.py'));
    }
  }
};

module.exports = config;
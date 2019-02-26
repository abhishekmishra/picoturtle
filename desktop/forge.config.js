const fs = require('fs');
const path = require('path');
var isWin = process.platform === "win32";

let extraResource = ["../server/dist/picoturtle-server"];
if (isWin) {
  extraResource = ["../server/dist/picoturtle-server.exe"];
}

let config = {
  "packagerConfig": {
    "extraResource": extraResource,
    "asar": true
  },
  "makers": [
    {
      "name": "@electron-forge/maker-squirrel",
      "config": {
        "name": "picoturtle_desktop"
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
    packageAfterCopy: async (config, buildPath, electronVersion, platform, arch) => {
      fs.copyFileSync(path.join(buildPath, 'env-prod.js'), path.join(buildPath, 'env.js'));
      let resourcesFolder = path.join(buildPath, '..');
      fs.mkdirSync(path.join(resourcesFolder, 'client'));
      fs.mkdirSync(path.join(resourcesFolder, 'client', 'python'));
      fs.copyFileSync('../client/python/picoturtle.py', path.join(resourcesFolder, 'client', 'python', 'picoturtle.py'));
    }
  }
};

module.exports = config;
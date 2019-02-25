var isWin = process.platform === "win32";

let extraResource = ["../server/dist/picoturtle-server"];
if (isWin) {
    extraResource = ["../server/dist/picoturtle-server.exe"];
}
extraResource.push()

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
    hooks: {
        packageAfterCopy: async (config, buildPath, electronVersion, platform, arch) => {
            console.log(buildPath);
        }
    }
  };

  module.exports = config;
var isWin = process.platform === "win32";

let extraResource = ["../server/dist/picoturtle-server"];
if (isWin) {
    extraResource = ["../server/dist/picoturtle-server.exe"];
}

let config = {
    "packagerConfig": {
      "extraResource": extraResource
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
    ]
  };

  module.exports = config;
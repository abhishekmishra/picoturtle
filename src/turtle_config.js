let fs = require('fs-extra');
let os = require('os');

class TurtleConfig {
    constructor() {
        this.configFile = os.homedir() + '/.picoturtle';
        this.config = null;

        console.log('Reading config at - ' + this.configFile);
        if(!fs.existsSync(this.configFile)) {
            fs.copy('./src/conf/picoturtle.config.default', this.configFile)
            .then(() => {
                console.debug('reading config file at ' + this.configFile);
                this.readConfig();
            })
            .catch((err) => {
                console.error('Unable to write a new config file, ' + err);
            });
        }
    }

    readConfig() {
        fs.readFile(this.configFile, (err, data) => {
            if(err) {
            console.error('error reading file ' + err);
            } else {
                try {
                    this.config = JSON.parse(data);
                } catch(e) {
                    console.error('Error parsing config ' + e);
                }
            }
        });
    }

    get(param) {
        return this.config[param];
    }
}

module.exports.TurtleConfig = TurtleConfig;
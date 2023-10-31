const lv = require('lv')
const Part = require('./Part')
const config = require("./part.lib/screen.js")

class BeScreen extends Part{
    constructor(parent) {
        super(parent)
        this.loadConf(this.config())
    }
}

class BeScreen130 extends BeScreen{
    static config = config["130"]
    constructor(parent) {
        super(parent)
    }
    
}
class BeScreen096 extends BeScreen{
    static config = config["096"]
    constructor(parent) {
        super(parent)
    }
    
}

exports.BeScreen130 = BeScreen130
exports.BeScreen096 = BeScreen096
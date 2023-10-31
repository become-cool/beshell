const lv = require('lv')
const HostPart = require('./HostPart')

const ScreenWidth = 77.6
const ScreenHeight = 58 // 54

class BePad extends HostPart {
    static config = require("./part.lib/003.js")
    constructor(parent) {
        super(parent)

        this.loadConf(BePad.config)

        this.refs = this.fromJson({
            children: [
                {
                    center: true ,
                    bubble: true ,
                    ref: "screen" ,
                    style:{
                        "border-width": 0 ,
                        "bg-color": lv.rgb(0,0,0) ,
                        radius: 0 ,
                    }
                }
            ]
        })
    }

    repaint(zoom) {
        super.repaint(zoom)
        if(this.dir==1||this.dir==3) {
            this.refs.screen.setWidth(ScreenWidth*zoom)
            this.refs.screen.setHeight(ScreenHeight*zoom)
        }else{
            this.refs.screen.setWidth(ScreenHeight*zoom)
            this.refs.screen.setHeight(ScreenWidth*zoom)
        }
    }
}

module.exports = BePad
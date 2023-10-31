const lv = require('lv')

const RAD = 3

class Port extends lv.Canvas{

    female = false

    constructor(parent, conf, female) {
        super(parent, conf)
        this.female = !!female

        this.conf = conf
        // if(conf.x) {
        //     this.setX(conf.x*PPM)
        // }
        // if(conf.y) {
        //     this.setY(conf.y*PPM)
        // }

        this.refs = this.fromJson({
            style: {
                // radius: 3*PPM ,
                "border-width": 1 ,
                "border-color": lv.palette("blue-grey") ,
            } ,
            center: true ,
            children: [
                // {
                //     class: 'CleanObj' ,
                //     x: 1*PPM ,
                //     y: 1*PPM ,
                //     width: 6*PPM ,
                //     height: 6*PPM ,
                //     bubble: true ,
                //     ref: "port" ,
                //     style: {
                //     }
                // } ,
                
                // {
                //     class: 'Label' ,
                //     width: -1 ,
                //     height: 16 ,
                //     center: true ,
                //     style: {}
                // }
            ]
        })

        this.draggable()
    }


    repaint(zoom, dir) {
        this.setStyle("radius", RAD*zoom)
        this.setWidth(RAD*2*zoom)
        this.setHeight(RAD*2*zoom)

        if(dir==1) {
            this.setX(this.conf.x*zoom)
            this.setY(this.conf.y*zoom)
        }
        else if(dir==2) {
            this.setX(-this.conf.y*zoom)
            this.setY(-this.conf.x*zoom)
        }
        else if(dir==3) {
            this.setX(-this.conf.x*zoom)
            this.setY(-this.conf.y*zoom)
        }
        else if(dir==4) {
            this.setX(this.conf.y*zoom)
            this.setY(this.conf.x*zoom)
        }
    }
}

module.exports = Port
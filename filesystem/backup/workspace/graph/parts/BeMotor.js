const lv = require('lv')
const Part = require('./Part')
const config = require("./part.lib/motor.js")

const PartWidth = 40
const PartHeight = 34.8

class BeMotor extends Part{
    static config = config.motor
    constructor(graph) {
        super(graph)
        this.removeStyleAll()

        this.docW = PartWidth
        this.docH = PartHeight

        this.dscBody = new lv.DrawRectDsc()
        this.dscBody.setBorderWidth(0)
        this.dscBody.setBgColor(lv.palette("orange"))

        let dscAxie = new lv.DrawRectDsc()
        dscAxie.setBorderWidth(1)
        dscAxie.setBorderColor(lv.palette("grey"))
        // dscAxie.setBorderSide( lv.side.top|lv.side.left|lv.side.right )
        dscAxie.setBgColor(lv.rgb(255,255,255))

        this.on("draw-main",(ename, target, clip)=>{
            let zoom = graph.zoom
            this.updateCenterCoords()

            lv.drawPolygon([
                this.local(1.6, -17.4, zoom) ,
                this.local(6.4, -17.4, zoom) ,
                this.local(6.4, -11.4, zoom) ,
                this.local(1.6, -11.4, zoom) ,
            ], clip, dscAxie)

            lv.drawPolygon([
                this.local(-12, -8.2, zoom) ,
                this.local(12, -8.2, zoom) ,
                this.local(12, 17.4, zoom) ,
                this.local(-12, 17.4, zoom) ,
            ], clip, this.dscBody)

            lv.drawPolygon([
                this.local(12, -1.8, zoom) ,
                this.local(20, -1.8, zoom) ,
                this.local(20, 7.8, zoom) ,
                this.local(12, 7.8, zoom) ,
            ], clip, this.dscBody)
            lv.drawPolygon([
                this.local(-12, -1.8, zoom) ,
                this.local(-20, -1.8, zoom) ,
                this.local(-20, 7.8, zoom) ,
                this.local(-12, 7.8, zoom) ,
            ], clip, this.dscBody)
            
            lv.drawPolygon([
                this.local(-6, -11.4, zoom) ,
                this.local(10, -11.4, zoom) ,
                this.local(10, -8.2, zoom) ,
                this.local(-6, -8.2, zoom) ,
            ], clip, this.dscBody)

        })
    }
}

class BeServo extends BeMotor{
    static config = config.servo
    constructor(graph) {
        super(graph)

        this.dscBody.setBgColor(lv.palette("grey"))

    }
}

exports.BeMotor = BeMotor
exports.BeServo = BeServo
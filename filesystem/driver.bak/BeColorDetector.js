const BeSensorBase = require("./BeSensorBase")

class ColorDetector extends BeSensorBase {

    constructor(){
        super()
        this.i2c = I2C1
        this._itid = -1
        this.c = 0
        this.r = 0
        this.g = 0
        this.b = 0
    }

    begin(i2c, pinSource, pinGnd) {
        this.i2c = i2c? i2c: I2C1
        
        if(pinSource!=undefined) {
            digitalWrite(pinSource, 1)
        }
        if(pinGnd!=undefined) {
            digitalWrite(pinGnd, 0)
        }

        this.setIntegration(0xF6)
        this.setGain(1)

        return this.setEnable(true)
    }

    setIntegration(time) {
        this.i2c.writeTo(0x29, 0x81, time)
    }
    setGain(gain) {
        this.i2c.writeTo(0x29, 0x8F, gain)
    }
    async setEnable(enable) {
        if(enable) {
            this.i2c.writeTo(0x29, 0x80, 1)
            await sleep(3)
            this.i2c.writeTo(0x29, 0x80, 3)
        }
        else {
            this.i2c.writeTo(0x29, 0x80, 0)
        }
    }

    readRaw() {
        this.c = this.readC()
        this.r = this.readR()
        this.g = this.readG()
        this.b = this.readB()
        return [this.c, this.r, this.g, this.b]
    }
    read() {
        this.readRaw()
        this.r = Math.round(this.r/this.c*255)
        this.g = Math.round(this.g/this.c*255)
        this.b = Math.round(this.b/this.c*255)
        return [this.r, this.g, this.b]
    }
    readC() {
        return this.i2c.readInt16(0x29, 0x94)
    }
    readR() {
        return this.i2c.readInt16(0x29, 0x96)
    }
    readG() {
        return this.i2c.readInt16(0x29, 0x98)
    }
    readB() {
        return this.i2c.readInt16(0x29, 0x9A)
    }

    begin() {
        if(this._itid>-1) {
            clearInterval(this._itid)
        }
        this._itid = setInterval(()=>{
            let  r = this.r, g = this.g, b = this.b
            this.read()
            if( r!=this.r || g!=this.g || b!=this.b ) {
                for(let wid in this._handlers) {
                    this._handlers[wid] (this.r, this.g, this.b)
                }
            }
        }, 100)
    }

    end() {
        if(this._itid>-1) {
            clearInterval(this._itid)
            this._itid = -1
        }
    }

}


module.exports = ColorDetector
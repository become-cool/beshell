const ADDR = 0x68
class BePoser extends beapi.EventEmitter {
    constructor(bus, sda, scl) {
        super()
        this.bus = bus || 0
        this.sda = sda==undefined? 12: sda
        this.scl = scl==undefined? 14: scl
        this._dmpdata = null
        this.q = {x:0, y:0, z:0, w:0}
    }
    async readDMP(euler) {
        let fifocnt = this.fifoCount()
        if(fifocnt>=512) {
            this.fifoReset()
            fifocnt = this.fifoCount()
        }
        for(let i=0;i<10;i++) {
            if(fifocnt>=42) {
                this._dmpdata = beapi.i2c.read(0, ADDR, 0x74, 42)
                let data = new DataView(this._dmpdata,0,16)
                this.q.x = data.getInt32(0)/1073741824
                this.q.y = data.getInt32(4)/1073741824
                this.q.z = data.getInt32(8)/1073741824
                this.q.w = data.getInt32(12)/1073741824
                beapi.i2c.send(0, ADDR, [0x6A, 0xcc]) // 0xcc = FIFO_RST|DMP_RST|DMP_EN|FIFO_EN
                return euler? this.toEuler(): this.q
            }
            await sleep(1)
            fifocnt = this.fifoCount()
        }
    }
    async begin(bus, sda, scl) {
        if(bus!=undefined) {
            this.bus = bus
        }
        if(sda!=undefined) {
            this.sda = sda
        }
        if(scl!=undefined) {
            this.scl = scl
        }
        beapi.i2c.setup(this.bus, this.sda, this.scl, 400000)

        // 重置
        beapi.i2c.send(this.bus, ADDR, [0x6B, 0x80])
        await sleep(100)
        beapi.i2c.send(this.bus, ADDR, [0x6B, 0])
        beapi.i2c.send(this.bus, ADDR, [0x6C, 0])

        beapi.i2c.send(this.bus, ADDR, [0x19, 0x04]) // 1000/(4+1) = 200hz
        beapi.i2c.send(this.bus, ADDR, [0x1A, 0x06]) //1KHZ
        beapi.i2c.send(this.bus, ADDR, [0x1B, 0x18]) //+- 2000
        beapi.i2c.send(this.bus, ADDR, [0x1C, 0x10]) //+- 4g

        if(!beapi.i2c.mpu9250load()){
            console.log("BePoser device is invalid.")
            return false
        }

        beapi.i2c.send(this.bus, ADDR, [0x70, 0x03])
        beapi.i2c.send(this.bus, ADDR, [0x71, 0x00])
        beapi.i2c.send(this.bus, ADDR, [0x6A, 0xCC])

        return true
    }
    fifoReset() {
        let regv = beapi.i2c.readUInt8(this.bus, ADDR, 0x6A)
        beapi.i2c.send(this.bus, ADDR, [0x6A, regv|4])
    }
    fifoCount() {
        return beapi.i2c.readUInt16(this.bus, ADDR, 0x72)
    }
    toEuler() {
        return {
            pitch: -Math.asin(2.0 * (this.q.x * this.q.z - this.q.w * this.q.y)) * 57.3 ,
            roll: Math.atan2(2.0 * (this.q.x * this.q.y + this.q.z * this.q.w), 1-2.0*(this.q.y * this.q.y + this.q.z * this.q.z))*57.3 ,
            yaw:  Math.atan2(2.0 * (this.q.x * this.q.w + this.q.y * this.q.z), 1-2.0 * (this.q.z * this.q.z + this.q.w * this.q.w)) * 57.3 ,
        }
    }
}


module.exports = BePoser
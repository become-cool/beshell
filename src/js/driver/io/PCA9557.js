import * as driver from 'driver'
import { exportValue } from "loader"

const mapModeValue = {
    output: 0 ,
    input: 1
}
// mode: 1=输入, 0=输出
class PCA9557 extends driver.I2CDevice {
    mode = 0
    output = 0
    setup(opt) {
        super.setup(opt)
        if(opt.mode!=undefined) {
            this.setMode(opt.mode)
        } else {
            this.getMode(true)
        }
    }
    read(pin) {
        let [mode] = this.readReg(0)
        return (this.mode & (1<<pin))? 1: 0
    }
    write(pin,level) {
        let output = (this.output & ~(1 << pin)) | (level << pin)
        this.writeReg(1, output)
        this.output = output
    }
    getMode(sync) {
        if(sync) {
            [this.mode] = this.readReg(3)
        }
        return this.mode
    }
    setMode(mode) {
        this.writeReg(3,mode)
        this.mode = mode
    }
    getPinMode(pin,sync) {
        if(sync) {
            this.getMode(true)
        }
        return (this.mode & (1<<pin))? "input": "ouput"
    }
    setPinMode(pin,dir,sync) {
        if(mapModeValue[dir]!=undefined) {
            dir = mapModeValue[dir]
        }
        if(dir!=1 && dir!=0) {
            throw new Exception("invalid pin dir, must be 1(input), 0(output)")
        }
        if(sync) {
            this.getMode(true)
        }
        if(dir) {
            this.setMode(this.mode | (1<<pin))
        } else {
            this.setMode(mode & (~(1<<pin)))
        }
    }
    printMode() {
        for(let i=0;i<8;i++) {
            console.log(`pin[${i}]: ${this.getPinMode(i)}`)
        }
    }
}

exportValue(driver, "PCA9557", PCA9557)

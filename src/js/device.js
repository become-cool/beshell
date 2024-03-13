import {deviceJsonPath} from 'device'
import * as serial from 'serial'

function createDevFromDriver(devConf) {
    try{
        // console.log("besdk/driver/"+devConf.driver+'.js')
        var driver = require("besdk/driver/"+devConf.driver+'.js')
    }catch(e){
        console.error(e)
        console.error("unknow driver", devConf.driver)
    }
    try {
        let dev = new driver
        if(dev.setup(devConf.setup)==false) {
            console.error(devConf.driver,'device setup failed.')
            return
        }
        if(dev.begin(devConf.begin)==false) {
            console.error(devConf.driver,'device begin failed.')
            return
        }
        dev.register(devConf.varname)
        return dev
    }catch(e) {
        console.error(e)
    }
}
(function(){
    let deviceConf = JSON.loadSync(deviceJsonPath,null)
    if(!deviceConf) {
        console.log(deviceJsonPath, "not exists or invalid.") ;
        return ;
    }
    // serial bus
    for(let num in deviceConf.spi||{}){
        let bus = 'spi'+num
        let conf = deviceConf.i2c[num]
        if(!serial[bus]) {
            console.log("unknown spi bus",num)
        } else {
            serial[bus].setup(conf.miso, conf.mosi, conf.sck)
            console.log(`setup SPI ${num}, miso:${conf.miso}, mosi:${conf.mosi}, sck:${conf.sck}`)
        }
    }
    for(let num in deviceConf.i2c||{}){
        let bus = 'i2c'+num
        let conf = deviceConf.i2c[num]
        if(!serial[bus]) {
            console.log("unknown i2c bus",num)
        } else {
            serial[bus].setup(conf.sda, conf.scl, conf.freq||100000)
            console.log(`setup I2C ${num}, sda:${conf.sda}, scl:${conf.scl}`)
        }
    }
    
    // dev
    for(let devConf of deviceConf.dev||[]){
        try {
            if(devConf.disable) {
                continue
            }
            createDevFromDriver(devConf)
        }catch(e){
            console.log(e)
        }
    }
})()
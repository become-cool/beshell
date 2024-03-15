import {deviceJsonPath, device} from 'device'
import * as serial from 'serial'
import * as driver from 'driver'
import * as lv from 'lv'
import {importSync} from 'loader'

(async function (){
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

    let lvdevs = []
    
    // dev
    for(let devConf of deviceConf.dev||[]){
        if(devConf.disable) {
            continue
        }
        let module = driver
        if(devConf.module) {
            module = importSync(devConf.module)
        }
        let driverClass = module[devConf.driver]
        if(!driverClass) {
            console.error("unknow driver", devConf.driver)
            continue ;
        }
        try {
            let dev = new driverClass
            if(dev.setup && dev.setup(devConf.setup)==false) {
                console.error(devConf.driver,'device setup failed.')
                continue
            }
            if(dev.begin && dev.begin(devConf.begin)==false) {
                console.error(devConf.driver,'device begin failed.')
                continue
            }
            if(devConf.lv) {
                if(typeof devConf.lv=='string') {
                    devConf.lv = {type:devConf.lv}
                }
                if(devConf.lv.type=='input') {
                    lvdevs.push([dev,devConf])
                } else if (devConf.lv.type=='display') {
                    lvdevs.unshift([dev,devConf])
                } else {
                    console.error("unknown lv type", devConf.lv.type)
                }
            }
            let varname = devConf.name || driverClass.name
            device[varname] = dev
        }catch(e) {
            console.error(devConf.driver,'device init failed.')
            console.error(e)
        }
    }

    // lv
    for(let [dev,devConf] of lvdevs) {
        try {
            if(devConf.lv.type=='input') {
                lv.registerInputDevice(dev, devConf.lv.option)
            } else if (devConf.lv.type=='display') {
                lv.registerDisplay(dev, devConf.lv.option)
            } else {
                console.error("unknown lv type", devConf.lv.type)
            }
        }catch(e){
            console.error(e)
        }
    }
}) ()
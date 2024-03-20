import {deviceJsonPath, device} from 'device'
import * as serial from 'serial'
import * as driver from 'driver'
import {importSync} from 'loader'

function setupSerialBus(type, num, conf) {
    let varname = type+num
    if(!serial[varname]) {
        console.log("unknown",type,"bus:",varname)
    } else {
        try {
            serial[varname].setup(conf)
        } catch(e) {
            console.error("setup",varname,"failed.")
            console.error(e)
        }
    }
}

(async function (){
    let deviceConf = JSON.loadSync(deviceJsonPath,null)
    if(!deviceConf) {
        console.log(deviceJsonPath, "not exists or invalid.")
        return ;
    }
    console.log("load device config from:", deviceJsonPath)
    for(let prepheral of ['spi','i2c','uart','i2s']) {
        for(let num in deviceConf[prepheral]||{}){
            setupSerialBus(prepheral,num,deviceConf[prepheral][num])
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
    try{
        var lv = await import('lv')
    }catch(e){}
    if(lv) {
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
    }
}) ()
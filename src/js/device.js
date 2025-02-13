import * as dt from 'dt'
import * as driver from 'driver'
import * as gpio from 'gpio'
import { importSync, exportValue } from 'loader'

try {
  var serial = importSync("serial")
} catch (e) {}

const jsDrivers = {}

function setupSerialBus(type, num, conf) {
  if(!serial) {
    console.error("serial module not used.")
    return
  }
  let varname = type + num
  if (!serial[varname]) {
    console.log("unknown", type, "bus:", varname)
  } else {
    try {
      serial[varname].setup(conf)
    } catch (e) {
      console.error("setup", varname, "failed.")
      console.error(e)
    }
  }
}

function load(deviceConf) {
  if( typeof deviceConf =='string' ) {
    let json = JSON.loadSync(deviceConf, null)
    if (!deviceConf) {
      throw new Error(deviceConf + " not exists or invalid.")
    }
    console.log("load device config from:", deviceConf)
    deviceConf = json
  }

  // gpio
  if (deviceConf.gpio) {
    for (let pin in deviceConf.gpio) {
      let gpioConf = deviceConf.gpio[pin]
      if (gpioConf.mode) {
        gpio.setMode(pin, gpioConf.mode)
      }
      if (gpioConf.pull) {
        gpio.pull(pin, gpioConf.pull)
      }
      if (gpioConf.level) {
        gpio.write(pin, gpioConf.level ? 1 : 0)
      }
    }
  }

  // serial
  for (let prepheral of ['spi', 'i2c', 'uart', 'i2s']) {
    for (let num in deviceConf[prepheral] || {}) {
      setupSerialBus(prepheral, num, deviceConf[prepheral][num])
    }
  }

  let lvdevs = []
  let devId = {}

  // dev
  for (let devConf of deviceConf.dev || []) {
    if (devConf.disable) {
      continue
    }
    let module = driver
    if (devConf.module) {
      module = importSync(devConf.module)
    }
    let driverClass = module[devConf.driver] || jsDrivers[devConf.driver]
    if (!driverClass) {
      console.error("unknow driver", devConf.driver)
      continue;
    }
    try {
      let dev = new driverClass
      if (dev.setup && dev.setup(devConf.setup) == false) {
        console.error(devConf.driver, 'device setup failed.')
        continue
      }
      if (dev.begin && dev.begin(devConf.begin) == false) {
        console.error(devConf.driver, 'device begin failed.')
        continue
      }
      if (devConf.lv) {
        if (typeof devConf.lv == 'string') {
          devConf.lv = { type: devConf.lv }
        }
        if (devConf.lv.type == 'input') {
          lvdevs.push([dev, devConf])
        } else if (devConf.lv.type == 'display') {
          lvdevs.unshift([dev, devConf])
        } else {
          console.error("unknown lv type", devConf.lv.type)
        }
      }
      let id = 0
      if (!devId[driverClass.name]) {
        devId[driverClass.name] = 0
      } else {
        id = devId[driverClass.name]++
      }
      let varname = driverClass.name.toLowerCase()
      if(id==0) {
        dt.device[varname] = dev
      }
      dt.device[varname][id] = dev
      if (devConf.name && !dt.device[devConf.name]) {
        dt.device[devConf.name] = dev
      }
    } catch (e) {
      console.error(devConf.driver, 'device init failed.')
      console.error(e)
    }
  }

  // lv
  if (lvdevs.length > 0) {
    try {
      var lv = importSync('lv')
    } catch (e) { }
    if (lv) {
      for (let [dev, devConf] of lvdevs) {
        try {
          if (devConf.lv.type == 'input') {
            lv.registerInputDevice(dev, devConf.lv)
          } else if (devConf.lv.type == 'display') {
            lv.registerDisplay(dev, devConf.lv)
          } else {
            console.error("unknown lv type", devConf.lv.type)
          }
        } catch (e) {
          console.error(e)
        }
      }
    }
  }
}

function registerDriver(clazz, name) {
  if(typeof clazz!='function') {
    throw new Error('driver must be a class')
  }
  if(!name) {
    name = clazz.name
  }
  if(!name) {
    throw new Error('driver must have a name')
  }
  if(jsDrivers[name]) {
    throw new Error('driver already registered')
  }
  jsDrivers[name] = clazz
}
function unregisterDriver(name) {
  delete driver[name]
}

exportValue(dt, 'load', load)
exportValue(dt, 'registerDriver', registerDriver)
exportValue(dt, 'unregisterDriver', unregisterDriver)
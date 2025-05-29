import * as bt from "bt"
import { exportValue } from "loader"

const mapHandleServices = {}
const mapHandleChars = {}
const mapUUIDChars = charGetter

let peerConnId = null

/*
opt = {
    uuid: "180d",
    primary: true,
    chars: [
        {
            uuid: "2a37",
            props: ["write"|"writeNR", "notify"|"indicate"]
        }
    ]
}
*/
class Service {
  constructor(opt) {
    this.uuid = opt.uuid
    this.primary = opt.primary
    this.chars = {}

    this.handle = bt.addService(this.uuid, 10)

    for (let charOpt of opt.chars) {
      let char = new Characteristic(this.handle, charOpt)
      this.chars[charOpt.uuid] = char
    }

    mapHandleServices[this.handle] = this
  }
}

class Characteristic extends EventEmitter {
  constructor(serviceHandle, opt) {
    super()
    this.serviceHandle = serviceHandle
    this.uuid = opt.uuid
    this.props = opt.props
    this.handle = bt.addChar(this.serviceHandle, this.uuid, this.props)
    this.notifiable = opt.props.includes("notify")
    this.indicatable = opt.props.includes("indicate")

    mapHandleChars[this.handle] = this
    mapUUIDChars[opt.uuid] = this
    if (opt.uuid.length==36 && opt.uuid[8]=="-") {
      mapUUIDChars[opt.uuid.substr(0, 8)] = this
    }
  }

  notify(data) {
    if(!this.notifiable) {
      throw new Error("notify not supported")
    }
    if (isNaN(parseInt(peerConnId))) {
      throw new Error("perihperal not connected")
    }
    bt.sendNotify(this.handle, data, false, peerConnId)
  }
  indicate(data) {
    if(!this.indicatable) {
      throw new Error("indicate not supported")
    }
    if (isNaN(parseInt(peerConnId))) {
      throw new Error("perihperal not connected")
    }
    bt.sendNotify(this.handle, data, true, peerConnId)
  }
}

function charGetter(uuid) {
  return mapUUIDChars[uuid] || null
}


class Peripheral extends EventEmitter {
  constructor() {
    super()
    this.char = charGetter
  }
  init() {
    bt._initPeripheral()

    bt.on("periph.connect", (connId, addr) => {
      peerConnId = connId
      this.emit("connect", connId)
    })
    bt.on("periph.disconnect", (connId, addr, reason) => {
      if( peerConnId == connId ){
        peerConnId = null
      }
      this.emit("disconnect", connId, reason)
    })
    bt.on("periph.open", (status) => {
      this.emit("open", status)
    })
    bt.on("periph.close", (status) => {
      this.emit("close", status)
    })
    bt.on("WRITE", (handle, data, connId) => {
      let char = mapHandleChars[handle]
      if (char) {
        char.emit("write", data, connId)
      }
    })
  }

  addService(opt) {
    let svr = new Service(opt)
    return svr
  }
}

const peripheral = new Peripheral()

exportValue(bt, "peripheral", peripheral)
exportValue(bt, "periph", peripheral)
exportValue(bt, "pher", peripheral)
 
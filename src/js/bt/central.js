import * as bt from "bt"
import { exportValue } from "loader"

class Central extends EventEmitter {

  constructor() {
    super()
    this._mapConnectResolves = {}
    this._mapConnId = {}
    this._mapAddr = {}
  }

  init() {

    bt._initCentral()

    bt.on("central.connect", (connid, addr) => {
      this.emit("connect", connid, addr)
    })
    bt.on("central.disconnect", (connid, addr) => {
      this.emit("disconnect", connid, addr)
    })
    bt.on("central.open", (status, connid, addr) => {
      console.log("central.open", status, connid, addr)
      if (status == 0) {
        this._mapConnId[connid] = new Peer(addr, connid)
        this._mapAddr[addr] = this._mapConnId[connid]
      } else {
        this._mapConnectResolves[addr](status, addr)
        delete this._mapConnectResolves[addr]
      }
    })
    bt.on("central.close", (connid, addr, reason) => {
      this.emit("close", connid, addr, reason)
    })
    bt.on("dis-srvc-cmpl", (status, connid) => {
      let peer = this._mapConnId[connid]
      if (!peer) {
        console.log("unknow conn id for event dis-srvc-cmpl", connid)
        return
      }
      if (this._mapConnectResolves[peer.addr]) {
        this._mapConnectResolves[peer.addr](status, peer)
        delete this._mapConnectResolves[peer.addr]
      }
      else {
        console.log("dis-srvc-cmpl not found callback",peer.addr,connid,status)
      }
    })
    bt.on("search-res", (connid, service) => {
      // console.log("search-res",connid, service)
      if (!this._mapConnId[connid]) {
        return
      }
      this._mapConnId[connid].emit("search", service)
    })
    bt.on("search-cmpl", (status, connid) => {
      // console.log("search-cmpl",status,connid)
      if (!this._mapConnId[connid]) {
        return
      }
      this._mapConnId[connid].emit("search-cmpl", status)
    })
    bt.on("cfg-mtu", (status, connid, mtu) => {
      this._mapConnId[connid]?.emit("cfg-mtu", status, mtu)
    })
  }

  connect(addr,ms=0) {
    let timeout = false
    let timer = -1
    return new Promise((resolve, reject) => {

      if(ms>0) {
        setTimeout(()=>{
            timeout = true
            reject(new Error('Connection timed out after ' + ms + 'ms'));
        }, ms)
      }

      addr = addr.toUpperCase()
      bt.connect(addr)

      this._mapConnectResolves[addr] = (status, peer) => {
        // 在连接成功之前触发了超时，主动断开，放弃连接
        if(timeout) {
          peer.disconnect()
          return
        }
        // 清除定时器
        if(timer>-1) {
          clearTimeout(timer)
        }
        if (status == 0)
          resolve(peer)
        else
          reject(new Error("connect failed with error: " + status))
      }
    })
  }
}

class Peer extends EventEmitter {

  _mapService = {}

  constructor(addr, connid) {
    super()
    this.addr = addr
    this.connid = connid

    this.on("search", (service) => {
      this._mapService[service.uuid] = {
        uuid: service.uuid,
        chars: {}
      }
      for (let char of service.chars) {
        let objChar = new Characteristic(this.connid, char.addr, char.props, char.handle, char.desc)
        this._mapService[service.uuid].chars[char.addr] = objChar
        this[char.addr] = objChar
        // console.log(char.addr.length, char.addr)
        if (char.addr.length == 36) {
          this[char.addr.substr(0, 8)] = objChar
        }
      }
    })
    this.on("search-cmpl", (status) => {
      if (status) {
        this.searchingReject(new Error("search failed with error: " + status))
      } else {
        this.searchingResolve()
      }
      this.searchingResolve = null
      this.searchingReject = null
    })
  }

  search() {
    return new Promise((resolve, reject) => {
      this._mapService = {}
      this._mapChars = {}
      this.searchingResolve = resolve
      this.searchingReject = reject
      bt.search(this.connid)
    })
  }

  setMTU(mtu) {
    bt.setMTU(mtu)
    bt.requestMTU(this.connid)
    return new Promise((resolve, reject) => {
      this.once("cfg-mtu", (status, mtc) => {
        if (status) {
          reject(new Error("set mtu failed with error: " + status))
        } else {
          resolve(mtc)
        }
      })
    })
  }

  disconnect(){
    bt.disconnect(this.connid)
  }
}

class Characteristic extends EventEmitter {
  constructor(connid, uuid, props, handle, lstDesc) {
    super()
    this.connid = connid
    this.uuid = uuid
    this.props = props
    this.handle = handle
    this.desc = {}
    if (lstDesc) {
      for (let uuid in lstDesc) {
        this.desc[uuid] = lstDesc[uuid].handle
      }
    }
    bt.on("write-char", (status, handle, offset) => {
      if (this.handle != handle) return
      // console.log(status,offset)
      this.emit("write-char", status, offset)
    })
    bt.on("write-desc", (status, handle, offset) => {
      if (this.desc["0x2902"] != handle) return
      this.emit("subscribe", status, offset)
    })
    bt.on("read-char", (status, handle, data) => {
      if (this.handle != handle) return
      this.emit("read-char", status, data)
    })
  }
  write(data,rsp) {
    bt.write(this.connid, this.handle, data, !!rsp)
    return new Promise((resolve, reject) => {
      this.once("write-char", (status, offset) => {
        if (status) {
          console.log("write failed with error: " + status)
          reject(new Error("write failed with error: " + status))
        } else {
          resolve()
        }
      })
    })
  }
  read() {
    bt.read(this.connid, this.handle)
    return new Promise((resolve, reject) => {
      this.once("read-char", (status, data) => {
        if (status) {
          console.log("write failed with error: " + status)
          reject(new Error("write failed with error: " + status))
        } else {
          resolve(data)
        }
      })
    })
  }
  subscribe() {
    if (!this.desc["0x2902"]) {
      throw new Error("not support subscribe, there is no 0x2902 desc")
    }
    bt.subscribe(this.connid, this.desc["0x2902"])
    return new Promise((resolve, reject) => {
      this.once("subscribe", (status) => {
        if (status) {
          reject(new Error("subscribe failed with error: " + status))
        } else {
          resolve()
        }
      })
    })
  }
}


const mapFieldName = {
  0x01:'flag', // Flag
  0x02:'more16', // Use of more than 16-bit UUID
  0x03:'complete16', // Complete List of 16-bit UUID
  0x04:'more32', // Use of more than 32-bit UUD
  0x05:'complete32', // Complete List of 32-bit UUID
  0x06:'more128', // Use of more than 128-bit UUID  
  0x07:'complete128', // Complete List of 128-bit UUID
  0x08:'shortName', // Shortened Device name
  0x09:'name', // Complete Device name
  0x0A:'txPower', // Transmit power
  0x0D:'classOfDevice', // Class of device
  0x0E:'spHashC', // Simple Pairing Hash C
  0x0F:'spRandomizerR', // Simple Pairing Randomizer
  0x10:'tkValue', // Temporary key value
  0x11:'oobFlags', // Out of Band Flag
  0x12:'slaveConnIntRange', // Slave connection interval range
  0x14:'rqrd16', // Require 16-bit service UUID
  0x1F:'rqrd32', // Require 32-bit service UUID
  0x15:'rqrd128', // Require 128-bit service UUID
  0x16:'service16', // Service data 16-bit UUID
  0x20:'service32', // Service data 32-bit UUID
  0x21:'service128', // Service data 128-bit UUID
  0x17:'pubTgtAddr', // Public Target Address
  0x18:'randTgtAddr', // Random Target Address
  0x19:'appearance', // Appearance
  0x1A:'advIntv', // Advertising Interval
  0x1B:'leBtAddr', // LE Bluetooth Device Address
  0x1C:'leRole', // LE Role
  0x1D:'spairHash', // Simple Pairing Hash C-256
  0x1E:'spairRand', // Simple Pairing Randomizer R-256
  0x3D:'3dInfo', // 3D Information Data
  0xFF:'msd', // Manufacturer specific data
}
function parseAdv(raw) {
  let adv = {}
  var data = new Uint8Array(raw)
  let offset = 0
  while (offset+1 < data.length) {
    let len = data[offset]
    let type = data[offset + 1]
    adv[mapFieldName[type]||type] = data.slice(offset+2, offset+len+1)
    offset += len + 1
  }
  return adv
}

function waitScanning(ms) {
  if(!ms) ms  = 10000
  let timeout = false
  return new Promise((resolve,reject)=>{
    if(!bt.isScanning()) {
      resolve()
      return
    }
    let timer = setTimeout(()=>{
      reject("timeout")
      clearTimeout(timer)
      timer = -1
    },ms)
    bt.once("scan-cmpl",()=>{
      if(!timeout) {
        resolve()
        clearTimeout(timer)
        timer = -1
      }
    })
  })
}

export const central = new Central
exportValue(bt,"central",central)
exportValue(bt,"cent",central)
exportValue(bt,"parseAdv",parseAdv)
exportValue(bt,"waitScanning",waitScanning)
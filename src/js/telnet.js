import { importSync, exportValue } from 'loader'
import * as telnet from 'telnet'
import * as mg from 'mg'

// websocket
const ws = {
  connect(url) {

    let channel = new telnet.TelnetChannel()

    let conn = mg.connect(url, (ev, data, isBinary) => {
      if(ev=="ws.msg") {
        channel && channel.process(data)
      }
      else if(['error','close'].includes(ev)){
        channel = null
      }
    })

    channel.on("output-stream", (data) => {
      if (conn && conn.isConnected()) {
        conn.send(data)
      }
    })
    
    return conn
  }
}

// BLE
try {
  var bt = importSync("bt")
} catch (e) {}

let bleStarted = false
let bleChannel = null

const ble = {
  start(advName=null, serviceId="0B0C", charId="0512") {
    if(!bt) {
      throw new Error("bt module not used, call `beshell.use<BT>()` in C++ first")
    }
    if(bleStarted) {
      throw new Error("telnet.ble already started")
    }
    bt.periph.init()

    bt.setMTU(256)

    bt.periph.addService({
      uuid: serviceId ,
      chars: [{
        uuid: charId ,
        props: ["write","notify"]
      }]
    })

    bt.periph.char[charId].on("write", (data) => {
      bleChannel && bleChannel.process(data)
    })

    bt.on("disconnect", () => {
      bleChannel = null
      bt.startAdv()
    })
    
    bt.on("connect", () => {
      bleChannel = new telnet.TelnetChannel()
      bleChannel.on("output-stream", (data) => {
        bt.periph.char[charId] && bt.periph.char[charId].notify(data)
      })
    })

    if(advName) {
      let u8arr = new Uint8Array(advName.length+2);
      u8arr[0] = advName.length+1 ; // length
      u8arr[1] = 0x09 ; // type
      for (let i = 0; i < advName.length; i++) {
        u8arr[i+2] = advName.charCodeAt(i) ; // data
      }
      bt.setAdvData(u8arr.buffer)
      bt.startAdv({
        min: 160,
        max: 170, // 间隔时间: 250-300ms
      })
    }

    bleStarted = true
  }
}

exportValue(telnet, "ws", ws)
exportValue(telnet, "ble", ble)
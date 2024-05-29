import { exportValue } from "loader"
import * as wifi from "wifi"

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms))
}
;["on", "off", "once", "race", "emit", "originHandle"].map(fname => exportValue(wifi, fname, EventEmitter.prototype[fname]))

// wifi mode
// const MODE_NONE = 0
const MODE_STA = 1
const MODE_AP = 2
// const MODE_APSTA = 3


// event type
const EVENT_WIFI = 1
const EVENT_IP = 2

// WiFi events
const WIFI_READY = 0
const WIFI_SCAN_DONE = 1
const WIFI_STA_START = 2
const WIFI_STA_STOP = 3
const WIFI_STA_CONNECTED = 4
const WIFI_STA_DISCONNECTED = 5
// const WIFI_STA_AUTHMODE_CHANGE = 6
// const WIFI_STA_WPS_ER_SUCCESS = 7
// const WIFI_STA_WPS_ER_FAILED = 8
// const WIFI_STA_WPS_ER_TIMEOUT = 9
// const WIFI_STA_WPS_ER_PIN = 10
// const WIFI_STA_WPS_ER_PBC_OVERLAP = 11
const WIFI_AP_START = 12
const WIFI_AP_STOP = 13
const WIFI_AP_STACONNECTED = 14
const WIFI_AP_STADISCONNECTED = 15
// const WIFI_AP_PROBEREQRECVED = 16
// const WIFI_FTM_REPORT = 17
// const WIFI_STA_BSS_RSSI_LOW = 18
// const WIFI_ACTION_TX_STATUS = 19
// const WIFI_ROC_DONE = 21
// const WIFI_STA_BEACON_TIMEOUT = 21
const WIFI_STA_CONNECTING = 101

let evtNames = {}
evtNames[WIFI_READY] = "ready"
evtNames[WIFI_SCAN_DONE] = "scan.done"
evtNames[WIFI_STA_START] = "sta.start"
evtNames[WIFI_STA_STOP] = "sta.stop"
evtNames[WIFI_STA_CONNECTED] = "sta.connected"
evtNames[WIFI_STA_DISCONNECTED] = "sta.disconnected"
evtNames[WIFI_AP_START] = "ap.start"
evtNames[WIFI_AP_STOP] = "ap.stop"
evtNames[WIFI_AP_STACONNECTED] = "ap.sta.connected"
evtNames[WIFI_AP_STADISCONNECTED] = "ap.sta.disconnected"
evtNames[WIFI_STA_CONNECTING] = "sta.connecting"
evtNames[102] = "sta.disconnecting"

// IP events
const STA_GOT_IP = 0
const STA_LOST_IP = 1
// const AP_STAIPASSIGNED = 2
// const GOT_IP6 = 3
// const ETH_GOT_IP = 4
// const PPP_GOT_IP = 5
// const PPP_LOST_IP = 6

let ipEvtNames = {}
ipEvtNames[STA_GOT_IP] = "ip.got"
ipEvtNames[STA_LOST_IP] = "ip.lost"

// power safe
const PS_NONE = 0
// const PS_MIN_MODEM = 1
// const PS_MAX_MODEM = 2 


wifi.registerEventHandle(function (eventType, eventId, data) {

  let eventName = null
  let eventArgv = []

  if (eventType == EVENT_WIFI) {
    eventName = evtNames[eventId]
    switch (eventId) {
      case WIFI_STA_DISCONNECTED:
        eventArgv.push(data)  // disconnect reason
      case WIFI_STA_CONNECTED:
        _connecting = false
        break
    }
  }
  else if (eventType == EVENT_IP) {
    eventName = ipEvtNames[eventId]
  }

  // console.log("..>",eventType,eventId,eventName)
  if (eventName) {
    wifi.emit(eventName, ...eventArgv)
  }

  if (eventType == EVENT_WIFI) {
    if (eventId == WIFI_STA_START || eventId == WIFI_AP_START) {
      if (contrastStatus(true)) {
        wifi.emit("start")
      }
    }
    else if (eventId == WIFI_STA_STOP || eventId == WIFI_AP_STOP) {
      if (contrastStatus(false)) {
        wifi.emit("stop")
      }
    }
  }
})

function contrastStatus(b) {
  let mode = wifi.mode()
  let status = 0
  if (b == wifi.staStarted()) {
    status |= MODE_STA
  }
  if (b == wifi.apStarted()) {
    status |= MODE_AP
  }
  return mode == status
}

function isReady() {
  return contrastStatus(true)
}
function start() {
  return new Promise(function (resolve) {
    wifi.peripheralStart()
    if (isReady()) {
      resolve()
    }
    else {
      wifi.once("start", resolve)
    }
  })
}
function stop() {
  return new Promise(function (resolve) {
    wifi.peripheralStop()
    if (contrastStatus(false)) {
      resolve()
    }
    else {
      wifi.once("stop", resolve)
    }
  })
}


let _connecting = false
function isConnecting() {
  return _connecting
}
function waitConnecting() {
  return new Promise(function (resolve) {
    wifi.race(["sta.disconnected", "sta.connected"], (evt, ...args) => {
      resolve(args[0] || 0)
    })
  })
}

async function connect(ssid, password, retry, retryDur) {

  if (_connecting) {
    await waitConnecting()
  }

  if (wifi.staConnected() && wifi.config(1)?.ssid == ssid) {
    return true
  }

  let mode = wifi.mode()
  wifi.setMode(mode | MODE_STA)

  await wifi.start()
  await wifi.disconnect()

  if (typeof ssid == 'object') {
    retry = ssid.retry
    retryDur = ssid.retryDur
    wifi.setStaConfig(ssid)
  }
  else {
    wifi.setStaConfig({ ssid, password })
  }

  retry = (parseInt(retry) || 0)
  if (retry <= 0)
    retry = 2
  retryDur = (parseInt(retryDur) || 2000)
  if (retryDur <= 0) {
    retryDur = 2000
  }

  while ((retry--) > 0) {
    // console.log("connect to ap:",ssid,'...')
    _connecting = true
    wifi.peripheralConnect()
    var res = await waitConnecting()
    _connecting = false
    // console.log("connect", res?"failed":"sucess", res)
    if (res != 0 && res != 202) {
      console.log("retry", retryDur, "ms later ...")
      await sleep(retryDur)
    }
    else {
      break;
    }
  }

  return 0 == res
}

async function disconnect() {
  if (timerDeamon >= 0) {
    clearTimeout(timerDeamon)
    timerDeamon = -1
  }
  return new Promise(function (resolve) {
    if (!wifi.staConnected()) {
      resolve()
      return
    }
    wifi.peripheralDisconnect()
    wifi.once("sta.disconnected", resolve)
  })
}

let timerDeamon = -1
function autoReconnect() {
  console.log("start wifi sta deamon")
  timerDeamon = setInterval(() => {
    if (wifi.staConnected() || !(wifi.mode() & MODE_STA) || _connecting || !wifi.staStarted()) {
      return
    }
    let staconf = wifi.config(1)
    if (!staconf.ssid) {
      return
    }
    wifi.connect(staconf.ssid, staconf.password)
  }, 10000)
}

function startAP(ssid, password) {
  return new Promise(resolve => {
    wifi.stopAP().then(ret => {
      if (!ret) {
        resolve(false)
        return
      }
      wifi.race(["ap.start", "ap.stop"], evt => resolve(evt == "ap.start"))
      wifi.setMode(wifi.mode() | MODE_AP)
      wifi.setAPConfig({ ssid, password })
      wifi.start()
    })
  })

}
function stopAP() {
  return new Promise(resolve => {
    if (!wifi.apStarted()) {
      resolve(true)
      return
    }
    wifi.race(["ap.start", "ap.stop"], evt => resolve(evt == "ap.stop"))
    wifi.setMode(wifi.mode() & (~MODE_AP))
  })
}

function status(netif) {
  if (!netif || netif == "apsta") {
    return {
      sta: wifi.status("sta"),
      ap: wifi.status("ap")
    }
  }
  else if (netif == 'sta') {
    let sta = wifi.getIpInfo(MODE_STA)
    sta.started = wifi.staStarted()
    sta.connected = wifi.staConnected()

    let config = wifi.config(MODE_STA)
    sta.ssid = config.ssid
    sta.password = config.password
    sta.authmode = config.authmode

    return sta
  }
  else if (netif == 'ap') {

    let ap = wifi.getIpInfo(MODE_AP)
    ap.started = wifi.apStarted()

    let config = wifi.config(MODE_AP)
    ap.ssid = config.ssid
    ap.password = config.password

    return ap
  }
  else {
    throw new Error("unknow netif: " + netif)
  }
}

function scan() {
  return new Promise(function (resolve, reject) {
    if (!(wifi.mode() & MODE_STA)) {
      wifi.setMode(wifi.mode() | MODE_STA)
    }
    if (!wifi.isScanning()) {
      if (!wifi.scanStart()) {
        reject()
        return
      }
    }
    wifi.once("scan.done", () => { resolve(wifi.scanRecords()) })
  })
}

function waitIP() {
  return new Promise(resolve => {
    let status = wifi.status("sta")
    console.log(status)
    if (status.connected && status.ip != "0.0.0.0") {
      resolve(status)
      return
    }
    let events = ['ip.got']
    if (wifi.isConnecting()) {
      events.push('sta.disconnected')
    }
    wifi.race(events, evt => {
      console.log(status)
      if (evt == 'sta.disconnected') {
        resolve(false)
      } else {
        resolve(wifi.status("sta"))
      }
    })
  })
}

[isReady, start, stop, connect, disconnect, isConnecting, startAP, stopAP, status, autoReconnect, scan, waitIP].map(func => exportValue(wifi, func.name, func))
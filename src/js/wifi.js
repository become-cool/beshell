import { exportValue } from "loader"
import * as wifi from "wifi"

function sleep(ms) {
  return new Promise(resolve => {
    setTimeout(resolve, ms)
  })
}

// wifi mode
// const MODE_NONE = 0
const MODE_STA = 1
const MODE_AP = 2
// const MODE_APSTA = 3


wifi.on("sta.connected",()=>{
  _connecting = false
})
wifi.on(["sta.start","ap.start"],()=>{
  if (contrastStatus(true)) {
    wifi.emit("start")
  }
})
wifi.on(["sta.stop","ap.stop"],()=>{
  if (contrastStatus(false)) {
    wifi.emit("stop")
  }
})
wifi.on('IP.GOT',()=>{
  // console.log("IP.GOT:", wifi.status("sta"))
  let {ip,netmask,gw} = wifi.status("sta")
  wifi.emit("ip.got", {ip,netmask,gw})
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
      resolve(evt == "sta.connected")
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
  await sleep(100)
  await wifi.disconnect(true)

  let res

  if (typeof ssid == 'object') {
    retry = ssid.retry
    retryDur = ssid.retryDur
    res = wifi.setStaConfig(ssid)
  }
  else {
    res = wifi.setStaConfig({ ssid, password })
  }
  if(res) {
    return false
  }

  retry = (parseInt(retry) || 0)
  let deamon = retry<0
  if (retry <= 0) {
    retry = 1
  }
  retryDur = (parseInt(retryDur) || 5000)
  if (retryDur <= 0) {
    retryDur = 5000
  }

  while ((retry--) > 0) {
    // console.log("connect to ap:",ssid,'...')
    _connecting = true
    res = wifi.peripheralConnect()

    res = await waitConnecting()
    _connecting = false
    // console.log("connect", res?"failed":"sucess", res)
    if (!res) {
      console.log("retry", retryDur, "ms later ...")
      await sleep(retryDur)
    }
    else {
      break;
    }
  }

  if(deamon) {
    startStaDeamon()
  }

  return res
}

async function disconnect(dontStopDeamon) {
  if(!dontStopDeamon) {
    stopStaDeamon()
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
function startStaDeamon(dur) {
  stopStaDeamon()
  if(!dur || isNaN(dur) ) {
    dur=10000
  }
  timerDeamon = setInterval(() => {
    if (wifi.staConnected() || !(wifi.mode() & 1) || wifi.isConnecting() || !wifi.staStarted()) {
      return
    }
    let staconf = wifi.config(1)
    if (!staconf.ssid) {
      return
    }
    wifi.connect(staconf.ssid, staconf.password,1,dur)
  }, dur)
}

function stopStaDeamon() {
  if (timerDeamon >= 0) {
    clearTimeout(timerDeamon)
    timerDeamon = -1
  }
}

function isStaDeamonRunning() {
  return timerDeamon >= 0
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
  return new Promise(function (resolve) {
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
    // console.log(status)
    if (status.connected && status.ip != "0.0.0.0") {
      resolve(status)
      return
    }
    let events = ['ip.got']
    if (wifi.isConnecting()) {
      events.push('sta.disconnected')
    }
    wifi.race(events, evt => {
      // console.log(status)
      if (evt == 'sta.disconnected') {
        resolve(false)
      } else {
        resolve(wifi.status("sta"))
      }
    })
  })
}

[isReady, start, stop, connect, disconnect, isConnecting, startAP, stopAP, status, startStaDeamon, stopStaDeamon, isStaDeamonRunning, scan, waitIP].map(func => exportValue(wifi, func.name, func))

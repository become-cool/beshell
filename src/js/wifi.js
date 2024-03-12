// wifi mode
// const MODE_NONE = 0
const MODE_STA = 1
const MODE_AP = 2
// const MODE_APSTA = 3


// event type
EVENT_WIFI = 1
EVENT_IP = 2 

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


const wifi = module.exports = new beapi.EventEmitter()
global.WiFi = wifi

wifi.mode = beapi.wifi.getMode
wifi.setMode = beapi.wifi.setMode

wifi.staStarted = beapi.wifi.staStarted
wifi.apStarted = beapi.wifi.apStarted

function contrastStatus(b) {
    let mode = wifi.mode()
    let status = 0
    if(b==beapi.wifi.staStarted()) {
        status|= MODE_STA
    }
    if(b==beapi.wifi.apStarted()) {
        status|= MODE_AP
    }
    return mode==status
}
wifi.isReady = function(){
    return contrastStatus(true)
}
wifi.start = function() {
    return new Promise(function(resolve){
        beapi.wifi.start()
        if(wifi.isReady()) {
            resolve()
        }
        else {
            wifi.once("start", resolve)
        }
    })
}
wifi.stop = function() {
    return new Promise(function(resolve){
        beapi.wifi.stop()
        if(contrastStatus(false)) {
            resolve()
        }
        else {
            wifi.once("stop", resolve)
        }
    })
}

let _connecting = false
wifi.isConnecting = function() {
    return _connecting
}
function waitConnecting() {
    return new Promise(function(resolve) {
        wifi.race(["sta.disconnected","sta.connected"], (evt, ...args)=>{
            resolve( args[0] || 0 )
        })
    })
}
/*
function(ssid,password,retry,retryDur)
or
function(options)
*/
wifi.connect = async function(ssid,password,retry,retryDur) {

    if(_connecting) {
        await waitConnecting()
    }

    if( beapi.wifi.staConnected() && beapi.wifi.getConfig(1)?.ssid==ssid ) {
        return true
    }

    let mode = wifi.mode()
    wifi.setMode(mode|MODE_STA)

    await wifi.start()
    await wifi.disconnect()
    
    if(typeof ssid=='object') {
        retry = ssid.retry
        retryDur = ssid.retryDur
        beapi.wifi.setStaConfig(ssid)
    }
    else {
        beapi.wifi.setStaConfig({ssid, password})
    }

    retry = (parseInt(retry) || 0) 
    if(retry<=0)
        retry = 2
    retryDur = (parseInt(retryDur) || 2000)
    if(retryDur<=0) {
        retryDur = 2000
    }

    while((retry--)>0) {
        // console.log("connect to ap:",ssid,'...')
        _connecting = true
        beapi.wifi.connect()
        var res = await waitConnecting()
        _connecting = false
        // console.log("connect", res?"failed":"sucess", res)
        if(res!=0 && res!=202) {
            console.log("retry", retryDur, "ms later ...")
            await sleep(retryDur)
        }
        else {
            break ;
        }
    }

    return 0==res
}

wifi.disconnect = async function() {
    if(timerDeamon>=0) {
        clearTimeout(timerDeamon)
        timerDeamon = -1
    }
    return new Promise(function(resolve) {
        if(!beapi.wifi.staConnected()) {
            resolve()
            return
        }
        beapi.wifi.disconnect()
        wifi.once("sta.disconnected", resolve)
    })
}

wifi.startAP = function(ssid, password, callback) {
    wifi.stopAP(ret=>{
        if(!ret) {
            callback(false)
            return
        }
        if(typeof callback=="function") {        
            wifi.race(["ap.start","ap.stop"], evt=>callback(evt=="ap.start"))
        }
        wifi.setMode( wifi.mode() | MODE_AP )
        beapi.wifi.setAPConfig({ssid, password})
        wifi.start()
    })
}
wifi.stopAP = function(callback) {
    if(!wifi.apStarted()) {
        callback && callback(true)
        return
    }
    if(typeof callback=="function") {        
        wifi.race(["ap.start","ap.stop"], evt=>callback(evt=="ap.stop"))
    }
    wifi.setMode( wifi.mode() & (~MODE_AP) )
}

wifi.status = function(netif) {
    if(!netif || netif=="apsta") {
        return {
            sta: wifi.status("sta"), 
            ap: wifi.status("ap")
        }
    }
    else if(netif=='sta') {
        let sta = beapi.wifi.getIpInfo(MODE_STA)
        sta.started = beapi.wifi.staStarted()
        sta.connected = beapi.wifi.staConnected()

        let config = beapi.wifi.getConfig(MODE_STA)
        sta.ssid = config.ssid
        sta.password = config.password
        sta.authmode = config.authmode
        
        return sta
    }
    else if(netif=='ap') {

        let ap = beapi.wifi.getIpInfo(MODE_AP)
        ap.started = beapi.wifi.apStarted()
    
        config = beapi.wifi.getConfig(MODE_AP)
        ap.ssid = config.ssid
        ap.password = config.password

        return ap
    }
    else {
        throw new Error("unknow netif: "+netif)
    }
}

function autoAP() {
    if( !(wifi.mode()&MODE_AP) ) {
        return
    }
    let config = beapi.wifi.getConfig(MODE_AP)
    if(!config||!config.ssid) {
        return
    }
    let res = config.ssid.match(/^ESP_([0-9A-F]{6})$/)
    if(!res) {
        return
    }
    let ssid = 'BECOME-'+res[1].slice(-4)
    beapi.wifi.setAPConfig({ssid, password: config.password})
}
let timerDeamon = -1
function deamon() {
    console.log("start wifi sta deamon")
    timerDeamon = setInterval(()=>{
        if( beapi.wifi.staConnected() || !(wifi.mode()&MODE_STA) || _connecting || !beapi.wifi.staStarted()){
            return
        }
        let staconf = beapi.wifi.getConfig(1)
        if( !staconf.ssid) {
            return
        }
        wifi.connect(staconf.ssid, staconf.password)
    },10000)
}
wifi.autostart = async function() {
    console.log("auto start wifi")
    autoAP()
    await wifi.start()
    
    let staconf = beapi.wifi.getConfig(1)
    if(wifi.mode()&MODE_STA && staconf.ssid) {
        await wifi.connect({
            ssid:staconf.ssid,
            password:staconf.password,
            authmode:staconf.authmode,
        }, 5, 2000)
        deamon()
    }
}

wifi.scan = function() {
    return new Promise(function(resolve, reject) {
        if( !(wifi.mode() & MODE_STA) ) {
            beapi.wifi.setMode(wifi.mode()|MODE_STA)
        }
        if(!beapi.wifi.isScanning()) {
            if(!beapi.wifi.scanStart()) {
                reject()
                return
            }
        }
        wifi.once("scan.done",resolve)
    })
}

beapi.wifi.registerEventHandle(function(eventType, eventId, data){

    let eventName = null
    let eventArgv = []

    if(eventType==EVENT_WIFI) {
        eventName = evtNames[eventId]
        switch(eventId) {
            case WIFI_STA_DISCONNECTED :
                eventArgv.push( data )  // disconnect reason
            case WIFI_STA_CONNECTED :
                _connecting = false
            break
        }
    }
    else if(eventType==EVENT_IP) {
        eventName = ipEvtNames[eventId]
    }

    // console.log(eventType,eventId,eventName)
    if(eventName) {
        wifi.emit(eventName, ...eventArgv)
    }

    if(eventType==EVENT_WIFI) {
        if(eventId==WIFI_STA_START || eventId==WIFI_AP_START) {
            if(contrastStatus(true)) {
                wifi.emit("start")
            }
        }
        else if(eventId==WIFI_STA_STOP || eventId==WIFI_AP_STOP) {
            if(contrastStatus(false)) {
                wifi.emit("stop")
            }
        }
    }
})

// 禁止 wifi 睡眠 (增加耗电)
if(beapi.wifi.hasInited()) {
    beapi.wifi.setPS(PS_NONE)
}
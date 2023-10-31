;(function(){

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
    
    const STA_GOT_IP = 0
    const STA_LOST_IP = 1
    
    let eventHandler = function(){} ;
    let mode = MODE_AP | MODE_STA
    let config = {
        1: {"ssid":"0^0=1","password":"12345678","channel":0,"scan_method":0,"listen_interval":3,"sort_method":0,"rm_enabled":0,"btm_enabled":0} ,
        2: {"ssid":"BECOME-B335","password":"","channel":5,"authmode":0,"ssid_hidden":0,"max_connection":4,"beacon_interval":100,"ftm_responder":0}
    }
    let ipInfo = {
        1: {"ip":"192.168.0.113","netmask":"255.255.255.0","gw":"192.168.0.1"} ,
        2: {"ip":"192.168.4.1","netmask":"255.255.255.0","gw":"192.168.4.1"} ,
    }
    
    let isStaConnected = false
    
    beapi.wifi = {
        start() {
            setTimeout(function(){
                eventHandler(EVENT_WIFI, WIFI_STA_START|WIFI_AP_START)
            }, 0)
        } ,
        stop() {
            setTimeout(function(){
                eventHandler(EVENT_WIFI, WIFI_STA_STOP|WIFI_AP_STOP)
            }, 0)
        } ,
        setPS() {} ,
        setMode(m) { mode = m } ,
        getMode() { return mode } ,
        
        setStaConfig(config) {} ,
        setAPConfig(config) {} ,
        getConfig(type) {
            return JSON.parse(JSON.stringify(config[type]))
        } ,
        allSta() {return []} ,
    
        connect() {
            eventHandler(EVENT_WIFI, WIFI_STA_CONNECTING)
            setTimeout(function(){
                isStaConnected = true
                eventHandler(EVENT_WIFI, WIFI_STA_CONNECTED)
                setTimeout(function(){
                    eventHandler(EVENT_IP, STA_GOT_IP)
                },1000)
            }, 3000)
        } ,
        disconnect() {
            setTimeout(function(){
                isStaConnected = false
                eventHandler(EVENT_WIFI, WIFI_STA_DISCONNECTED)
                setTimeout(function(){
                    eventHandler(EVENT_IP, STA_LOST_IP)
                },0)
            }, 0)
        } ,
    
        getIpInfo(type) { return JSON.parse(JSON.stringify(ipInfo[type])) } ,
        setHostname() {} ,
    
        registerEventHandle(callback) { eventHandler = callback } ,
    
        scanStart() {} ,
        scanStop() {} ,
        isScanning() { return false } ,
        scanRecords() {return []} ,
    
        staStarted() { return true } ,
        staConnected() { return isStaConnected } ,
        apStarted() { return true } ,
    
        hasInited() { return false } ,
    }
    
    beapi.spi = {
        setup: function() {} ,
        free: function() {} ,
        addDevice: function() {} ,
        removeDevice: function() {} ,
        send: function() {} ,
        sendU8: function() {} ,
        sendU16: function() {} ,
        sendU32: function() {} ,
        recvU8: function() {} ,
        recvU16: function() {} ,
        recvU32: function() {} ,
        transU8: function() {} ,
        transU16: function() {} ,
        transU32: function() {} ,
    }
    
    beapi.i2c = {
        setup: function() {} ,
        free: function() {} ,
        hasSetup: function() {} ,
        ping: function() {} ,
    
        send: function() {} ,
        write8: function() {} ,
        write16: function() {} ,
        write32: function() {} ,
        recv: function() {} ,
        recvByte: function() {} ,
        read: function() {} ,
        read8: function() {} ,
        read16: function() {} ,
        read32: function() {} ,
        readU8: function() {} ,
        readU16: function() {} ,
        readU32: function() {} ,   
    }
    
    
    beapi.i2s = {
        setup: function() {} ,  
        unsetup: function() {} ,  
        hasSetup: function() {} , 
    }

    beapi.driver = {
        mountSD(spi, cs, mount, khz) { beapi.fs.mkdirSync(mount) }
    }
    
    beapi.nvs = {
        erase: function() {} ,  

        readInt8: function() { return 0 } ,  
        readInt16: function() { return 0 } ,  
        readInt32: function() { return 0 } ,  
        readInt64: function() { return 0 } ,  
        readUint8: function() { return 0 } ,  
        readUint16: function() { return 0 } ,  
        readUint32: function() { return 0 } ,  
        readInt8: function() { return 0 } ,  

        writeInt8: function() {} ,  
        writeInt16: function() {} ,  
        writeInt32: function() {} ,  
        writeInt64: function() {} ,  
        writeUint8: function() {} ,  
        writeUint16: function() {} ,  
        writeUint32: function() {} ,  

        readString: function() { return '' } ,
        writeString: function() {} ,  

    }

})()
    
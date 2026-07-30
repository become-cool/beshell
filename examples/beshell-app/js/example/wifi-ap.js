import * as wifi from "wifi"

const ssid = "BeShell-APP"
const pwd = "12345678"

async function main(){

    console.log("Starting WiFi AP...")

    if (!wifi.isSoftAPSupported()) {
        console.log("SoftAP is not supported, please enable CONFIG_ESP_WIFI_SOFTAP_SUPPORT in sdkconfig")
        return
    }

    // start WiFi AP
    if( ! await wifi.startAP(ssid, pwd) ){
        console.log("Failed to start AP")
        return
    }
    console.log(`
  WiFi AP started as

    SSID: ${ssid}
    PWD:  ${pwd}

`)

    // evnets
    wifi.on("ap.sta.connected", ()=>{
        console.log("A client connected to AP")
    })
    wifi.on("ap.sta.disconnected", ()=>{
        console.log("a client disconnected to AP")
    })

}

main()
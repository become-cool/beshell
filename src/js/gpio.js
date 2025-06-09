import { exportValue } from "loader"
import * as gpio from "gpio"

const mapPinsToChannels = {}
const mapChannelsToPins = [[-1, -1, -1, -1, -1, -1, -1, -1],[-1, -1, -1, -1, -1, -1, -1, -1]]

function findUnusedChannel(mode) {
    if (!mapChannelsToPins[mode]) {
        throw new Error(`Mode ${mode} invalid`)
    }
    for (let i = 0; i < mapChannelsToPins[mode].length; i++) {
        if (mapChannelsToPins[mode][i] === -1) {
            return i
        }
    }
    throw new Error(`No unused channel found for mode ${mode}`)
}

function configPWM(pin,option){
    if(!option) {
        option = {}
    }
    const [mode, channel] = mapPinsToChannels[pin] || [0, -1]
    if(option.mode === undefined) {
        option.mode = mode
    }
    if(option.channel === undefined) {
        option.channel = (channel!=-1)? channel: findUnusedChannel(option.mode, pin)
    }

    gpio.apiConfigPWM(pin, option)

    mapPinsToChannels[pin] = [option.mode, option.channel]
    mapChannelsToPins[option.mode][option.channel] = pin
}

function writePWM(pin,value,update){
    if(!mapPinsToChannels[pin]) {
        throw new Error(`Pin ${pin} not configured for PWM`)
    }
    const [mode, channel] = mapPinsToChannels[pin]
    gpio.apiWritePWM(mode, channel, value, update)
}
function updatePWM(pin){
    if(!mapPinsToChannels[pin]) {
        throw new Error(`Pin ${pin} not configured for PWM`)
    }
    const [mode, channel] = mapPinsToChannels[pin]
    gpio.apiUpdatePWM(mode, channel)
    
}
function stopPWM(pin){
    if(!mapPinsToChannels[pin]) {
        throw new Error(`Pin ${pin} not configured for PWM`)
    }
    const [mode, channel] = mapPinsToChannels[pin]
    gpio.apiStopPWM(mode, channel)
    delete mapPinsToChannels[pin]
    mapChannelsToPins[mode][channel] = -1
}
function blink(gpio,time) {
    this.setMode(gpio,"output")
    return setInterval(() => {
        this.write(gpio,this.read(gpio)?0:1)
    }, time||1000)
}


const mapPinToWatchings = {}
gpio.apiSetHandler(function(pin,level){
    let edge = level? "rising" : "falling"
    if(!mapPinToWatchings[pin] || !mapPinToWatchings[pin][edge]) {
        return
    }
    for(let callback of mapPinToWatchings[pin][edge]) {
        callback(pin,level)
    }
    for(let callback of mapPinToWatchings[pin].both) {
        callback(pin,level)
    }
})

function watch(pin, edge, callback) {
    if(!["rising","falling","both"].includes(edge)){
        throw new Error(`Invalid edge type: ${edge}. Must be 'rising', 'falling', or 'both'.`)
    }
    if(typeof callback !== "function") {
        throw new Error("Arg callback must be a function")
    }
    if(!mapPinToWatchings[pin]) {
        mapPinToWatchings[pin] = {rising:[], falling:[], both:[]}
        gpio.apiAddISR(pin)
    }
    mapPinToWatchings[pin][edge].push(callback)
    console.log(mapPinToWatchings)
}

function unwatch(gpio, edge, callback) {
    if(!mapPinToWatchings[pin]) {
        return
    }
    if(!["rising","falling","both"].includes(edge)){
        throw new Error(`Invalid edge type: ${edge}. Must be 'rising', 'falling', or 'both'.`)
    }
    if(typeof callback !== "function") {
        throw new Error("Arg callback must be a function")
    }
    const index = mapPinToWatchings[pin][edge].indexOf(callback)
    if(index>-1) {
        mapPinToWatchings[pin][edge].splice(index, 1)
    }
    if(mapPinToWatchings[pin].rising.length === 0 && mapPinToWatchings[pin].falling.length === 0 && mapPinToWatchings[pin].both.length === 0) {
        delete mapPinToWatchings[pin]
        gpio.apiRemoveISR(pin)
    }
}

exportValue(gpio, "configPWM", configPWM)
exportValue(gpio, "writePWM", writePWM)
exportValue(gpio, "updatePWM", updatePWM)
exportValue(gpio, "stopPWM", stopPWM)
exportValue(gpio, "blink", blink)
exportValue(gpio, "watch", watch)
exportValue(gpio, "unwatch", unwatch)

import * as gpio from "gpio"
import * as sleep from "sleep"
import * as process from "process"

// deep sleep (深度睡眠) 示例
// deep sleep 唤醒后设备重启，无法恢复 JS 运行环境。
// 唤醒重启后再次运行本示例，可看到上次的唤醒原因。

console.log("reset reason:", process.resetReason())
console.log("wakeup cause:", sleep.wakeupCause())

// EXT1 唤醒时可查询具体是哪个引脚触发的
if(sleep.wakeupCause() == "ext1") {
    console.log("ext1 wakeup pins:", sleep.ext1WakeupPins())
}

// ---- 唤醒源设置（deep sleep 可用，可叠加）----

// 1. 定时唤醒 5 秒（也可以直接 sleep.deep(5000)）
sleep.enableTimerWakeup(5000)

// 2. EXT1: 多个 RTC GPIO 唤醒，GPIO 4 或 5 任一变高电平唤醒
//    （ESP32 上可用 "all-low"；ESP32-S3/C3 等新芯片用 "any-low"）
//    引脚配置为输入+下拉，避免浮空误触发，接 3.3V 可唤醒
if(sleep.isValidWakeupGPIO(4) && sleep.isValidWakeupGPIO(5)) {
    gpio.setMode(4, "input")
    gpio.pull(4, "down")
    gpio.setMode(5, "input")
    gpio.pull(5, "down")
    sleep.enableExt1Wakeup([4, 5], "any-high")
}

// 其他 deep sleep 可用的唤醒源（按需取消注释）：
//
// // EXT0: 单个 RTC GPIO 电平唤醒（ESP32-C3 等新芯片不支持）
// sleep.enableExt0Wakeup(4, 1)
//
// // deep sleep GPIO 唤醒（仅 ESP32-S3/C3 等新芯片支持）
// sleep.enableDeepGPIOWakeup([4], "low")
//
// // 触摸唤醒（需先初始化触摸驱动）
// sleep.enableTouchWakeup()
//
// // ULP 协处理器唤醒（需先加载并启动 ULP 程序）
// sleep.enableULPWakeup()

// 心跳定时器：每秒输出一次，表示未进入休眠
// 10 秒倒计时后进入 deep sleep，进入后设备断电重启，心跳停止
let tick = 0
let heartbeat = setInterval(() => {
    tick++
    if(tick >= 10) {
        
        console.log("Entering deep sleep, wake up after 5s or GPIO 4/5 high level ...")
        console.log("!!! device will reboot on wakeup, run this example again to see the wakeup cause")
        sleep.deep()   // 不会返回，唤醒后设备重启

        return
    }
    console.log(`running ${tick}, enter sleep after ${10-tick}s ...`)
}, 1000)

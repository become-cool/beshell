import * as gpio from "gpio"
import * as sleep from "sleep"

// light sleep (轻睡眠) 示例
// light sleep 期间 SRAM/PSRAM 数据保持，唤醒后从下一条语句继续运行，
// beshell / QuickJS / mongoose 等的状态完全保留

// ---- 唤醒源设置（只需设置一次，之后的每次休眠都会沿用）----

// 1. 定时唤醒 5 秒（也可以直接 sleep.light(5000)）
sleep.enableTimerWakeup(5000)

// 2. GPIO 唤醒（仅 light sleep 有效，任意 GPIO，不要求 RTC 功能）
//    引脚需配置为输入模式+上拉（output 模式无法检测外部电平），将 GPIO 4 接 GND 可唤醒
gpio.setMode(4, "input")
gpio.pull(4, "up")
sleep.enableLightGPIOWakeup(4, 0)

// 其他 light sleep 可用的唤醒源（按需取消注释）：
//
// // EXT0: 单个 RTC GPIO 电平唤醒（ESP32-C3 等新芯片不支持）
// sleep.enableExt0Wakeup(4, 1)
//
// // EXT1: 多个 RTC GPIO 唤醒，"any-high" 任一为高 / "all-low" 全为低（ESP32）/ "any-low" 任一为低（新芯片）
// sleep.enableExt1Wakeup([4, 5], "any-high")
//
// // UART 唤醒（需先用 uart 驱动设置 wakeup threshold，睡眠期间收到的数据可能丢失）
// sleep.enableLightUARTWakeup(0)
//
// // WiFi / 蓝牙唤醒（需先开启对应省电模式，WiFi 建议直接使用 sleep.modem()）
// sleep.enableLightWiFiWakeup()
// sleep.enableLightBTWakeup()
//
// // 触摸唤醒（需先初始化触摸驱动）
// sleep.enableTouchWakeup()
//
// // ULP 协处理器唤醒（需先加载并启动 ULP 程序）
// sleep.enableULPWakeup()
//
// // 禁用某个唤醒源
// sleep.disableWakeupSource("timer")


// 心跳定时器：每秒输出一次，表示未进入休眠
// 休眠期间心跳停止输出，唤醒后恢复
let tick = 0
setInterval(async () => {
    tick++
    if(tick < 10) {
        console.log(`running ${tick} ...`)
    } else {
        console.log("Entering light sleep, wake up after 5s or GPIO 4 low level ...")
        await delay(100)

        let source = sleep.light()   // 阻塞，直到唤醒（期间心跳停止输出）
        
        console.log("wakeup source:", source)
    }
}, 1000)


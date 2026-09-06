import * as gpio from "gpio"
import { tween, Timeline, morph } from "animator"

// PWM LED 亮度动画示例：演示 animator 模块的三种用法
// 将 LED 接到 GPIO 12（按实际硬件修改引脚）

const PIN = 12
const DUTY_MAX = 1023        // pwm 默认 10bit 分辨率，占空比 0~1023

gpio.pwm.config(PIN, { freq: 1000 })

// LED 亮度目标对象, Timeline 直接插值其 duty 属性
const led = { duty: 0 }
function applyDuty() {
    gpio.pwm.write(PIN, Math.round(led.duty))
}

// 1. morph: 一次性动画, 1.5 秒淡入到最亮, 完成后(on stop)启动呼吸循环
morph(0, DUTY_MAX, 1500, (v)=> {
    led.duty = Math.round(v)
    applyDuty()
}, "inOutQuad").on("stop", ()=> {

    // 2. Timeline 关键帧动画: 2 秒一轮呼吸, 循环播放
    //    0 帧自动捕获当前值(最亮) -> 第 50 帧渐暗到 0 -> 第 100 帧渐亮回最亮
    const breath = new Timeline(50, 100, true)   // 50fps x 100帧 = 2秒, repeat
    breath.set(50, led, "duty", 0, "inOutSine")
    breath.set(100, led, "duty", DUTY_MAX, "inOutSine")
    breath.on("change", applyDuty)    // 亮度实际变化时才写 PWM
    breath.play()
})

// 3. tween: 直接计算缓动值
console.log("tween.inQuad(0.5) =", tween.inQuad(0.5))
console.log("tween.outBounce(0.5) =", tween.outBounce(0.5))

console.log("PWM LED breathing on GPIO", PIN)
console.log("Enter `reboot` command to stop and back to main menu.")

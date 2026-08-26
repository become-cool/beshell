#include <BeShell.hpp>
#include <JSEngine.hpp>
#include "module/Sleep.hpp"
#include "esp_err.h"

#ifdef ESP_PLATFORM
#include "sdkconfig.h"
#include "esp_idf_version.h"
#include "esp_sleep.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_private/wifi.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "soc/soc_caps.h"
#if SOC_USB_SERIAL_JTAG_SUPPORTED && !SOC_USB_SERIAL_JTAG_SUPPORT_LIGHT_SLEEP
#include "hal/usb_serial_jtag_ll.h"
#include "driver/usb_serial_jtag.h"
#include "esp_private/rtc_clk.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// 置位期间, light sleep 保持 BBPLL 开启 (见下方 __wrap_rtc_clk_cpu_freq_set_xtal)
static volatile bool s_keep_bbpll_in_sleep = false ;
// (诊断) wrap 命中计数: 验证 --wrap 是否真的拦截了睡眠路径的时钟切换
static volatile int s_wrap_hit_keep = 0 ;
static volatile int s_wrap_hit_pass = 0 ;

extern "C" {

/**
 * light sleep 期间保持 BBPLL (USB PHY 48MHz 时钟源) 运行
 *
 * 背景: S3 的 USJ PHY 48MHz 时钟来自 BBPLL。IDF 进入 light sleep 时调用
 * rtc_clk_cpu_freq_set_xtal(_for_sleep) 把 CPU 切到 XTAL 并无条件关闭 BBPLL,
 * PHY 失去时钟后进入不可恢复状态 (唤醒后即使物理拔插也无法枚举)。
 *
 * 实测结论 (ESP32-S3, IDF v5.5): 以下保活措施可让 PHY 模拟层全程有电有时钟,
 * 但唤醒后 USB 串口依然无法恢复 (无法枚举/识别)。根因未完全查明, 疑似 IDF
 * 唤醒路径对运行中 BBPLL 的重新校准扰动 SIE 数字状态机 (尝试过跳过重校准的
 * 自定义时钟恢复, 会引入 LDO/dbias 欠压导致系统不稳定, 已放弃)。
 * 本问题暂时搁置, 相关 wrap 与唤醒恢复代码暂时保留, 见 JSDoc 中的警告说明。
 *
 * 代价: light sleep 期间电流略增 (BBPLL 维持供电)。
 *
 * 其他调用路径 (如开机 BBPLL 重校准 recalib_bbpll、deep sleep) 不受影响。
 */
void __real_rtc_clk_cpu_freq_set_xtal(void);
void __wrap_rtc_clk_cpu_freq_set_xtal(void) {
    if(s_keep_bbpll_in_sleep) {
        s_wrap_hit_keep++ ;
        rtc_clk_cpu_set_to_default_config() ;   // 只切 CPU 到 XTAL, 不关闭 BBPLL
    } else {
        s_wrap_hit_pass++ ;
        __real_rtc_clk_cpu_freq_set_xtal() ;
    }
}

// IDF 5.5+ 睡眠路径使用专用入口 rtc_clk_cpu_freq_set_xtal_for_sleep
// (其内部调用 rtc_clk_cpu_freq_set_xtal 可能被编译器内联, 需单独 wrap 确保拦截)
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,5,0)
void __real_rtc_clk_cpu_freq_set_xtal_for_sleep(void);
void __wrap_rtc_clk_cpu_freq_set_xtal_for_sleep(void) {
    if(s_keep_bbpll_in_sleep) {
        s_wrap_hit_keep++ ;
        rtc_clk_cpu_set_to_default_config() ;   // 同上方 wrap: 只切 XTAL, 保 BBPLL
    } else {
        s_wrap_hit_pass++ ;
        __real_rtc_clk_cpu_freq_set_xtal_for_sleep() ;
    }
}
#endif

/**
 * light sleep 期间保持 USJ pad 与模块时钟不关闭
 *
 * IDF 睡眠路径 (misc_modules_sleep_prepare) 会调用
 * sleep_console_usj_pad_backup_and_disable() 关闭 USJ pad 和模块时钟,
 * 唤醒时再恢复。实测即使 BBPLL 全程保活, 这个 pad/时钟的关断-重开循环依然会使
 * PHY/SIE 进入不可恢复状态 (唤醒后拔插都报“无法识别的设备”)。
 * 因此在 sleep 模块主动入睡期间拦截为无操作, USJ 全程保持原样运行。
 * 代价: 睡眠期间 USJ pad/时钟保持供电, 电流略增 (IDF 原本为防漏电而关闭)。
 */
void __real_sleep_console_usj_pad_backup_and_disable(void);
void __wrap_sleep_console_usj_pad_backup_and_disable(void) {
    if(!s_keep_bbpll_in_sleep) {
        __real_sleep_console_usj_pad_backup_and_disable() ;
    }
}
void __real_sleep_console_usj_pad_restore(void);
void __wrap_sleep_console_usj_pad_restore(void) {
    if(!s_keep_bbpll_in_sleep) {
        __real_sleep_console_usj_pad_restore() ;
    }
}

}

#if SOC_USB_SERIAL_JTAG_SUPPORTED && !SOC_USB_SERIAL_JTAG_SUPPORT_LIGHT_SLEEP
// (诊断) 输出 USJ 关键寄存器, 用于排查唤醒后 console 假死
// wrap=keep/pass: BBPLL 保活 wrap 的命中次数 (验证 --wrap 是否真正生效)
static void usj_diag_dump(const char * tag) {
    printf("[usj] %s conf0=%08lx int_ena=%04lx int_raw=%04lx int_st=%04lx ep1_conf=%02lx connected=%d wrap=%d/%d\n",
        tag,
        (unsigned long)USB_SERIAL_JTAG.conf0.val,
        (unsigned long)USB_SERIAL_JTAG.int_ena.val,
        (unsigned long)USB_SERIAL_JTAG.int_raw.val,
        (unsigned long)USB_SERIAL_JTAG.int_st.val,
        (unsigned long)USB_SERIAL_JTAG.ep1_conf.val,
        (int)usb_serial_jtag_is_connected(),
        (int)s_wrap_hit_keep, (int)s_wrap_hit_pass) ;
}

// (诊断) 探测 host 是否在线: 清 SOF 原始位后采样, host 在线时每 1ms 发送一次 SOF
// (连接监视器的 tick hook 每个 tick 也会清 SOF 位, 多次采样避免竞争)
static bool usj_probe_host_sof(int max_ms) {
    usb_serial_jtag_ll_clr_intsts_mask(USB_SERIAL_JTAG_INTR_SOF) ;
    for(int i=0; i<max_ms; i++) {
        esp_rom_delay_us(1000) ;
        if(usb_serial_jtag_ll_get_intraw_mask() & USB_SERIAL_JTAG_INTR_SOF) {
            return true ;
        }
    }
    return false ;
}
#endif

/**
 * esp_light_sleep_start 的包装: 睡眠期间保持 BBPLL (仅 USJ 不支持 light sleep 保活的芯片)
 */
static esp_err_t light_sleep_start_keep_usj() {
    usj_diag_dump("pre-sleep") ;
    printf("[usj] pre-sleep sof=%d\n", (int)usj_probe_host_sof(50)) ;
    s_keep_bbpll_in_sleep = true ;
    esp_err_t err = esp_light_sleep_start() ;
    s_keep_bbpll_in_sleep = false ;
    return err ;
}
#endif

// ESP_SLEEP_POWER_DOWN_CPU 宏由 IDF 5.2+ 提供, 5.1 需要自行判断
#ifndef ESP_SLEEP_POWER_DOWN_CPU
    #if defined(CONFIG_PM_POWER_DOWN_CPU_IN_LIGHT_SLEEP) && SOC_PM_SUPPORT_CPU_PD
        #define ESP_SLEEP_POWER_DOWN_CPU 1
    #else
        #define ESP_SLEEP_POWER_DOWN_CPU 0
    #endif
#endif

// light sleep 时 CPU 下电需要先初始化 CPU retention, 否则无法唤醒
#if ESP_SLEEP_POWER_DOWN_CPU
static bool cpu_retention_inited = false ;
static esp_err_t sleep_cpu_retention_init_once() {
    if(!cpu_retention_inited) {
        esp_err_t err = esp_sleep_cpu_retention_init() ;
        if(err!=ESP_OK) {
            return err ;
        }
        cpu_retention_inited = true ;
    }
    return ESP_OK ;
}
#endif

/**
 * 唤醒后恢复 USB CDC console (仅 S3/C3 等 USJ 不支持 light sleep 保活的芯片需要)
 *
 * 设备端保活 (--wrap, 见上方): light sleep 期间 BBPLL (USB PHY 48MHz 时钟源) 不关断,
 * USJ pad 与模块时钟不关闭, PHY 模拟层全程有电有时钟。
 * 但唤醒路径的 BBPLL 重新校准会对运行中的 PLL 重写反馈分频参数, 输出时钟扰动可能
 * 使 SIE 数字状态机进入错误状态; 且睡眠期间设备无法应答 host, host 侧可能已闩锁
 * 错误状态。因此唤醒后:
 *   1. 完整复位 USJ 外设 (SIE/FIFO/端点回到上电默认)
 *   2. 模拟一次拔插 (D+/D- 拉低 100ms), 给 host 干净的拔出->插入事件强制重新枚举
 *
 * > 实测结论: 此恢复流程在 ESP32-S3 上仍无法恢复 USB 串口 (枚举失败),
 * > 问题暂时搁置, 代码保留。详见 light() 的 JSDoc 警告。
 * > 注意: 拔插模拟后 COM 口会消失再出现, host 端软件 (如 beconsole) 需要能检测
 * > 设备重新上线并重新打开串口, 否则终端依然收不到输出。
 */
static void usj_console_recover_after_wakeup() {
#if SOC_USB_SERIAL_JTAG_SUPPORTED && !SOC_USB_SERIAL_JTAG_SUPPORT_LIGHT_SLEEP
    // __DECLARE_RCC_ATOMIC_ENV 是 USJ LL 宏约定由调用方声明的局部变量名
    // (参考 esp_hw_support/sleep_console.c 的用法)
    int __DECLARE_RCC_ATOMIC_ENV __attribute__ ((unused));

    if(!usb_serial_jtag_ll_module_is_enabled() || !usb_serial_jtag_ll_phy_is_pad_enabled()) {
        return ;    // USJ 未使用, 无需恢复
    }
    usj_diag_dump("wake") ;
    printf("[usj] wake sof=%d\n", (int)usj_probe_host_sof(50)) ;

    // 完整复位 USJ 外设: SIE 状态机/FIFO/端点状态回到上电默认值,
    // 消除睡眠与唤醒时钟切换 (BBPLL 重新校准) 期间可能积累的错误状态。
    // (PHY 模拟层由 BBPLL 保活护住, 数字状态机由此复位)
    usb_serial_jtag_ll_enable_bus_clock(true) ;
    usb_serial_jtag_ll_reset_register() ;
    esp_rom_delay_us(1000) ;
    usb_serial_jtag_ll_phy_enable_pad(true) ;
    // 恢复驱动安装的 RX 中断 (复位会清空中断使能; TX 中断由驱动写数据时自行打开)
    usb_serial_jtag_ll_ena_intr_mask(USB_SERIAL_JTAG_INTR_SERIAL_OUT_RECV_PKT) ;
    usj_diag_dump("reset") ;

    // 模拟拔插: 给 host 一个干净的拔出->插入事件, 强制其重新枚举
    // (同时清除 host 在设备睡眠期间枚举失败可能闩锁的错误状态)
    usb_serial_jtag_pull_override_vals_t detach = {
        .dp_pu = 0, .dm_pu = 0, .dp_pd = 1, .dm_pd = 1,
    } ;
    // 清除 BUS_RESET/SOF 原始中断状态, 用于检测 host 是否发起重新枚举
    usb_serial_jtag_ll_clr_intsts_mask(USB_SERIAL_JTAG_INTR_BUS_RESET | USB_SERIAL_JTAG_INTR_SOF) ;
    usb_serial_jtag_ll_phy_enable_pull_override(&detach) ;
    vTaskDelay(pdMS_TO_TICKS(100)) ;    // 保持断开 100ms, 确保 host 完成设备移除检测
    usb_serial_jtag_ll_phy_disable_pull_override() ;

    // 等待 host 重新枚举 (最多 3 秒): host 枚举设备时会先发 BUS_RESET,
    // 枚举成功后持续发送 SOF (连接监视器据此更新 is_connected)
    // 注意必须使用 vTaskDelay 而不是忙等, 否则长时间占用 CPU 触发任务看门狗
    bool got_reset = false, connected = false ;
    for(int i=0; i<30; i++) {
        vTaskDelay(pdMS_TO_TICKS(100)) ;
        if(usb_serial_jtag_ll_get_intraw_mask() & USB_SERIAL_JTAG_INTR_BUS_RESET) {
            got_reset = true ;
        }
        if(usb_serial_jtag_is_connected()) {
            connected = true ;
            break ;
        }
    }
    printf("[usj] reattach: bus_reset=%d connected=%d\n", (int)got_reset, (int)connected) ;
    usj_diag_dump("reattach") ;
#endif
}
#endif

namespace be {

#ifdef ESP_PLATFORM

    /**
     * 唤醒源枚举值转为字符串
     */
    static const char * wakeupCauseString(esp_sleep_source_t cause) {
        switch(cause) {
            case ESP_SLEEP_WAKEUP_EXT0:     return "ext0" ;
            case ESP_SLEEP_WAKEUP_EXT1:     return "ext1" ;
            case ESP_SLEEP_WAKEUP_TIMER:    return "timer" ;
            case ESP_SLEEP_WAKEUP_TOUCHPAD: return "touch" ;
            case ESP_SLEEP_WAKEUP_ULP:      return "ulp" ;
            case ESP_SLEEP_WAKEUP_GPIO:     return "gpio" ;
            case ESP_SLEEP_WAKEUP_UART:     return "uart" ;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,2,0) && ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,3,0)
            case ESP_SLEEP_WAKEUP_UART1:    return "uart1" ;
            case ESP_SLEEP_WAKEUP_UART2:    return "uart2" ;
#endif
            case ESP_SLEEP_WAKEUP_WIFI:     return "wifi" ;
            case ESP_SLEEP_WAKEUP_COCPU:    return "cocpu" ;
            case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG: return "cocpu-trap" ;
            case ESP_SLEEP_WAKEUP_BT:       return "bt" ;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,4,0)
            case ESP_SLEEP_WAKEUP_VAD:      return "vad" ;
#endif
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,5,0)
            case ESP_SLEEP_WAKEUP_VBAT_UNDER_VOLT: return "vbat-under-volt" ;
#endif
            default:                        return "undefined" ;
        }
    }

    /**
     * 字符串转唤醒源枚举值, 用于 disableWakeupSource()
     */
    static bool wakeupSourceFromString(const char * str, esp_sleep_source_t * out) {
        if(!strcmp(str,"all"))    { *out = ESP_SLEEP_WAKEUP_ALL ;      return true ; }
        if(!strcmp(str,"ext0"))   { *out = ESP_SLEEP_WAKEUP_EXT0 ;     return true ; }
        if(!strcmp(str,"ext1"))   { *out = ESP_SLEEP_WAKEUP_EXT1 ;     return true ; }
        if(!strcmp(str,"timer"))  { *out = ESP_SLEEP_WAKEUP_TIMER ;    return true ; }
        if(!strcmp(str,"touch"))  { *out = ESP_SLEEP_WAKEUP_TOUCHPAD ; return true ; }
        if(!strcmp(str,"ulp"))    { *out = ESP_SLEEP_WAKEUP_ULP ;      return true ; }
        if(!strcmp(str,"gpio"))   { *out = ESP_SLEEP_WAKEUP_GPIO ;     return true ; }
        if(!strcmp(str,"uart"))   { *out = ESP_SLEEP_WAKEUP_UART ;     return true ; }
        if(!strcmp(str,"wifi"))   { *out = ESP_SLEEP_WAKEUP_WIFI ;     return true ; }
        if(!strcmp(str,"bt"))     { *out = ESP_SLEEP_WAKEUP_BT ;       return true ; }
        return false ;
    }

    /**
     * 从 GPIO 编号数组参数构建 64 位掩码
     */
    static bool gpioMaskFromJSArray(JSContext *ctx, JSValueConst jspins, uint64_t * mask) {
        *mask = 0 ;
        if(!JS_IsArray(ctx, jspins)) {
            JS_ThrowTypeError(ctx, "pins must be an array of GPIO numbers") ;
            return false ;
        }
        JSValue jslen = JS_GetPropertyStr(ctx, jspins, "length") ;
        uint32_t len = 0 ;
        JS_ToUint32(ctx, &len, jslen) ;
        JS_FreeValue(ctx, jslen) ;
        for(uint32_t i=0; i<len; i++) {
            JSValue jspin = JS_GetPropertyUint32(ctx, jspins, i) ;
            uint32_t pin = 0 ;
            JS_ToUint32(ctx, &pin, jspin) ;
            JS_FreeValue(ctx, jspin) ;
            if(pin>63) {
                JS_ThrowRangeError(ctx, "invalid GPIO pin: %d", (int)pin) ;
                return false ;
            }
            *mask |= (1ULL<<pin) ;
        }
        return true ;
    }

    // ---- modem 睡眠的 WiFi 唤醒原因检测 ------------------------------------

    // WiFi 数据帧/管理事件导致的唤醒原因, nullptr 表示无(纯 beacon 监听)
    static volatile const char * modem_wake_reason = nullptr ;
    // STA netif 句柄, 用于链式转发 RX 数据包
    static esp_netif_t * sta_netif = nullptr ;
    static bool rxcb_registered = false ;

    /**
     * WiFi RX 回调 (替换式注册, 必须链式转发到 netif 保持正常数据通路)
     * beacon 帧由 MAC 层内部处理, 不会进入此回调, 所以进入这里即为下行数据
     */
    static esp_err_t sleep_wifi_rxcb(void *buffer, uint16_t len, void *eb) {
        modem_wake_reason = "wifi.data" ;
        if(sta_netif) {
            return esp_netif_receive(sta_netif, buffer, len, eb) ;
        }
        return ESP_FAIL ;
    }

    /**
     * WiFi 管理事件钩子: 断连 / beacon 超时
     */
    static void sleep_wifi_event_handler(void* arg, esp_event_base_t base, int32_t id, void* data) {
        (void)arg; (void)base; (void)data;
        if(id==WIFI_EVENT_STA_DISCONNECTED) {
            modem_wake_reason = "wifi.disconnected" ;
        }
        else if(id==WIFI_EVENT_STA_BEACON_TIMEOUT) {
            modem_wake_reason = "wifi.beacon-timeout" ;
        }
    }

#endif // ESP_PLATFORM


    /**
     * 系统睡眠模块, 提供 ESP32 的 light/deep/modem 睡眠以及各种唤醒源的设置
     *
     * 三种睡眠方式在调用逻辑上有本质区别:
     * - `light()` / `modem()`: **同步阻塞函数**。调用后当前 JS 线程挂起
     *   (事件循环停止, 定时器/网络回调等不再执行), 直到唤醒源触发才返回,
     *   返回值为唤醒源字符串, 之后的代码继续执行, JS 运行环境完全保留
     * - `deep()`: **调用后不会返回**。唤醒时设备整体重启, 脚本从头重新执行,
     *   JS 运行环境无法恢复, 唤醒原因需在重启后用 `wakeupCause()` 查询
     *
     * 唤醒源字符串: `"timer"` `"ext0"` `"ext1"` `"gpio"` `"uart"` `"touch"` `"wifi.data"` `"wifi.disconnected"` `"wifi.beacon-timeout"` 等
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // 1. 最简单的 light sleep: 睡 5 秒 (同步阻塞, 5 秒后才执行下一行)
     * let source = sleep.light(5000)
     * console.log("wakeup source:", source)   // "timer"
     *
     * // 2. light sleep + 按钮唤醒: 定时与 GPIO 唤醒源叠加, 任一触发即返回
     * import * as gpio from "gpio"
     * gpio.setMode(4, "input")
     * gpio.pull(4, "up")
     * sleep.enableLightGPIOWakeup(4, 0)        // GPIO 4 接地时唤醒
     * source = sleep.light(60000)              // 最多睡 60 秒, 按钮可提前唤醒
     * console.log("wakeup source:", source)   // "gpio" 或 "timer"
     *
     * // 3. deep sleep: 睡 1 小时后重启 (此行之后的代码不会执行)
     * //    需要跨睡眠保存的数据先写入 NVS
     * sleep.deep(3600_000)
     * console.log("这行永远不会执行")
     *
     * // 4. deep sleep + 按钮唤醒, 重启后判断唤醒原因
     * //    (脚本开头)
     * if(sleep.wakeupCause() == "ext0") {
     *     console.log("被按钮唤醒")
     * }
     * //    ... 正常工作逻辑 ...
     * sleep.enableExt0Wakeup(33, 1)            // GPIO 33 高电平唤醒
     * sleep.deep()                             // 不返回, 唤醒即重启
     *
     * // 5. modem sleep: 保持 WiFi 连接的睡眠, 有下行数据时唤醒
     * //    (要求 WiFi STA 已连接)
     * source = sleep.modem(10000)              // 有下行数据或 10 秒超时返回
     * console.log("wakeup source:", source)   // "wifi.data" / "timer" / ...
     * ```
     *
     * @module sleep
     */
    Sleep::Sleep(JSContext *ctx, const char *name, uint8_t flagGlobal)
            : NativeModule(ctx, name, flagGlobal) {

        exportFunction("light", light);
        exportFunction("deep", deep);
        exportFunction("modem", modem);
        exportFunction("enableTimerWakeup", enableTimerWakeup);
        exportFunction("enableExt0Wakeup", enableExt0Wakeup);
        exportFunction("enableExt1Wakeup", enableExt1Wakeup);
        exportFunction("enableLightGPIOWakeup", enableLightGPIOWakeup);
        exportFunction("enableLightUARTWakeup", enableLightUARTWakeup);
        exportFunction("enableLightWiFiWakeup", enableLightWiFiWakeup);
        exportFunction("enableLightBTWakeup", enableLightBTWakeup);
        exportFunction("enableDeepGPIOWakeup", enableDeepGPIOWakeup);
        exportFunction("enableTouchWakeup", enableTouchWakeup);
        exportFunction("enableULPWakeup", enableULPWakeup);
        exportFunction("disableWakeupSource", disableWakeupSource);
        exportFunction("wakeupCause", wakeupCause);
        exportFunction("ext1WakeupPins", ext1WakeupPins);
        exportFunction("isValidWakeupGPIO", isValidWakeupGPIO);
    }

    /**
     * 进入 light sleep (轻睡眠)
     *
     * **这是一个同步阻塞函数**: 调用后 CPU 暂停执行, 当前 JS 线程挂起,
     * 事件循环停止运转 (setTimeout/Promise 回调/网络事件等都不会执行)。
     * 唤醒源触发后函数才返回唤醒源字符串, 其后的语句继续执行;
     * SRAM/PSRAM 数据保持, beshell/QuickJS 以及 mongoose 等网络库的状态完全保留。
     * 请勿用 `sleep.light(ms).then(...)` 之类的异步方式调用。
     *
     * 传入 `ms` 参数等价于先调用 `enableTimerWakeup(ms)` 再进入睡眠;
     * 不传参数则一直睡眠, 直到已配置的唤醒源触发。
     *
     * > 警告: 不传 `ms` 且没有配置任何唤醒源时, 将永远无法唤醒。
     *
     * > 警告: 在 ESP32-S3/C3/C6 等芯片上, 若使用原生 USB (USB Serial/JTAG)
     * > 作为 console, 当前版本的 light() 返回后 **USB REPL 将失效**: 唤醒后
     * > USB 串口无法恢复 (重连串口、物理拔插均可能无法识别设备, 通常需重启
     * > 设备才能恢复)。这是 ESP-IDF 官方已知限制: 该类芯片的 USJ 外设不支持
     * > light sleep 保活 (见 ESP-IDF 文档 Light-sleep Limitations); 本模块曾
     * > 尝试 BBPLL 保活 + 唤醒后复位 USJ/模拟拔插强制重枚举, 均无法稳定恢复。
     * > **如需保留 USB REPL 请改用 deep()** (唤醒即重启, USB 正常重新枚举)。
     * > UART0 串口 console 不受此问题影响。
     *
     * 唤醒后将唤醒源字符串作为返回值。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // 定时 5 秒唤醒
     * let source = sleep.light(5000)
     * console.log("wakeup source:", source)
     *
     * // 由 GPIO 唤醒
     * sleep.enableLightGPIOWakeup(4, 0)
     * source = sleep.light()
     * console.log("wakeup source:", source)  // "gpio"
     * ```
     *
     * @module sleep
     * @function light
     * @param ms:number= 定时唤醒时间, 单位毫秒, 可选
     * @return string 唤醒源, 如 "timer"|"ext0"|"ext1"|"gpio"|"uart"|"touch"|"wifi" 等
     */
    JSValue Sleep::light(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        if(argc>0 && !JS_IsNone(argv[0])) {
            int64_t ms = 0 ;
            if(JS_ToInt64(ctx, &ms, argv[0])!=0) {
                JSTHROW("Invalid param type")
            }
            if(ms<=0) {
                JSTHROW("ms must > 0")
            }
            CALL_IDF_API( esp_sleep_enable_timer_wakeup((uint64_t)ms*1000), "esp_sleep_enable_timer_wakeup" )
        }
#if ESP_SLEEP_POWER_DOWN_CPU
        esp_err_t rerr = sleep_cpu_retention_init_once() ;
        if(rerr!=ESP_OK) {
            JSTHROW("esp_sleep_cpu_retention_init() failed, err: %d", rerr)
        }
#endif
#if SOC_USB_SERIAL_JTAG_SUPPORTED && !SOC_USB_SERIAL_JTAG_SUPPORT_LIGHT_SLEEP
        esp_err_t err = light_sleep_start_keep_usj() ;
#else
        esp_err_t err = esp_light_sleep_start() ;
#endif
        if(err==ESP_ERR_SLEEP_REJECT) {
            JSTHROW("light sleep rejected: a wakeup source is already triggered (check wakeup GPIO level)")
        }
        if(err!=ESP_OK) {
            JSTHROW("esp_light_sleep_start() failed, err: %d", err)
        }
        usj_console_recover_after_wakeup() ;
        const char * source = wakeupCauseString(esp_sleep_get_wakeup_cause()) ;
        return JS_NewString(ctx, source) ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 进入 deep sleep (深度睡眠)
     *
     * **此函数不会返回, 调用即意味着重启**: CPU、SRAM、PSRAM 全部断电,
     * 仅 RTC 域保持供电。唤醒后设备从入口函数整体重启, 脚本从头开始执行,
     * **JS 运行环境无法恢复**, 写在 `deep()` 调用之后的代码永远不会执行到。
     * 重启后可用 `wakeupCause()` 或 `process.resetReason()` 查询唤醒原因。
     *
     * 需要跨睡眠保存的数据请使用 NVS (nvs 模块)。
     *
     * 传入 `ms` 参数等价于先调用 `enableTimerWakeup(ms)` 再进入睡眠;
     * 不传参数则一直睡眠, 直到已配置的唤醒源触发或外部复位。
     *
     * > 警告: 不传 `ms` 且没有配置任何唤醒源时, 只能断电或复位唤醒。
     *
     * > 提示: deep sleep 唤醒即重启, USB (USB Serial/JTAG) console 会随启动
     * > 正常重新枚举, **不存在 light() 的 USB REPL 失效问题**。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // 1 分钟后唤醒(重启)
     * sleep.deep(60000)
     *
     * // 按钮唤醒 (ext0, GPIO 33 高电平)
     * sleep.enableExt0Wakeup(33, 1)
     * sleep.deep()
     * ```
     *
     * @module sleep
     * @function deep
     * @param ms:number= 定时唤醒时间, 单位毫秒, 可选
     * @return undefined 此函数不会返回
     */
    JSValue Sleep::deep(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        if(argc>0 && !JS_IsNone(argv[0])) {
            int64_t ms = 0 ;
            if(JS_ToInt64(ctx, &ms, argv[0])!=0) {
                JSTHROW("Invalid param type")
            }
            if(ms<=0) {
                JSTHROW("ms must > 0")
            }
            CALL_IDF_API( esp_sleep_enable_timer_wakeup((uint64_t)ms*1000), "esp_sleep_enable_timer_wakeup" )
        }
        esp_deep_sleep_start() ;  // 不会返回, 唤醒后重启
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 进入 modem sleep (保持 WiFi 连接的睡眠)
     *
     * 开启 WiFi 省电模式(PS Mode)并使能 WiFi 唤醒后进入 light sleep。
     * 睡眠期间 WiFi 连接保持, AP 会为设备缓存下行数据。
     *
     * **唤醒过滤**: 仅为监听 beacon 的协议性唤醒(TBTT/DTIM)不会返回,
     * 函数内部会继续睡眠, JS 无感知。只有以下情况会返回:
     * - `"wifi.data"`: 收到下行数据帧
     * - `"wifi.disconnected"`: 被 AP 断开连接
     * - `"wifi.beacon-timeout"`: beacon 超时
     * - 其他唤醒源字符串: 如 `"timer"` `"gpio"` 等(睡前配置的其他唤醒源)
     *
     * > 注意:
     * > 1. 调用前 WiFi STA 必须已连接。
     * > 2. 睡眠期间 JS 事件循环不运转, mongoose 等网络库的回调在唤醒返回后才会执行。
     * > 3. 强制 light sleep 不与 WiFi 驱动协调, 单次睡眠时间不宜过长, 否则可能掉线。
     * > 4. `wifi.data` 检测依赖 STA netif "WIFI_STA_DEF", 非默认 netif 时该检测不可用。
     * > 5. 返回后 WiFi 省电模式保持开启, 可用 `wifi.setPS(0)` 关闭。
     * > 6. 若使用原生 USB (USB Serial/JTAG) console, modem() 返回后 USB REPL
     * >    同样会失效 (与 light() 相同), 详见 `light()` 的警告说明。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // 保持 WiFi 连接睡眠, 有下行数据或 10 秒超时唤醒
     * let source = sleep.modem(10000)
     * console.log("wakeup source:", source)  // "wifi.data" / "timer" / ...
     *
     * // 最大省电模式, 只在 DTIM 周期监听
     * source = sleep.modem(10000, "max")
     * ```
     *
     * @module sleep
     * @function modem
     * @param ms:number= 最长睡眠时间, 单位毫秒, 可选
     * @param mode:string="min" WiFi 省电模式, "min"(每个 beacon 周期监听) 或 "max"(按 DTIM/listen interval 监听, 更省电但延迟更高)
     * @return string 唤醒原因, 如 "wifi.data"|"wifi.disconnected"|"wifi.beacon-timeout"|"timer"|"gpio" 等
     * @throws WiFi 未连接或芯片不支持 WiFi 唤醒时抛出异常
     */
    JSValue Sleep::modem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        int64_t ms = -1 ;
        if(argc>0 && !JS_IsNone(argv[0])) {
            if(JS_ToInt64(ctx, &ms, argv[0])!=0) {
                JSTHROW("Invalid param type")
            }
            if(ms<=0) {
                JSTHROW("ms must > 0")
            }
        }

        wifi_ps_type_t ps_mode = WIFI_PS_MIN_MODEM ;
        if(argc>1 && !JS_IsNone(argv[1])) {
            ARGV_TO_CSTRING_E(1, modestr, "mode must be a string")
            if(!strcmp(modestr,"min")) {
                ps_mode = WIFI_PS_MIN_MODEM ;
            }
            else if(!strcmp(modestr,"max")) {
                ps_mode = WIFI_PS_MAX_MODEM ;
            }
            else {
                JS_FreeCString(ctx, modestr) ;
                JSTHROW("mode must be \"min\" or \"max\"")
            }
            JS_FreeCString(ctx, modestr) ;
        }

        // 开启 WiFi 省电模式 (要求 WiFi STA 已连接)
        esp_err_t err = esp_wifi_set_ps(ps_mode) ;
        if(err!=ESP_OK) {
            JSTHROW("failed to enable WiFi power save (WiFi STA connected?), err: %d", err)
        }

        // 注册 RX 回调检测下行数据 (替换式注册, 仅注册一次并永久链式转发到 netif)
        if(!rxcb_registered) {
            sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF") ;
            if(sta_netif && esp_wifi_internal_reg_rxcb(WIFI_IF_STA, sleep_wifi_rxcb)==ESP_OK) {
                rxcb_registered = true ;
            }
        }

        // 注册 WiFi 管理事件钩子
        esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, sleep_wifi_event_handler, nullptr) ;
        esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_BEACON_TIMEOUT, sleep_wifi_event_handler, nullptr) ;

        modem_wake_reason = nullptr ;

        if(ms>0) {
            CALL_IDF_API( esp_sleep_enable_timer_wakeup((uint64_t)ms*1000), "esp_sleep_enable_timer_wakeup" )
        }
        CALL_IDF_API( esp_sleep_enable_wifi_wakeup(), "esp_sleep_enable_wifi_wakeup" )

#if ESP_SLEEP_POWER_DOWN_CPU
        esp_err_t rerr = sleep_cpu_retention_init_once() ;
        if(rerr!=ESP_OK) {
            JSTHROW("esp_sleep_cpu_retention_init() failed, err: %d", rerr)
        }
#endif

        const char * reason = nullptr ;
        while(true) {
#if SOC_USB_SERIAL_JTAG_SUPPORTED && !SOC_USB_SERIAL_JTAG_SUPPORT_LIGHT_SLEEP
            light_sleep_start_keep_usj() ;
#else
            esp_light_sleep_start() ;
#endif
            if(modem_wake_reason) {                 // 下行数据或管理帧事件
                reason = (const char *)modem_wake_reason ;
                modem_wake_reason = nullptr ;
                break ;
            }
            esp_sleep_source_t cause = esp_sleep_get_wakeup_cause() ;
            if(cause!=ESP_SLEEP_WAKEUP_WIFI) {      // timer/gpio 等其他唤醒源
                reason = wakeupCauseString(cause) ;
                break ;
            }
            // 仅为 TBTT/DTIM 监听 beacon, 不出循环, 继续睡
        }

        esp_sleep_disable_wifi_wakeup() ;
        esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, sleep_wifi_event_handler) ;
        esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_BEACON_TIMEOUT, sleep_wifi_event_handler) ;

        usj_console_recover_after_wakeup() ;
        return JS_NewString(ctx, reason) ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置定时唤醒 (light sleep 和 deep sleep 均有效)
     *
     * 多个唤醒源可以叠加, 任一触发即唤醒。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * sleep.enableTimerWakeup(10000)  // 10 秒后唤醒
     * sleep.light()
     * ```
     *
     * @module sleep
     * @function enableTimerWakeup
     * @param ms:number 定时唤醒时间, 单位毫秒
     * @return undefined
     */
    JSValue Sleep::enableTimerWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CHECK_ARGC(1)
        ARGV_TO_INT64(0, ms)
        if(ms<=0) {
            JSTHROW("ms must > 0")
        }
        CALL_IDF_API( esp_sleep_enable_timer_wakeup((uint64_t)ms*1000), "esp_sleep_enable_timer_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 EXT0 唤醒: 单个 RTC GPIO 电平唤醒 (light sleep 和 deep sleep 均有效)
     *
     * 只能监听一个引脚, 引脚必须具有 RTC 功能 (可用 `isValidWakeupGPIO()` 检查)。
     * 当引脚电平**等于**指定电平时唤醒。
     *
     * > 注意: 部分新芯片 (如 ESP32-C3) 不支持 EXT0, 此时抛出异常,
     * > 可改用 `enableExt1Wakeup()` 或 `enableDeepGPIOWakeup()`。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // GPIO 33 高电平唤醒
     * sleep.enableExt0Wakeup(33, 1)
     * sleep.deep()
     * ```
     *
     * @module sleep
     * @function enableExt0Wakeup
     * @param pin:number RTC GPIO 引脚号
     * @param level:number 触发电平, 0=低电平唤醒, 1=高电平唤醒
     * @return undefined
     * @throws 芯片不支持 EXT0 或引脚不是 RTC GPIO 时抛出异常
     */
    JSValue Sleep::enableExt0Wakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_PM_SUPPORT_EXT0_WAKEUP
        CHECK_ARGC(2)
        ARGV_TO_GPIO(0, pin)
        ARGV_TO_UINT8(1, level)
        if(level>1) {
            JSTHROW("level must be 0 or 1")
        }
        CALL_IDF_API( esp_sleep_enable_ext0_wakeup(pin, level), "esp_sleep_enable_ext0_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("ext0 wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 EXT1 唤醒: 多个 RTC GPIO 电平唤醒 (light sleep 和 deep sleep 均有效)
     *
     * 可同时监听多个引脚, 引脚必须具有 RTC 功能。
     * 唤醒后可用 `ext1WakeupPins()` 查询是哪个引脚触发的。
     *
     * 触发模式:
     * - `"any-high"`: 任一引脚为高电平时唤醒
     * - `"all-low"`: 所有引脚都为低电平时唤醒 (仅 ESP32; 其他芯片上等同于 "any-low")
     * - `"any-low"`: 任一引脚为低电平时唤醒 (ESP32 不支持)
     *
     * > 注意: RTC 外设断电时内部上下拉失效, 需要外部上下拉电阻,
     * > 或配置 RTC 外设电源保持。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // GPIO 32 或 33 任一变为高电平时唤醒
     * sleep.enableExt1Wakeup([32, 33], "any-high")
     * sleep.deep()
     * // (重启后)
     * console.log(sleep.ext1WakeupPins())  // 例如 [33]
     * ```
     *
     * @module sleep
     * @function enableExt1Wakeup
     * @param pins:Array\<number\> RTC GPIO 引脚号数组
     * @param mode:string 触发模式, "any-high"|"all-low"|"any-low"
     * @return undefined
     * @throws 芯片不支持 EXT1 或引脚不是 RTC GPIO 时抛出异常
     */
    JSValue Sleep::enableExt1Wakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_PM_SUPPORT_EXT1_WAKEUP
        CHECK_ARGC(2)
        uint64_t mask = 0 ;
        if(!gpioMaskFromJSArray(ctx, argv[0], &mask)) {
            return JS_EXCEPTION ;
        }
        ARGV_TO_CSTRING_E(1, modestr, "mode must be a string")
        esp_sleep_ext1_wakeup_mode_t mode ;
        if(!strcmp(modestr,"any-high")) {
            mode = ESP_EXT1_WAKEUP_ANY_HIGH ;
        }
#if CONFIG_IDF_TARGET_ESP32
        else if(!strcmp(modestr,"all-low")) {
            mode = ESP_EXT1_WAKEUP_ALL_LOW ;
        }
#else
        else if(!strcmp(modestr,"all-low")||!strcmp(modestr,"any-low")) {
            mode = ESP_EXT1_WAKEUP_ANY_LOW ;
        }
#endif
        else {
            JS_FreeCString(ctx, modestr) ;
#if CONFIG_IDF_TARGET_ESP32
            JSTHROW("mode must be \"any-high\" or \"all-low\"")
#else
            JSTHROW("mode must be \"any-high\" or \"any-low\"")
#endif
        }
        JS_FreeCString(ctx, modestr) ;
        CALL_IDF_API( esp_sleep_enable_ext1_wakeup(mask, mode), "esp_sleep_enable_ext1_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("ext1 wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 GPIO 唤醒, **仅对 light sleep 有效**
     *
     * 与 EXT0/EXT1 不同, 此方式可以使用任意 GPIO (不要求 RTC 功能)。
     *
     * > 注意: 此函数不修改引脚配置。引脚必须事先用 gpio 模块配置为
     * > **输入模式**并设置好上下拉 (低电平唤醒建议上拉, 高电平唤醒建议下拉),
     * > 否则浮空电平可能导致无法唤醒或进不了睡眠 (ESP_ERR_SLEEP_REJECT)。
     *
     * 示例：
     * ```javascript
     * import * as gpio from "gpio"
     * import * as sleep from "sleep"
     *
     * // GPIO 4 低电平唤醒: 输入模式 + 上拉, 接地即可唤醒
     * gpio.setMode(4, "input")
     * gpio.pull(4, "up")
     * sleep.enableLightGPIOWakeup(4, 0)
     * sleep.light()
     * ```
     *
     * @module sleep
     * @function enableLightGPIOWakeup
     * @param pin:number GPIO 引脚号
     * @param level:number 触发电平, 0=低电平唤醒, 1=高电平唤醒
     * @return undefined
     */
    JSValue Sleep::enableLightGPIOWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CHECK_ARGC(2)
        ARGV_TO_GPIO(0, pin)
        ARGV_TO_UINT8(1, level)
        CALL_IDF_API( gpio_wakeup_enable(pin, level? GPIO_INTR_HIGH_LEVEL: GPIO_INTR_LOW_LEVEL), "gpio_wakeup_enable" )
        CALL_IDF_API( esp_sleep_enable_gpio_wakeup(), "esp_sleep_enable_gpio_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 UART 唤醒, **仅对 light sleep 有效**
     *
     * 当 UART RX 引脚上检测到一定数量的正跳变沿时唤醒。
     * 跳变沿数量阈值需事先用 UART 驱动的 `uart_set_wakeup_threshold` 设置。
     *
     * > 注意: 唤醒需要一定时间, 睡眠期间收到的数据可能丢失,
     * > 适合"唤醒后再通信"的场景。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * sleep.enableLightUARTWakeup(1)  // UART1 唤醒
     * sleep.light()
     * ```
     *
     * @module sleep
     * @function enableLightUARTWakeup
     * @param uartNum:number UART 端口号
     * @return undefined
     * @throws 该 UART 不支持唤醒时抛出异常
     */
    JSValue Sleep::enableLightUARTWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, uartNum)
        CALL_IDF_API( esp_sleep_enable_uart_wakeup(uartNum), "esp_sleep_enable_uart_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 WiFi 唤醒, **仅对 light sleep 有效**
     *
     * 使能后 WiFi MAC 可以在收到数据或需要处理时唤醒 CPU。
     * 通常不需要直接调用, 使用 `modem()` 更方便 (自动设置省电模式和唤醒过滤)。
     *
     * > 注意: 需要先开启 WiFi 省电模式, 如 `wifi.setPS(1)`。
     *
     * @module sleep
     * @function enableLightWiFiWakeup
     * @return undefined
     * @throws 芯片不支持 WiFi 唤醒时抛出异常
     */
    JSValue Sleep::enableLightWiFiWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CALL_IDF_API( esp_sleep_enable_wifi_wakeup(), "esp_sleep_enable_wifi_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置蓝牙唤醒, **仅对 light sleep 有效**
     *
     * @module sleep
     * @function enableLightBTWakeup
     * @return undefined
     * @throws 芯片不支持蓝牙唤醒时抛出异常
     */
    JSValue Sleep::enableLightBTWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CALL_IDF_API( esp_sleep_enable_bt_wakeup(), "esp_sleep_enable_bt_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 GPIO 唤醒, **仅对 deep sleep 有效** (仅 ESP32-C3/S3 等新芯片支持)
     *
     * 老芯片 (ESP32) 请使用 `enableExt0Wakeup()` / `enableExt1Wakeup()`。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * // GPIO 2 低电平唤醒 (ESP32-S3)
     * sleep.enableDeepGPIOWakeup([2], "low")
     * sleep.deep()
     * ```
     *
     * @module sleep
     * @function enableDeepGPIOWakeup
     * @param pins:Array\<number\> RTC GPIO 引脚号数组
     * @param mode:string 触发电平, "low"|"high"
     * @return undefined
     * @throws 芯片不支持 deep sleep GPIO 唤醒时抛出异常
     */
    JSValue Sleep::enableDeepGPIOWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
        CHECK_ARGC(2)
        uint64_t mask = 0 ;
        if(!gpioMaskFromJSArray(ctx, argv[0], &mask)) {
            return JS_EXCEPTION ;
        }
        ARGV_TO_CSTRING_E(1, modestr, "mode must be a string")
        esp_deepsleep_gpio_wake_up_mode_t mode ;
        if(!strcmp(modestr,"low")) {
            mode = ESP_GPIO_WAKEUP_GPIO_LOW ;
        }
        else if(!strcmp(modestr,"high")) {
            mode = ESP_GPIO_WAKEUP_GPIO_HIGH ;
        }
        else {
            JS_FreeCString(ctx, modestr) ;
            JSTHROW("mode must be \"low\" or \"high\"")
        }
        JS_FreeCString(ctx, modestr) ;
        CALL_IDF_API( esp_deep_sleep_enable_gpio_wakeup(mask, mode), "esp_deep_sleep_enable_gpio_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("deep sleep gpio wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置触摸唤醒 (light sleep 和 deep sleep 均有效)
     *
     * > 注意: 触摸通道需要先通过触摸驱动初始化和配置,
     * > 此函数仅使能触摸唤醒源。
     *
     * @module sleep
     * @function enableTouchWakeup
     * @return undefined
     * @throws 芯片不支持触摸唤醒时抛出异常
     */
    JSValue Sleep::enableTouchWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_TOUCH_SENSOR_SUPPORTED
        CALL_IDF_API( esp_sleep_enable_touchpad_wakeup(), "esp_sleep_enable_touchpad_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("touch wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 设置 ULP 协处理器唤醒 (light sleep 和 deep sleep 均有效)
     *
     * > 注意: 需要事先加载并启动 ULP 程序, 此函数仅使能 ULP 唤醒源。
     *
     * @module sleep
     * @function enableULPWakeup
     * @return undefined
     * @throws 芯片不支持 ULP 时抛出异常
     */
    JSValue Sleep::enableULPWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_ULP_SUPPORTED
        CALL_IDF_API( esp_sleep_enable_ulp_wakeup(), "esp_sleep_enable_ulp_wakeup" )
        return JS_UNDEFINED ;
#else
        JSTHROW("ulp wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 禁用指定的唤醒源
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * sleep.disableWakeupSource("timer")  // 禁用定时唤醒
     * sleep.disableWakeupSource("all")    // 禁用所有唤醒源
     * ```
     *
     * @module sleep
     * @function disableWakeupSource
     * @param source:string 唤醒源, "all"|"ext0"|"ext1"|"timer"|"touch"|"ulp"|"gpio"|"uart"|"wifi"|"bt"
     * @return undefined
     * @throws 未知的唤醒源或该唤醒源未使能时抛出异常
     */
    JSValue Sleep::disableWakeupSource(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CHECK_ARGC(1)
        ARGV_TO_CSTRING_E(0, src, "source must be a string")
        esp_sleep_source_t source ;
        if(!wakeupSourceFromString(src, &source)) {
            JS_FreeCString(ctx, src) ;
            JSTHROW("unknown wakeup source")
        }
        JS_FreeCString(ctx, src) ;
        CALL_IDF_API( esp_sleep_disable_wakeup_source(source), "esp_sleep_disable_wakeup_source" )
        return JS_UNDEFINED ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 查询上次睡眠的唤醒原因
     *
     * deep sleep 唤醒后系统重启, 可在启动时用此函数查询本次启动是否由睡眠唤醒导致。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * const cause = sleep.wakeupCause()
     * if(cause == "timer") {
     *     console.log("被定时器唤醒")
     * } else if(cause == "undefined") {
     *     console.log("正常上电启动")
     * }
     * ```
     *
     * @module sleep
     * @function wakeupCause
     * @return string 唤醒原因, "undefined"|"ext0"|"ext1"|"timer"|"touch"|"ulp"|"gpio"|"uart"|"wifi"|"bt"|"cocpu" 等
     */
    JSValue Sleep::wakeupCause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        return JS_NewString(ctx, wakeupCauseString(esp_sleep_get_wakeup_cause())) ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 查询触发 EXT1 唤醒的引脚
     *
     * 如果上次唤醒不是 EXT1 导致的, 返回空数组。
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * if(sleep.wakeupCause() == "ext1") {
     *     console.log("wakeup pins:", sleep.ext1WakeupPins())  // 例如 [33]
     * }
     * ```
     *
     * @module sleep
     * @function ext1WakeupPins
     * @return Array\<number\> 触发唤醒的 GPIO 引脚号数组
     * @throws 芯片不支持 EXT1 时抛出异常
     */
    JSValue Sleep::ext1WakeupPins(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
#if SOC_PM_SUPPORT_EXT1_WAKEUP
        uint64_t mask = esp_sleep_get_ext1_wakeup_status() ;
        JSValue arr = JS_NewArray(ctx) ;
        int i = 0 ;
        for(int pin=0; pin<64; pin++) {
            if(mask & (1ULL<<pin)) {
                JS_SetPropertyUint32(ctx, arr, i++, JS_NewUint32(ctx, pin)) ;
            }
        }
        return arr ;
#else
        JSTHROW("ext1 wakeup is not supported on this chip")
#endif
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

    /**
     * 检查引脚是否可作为唤醒源 (是否具有 RTC 功能)
     *
     * 示例：
     * ```javascript
     * import * as sleep from "sleep"
     *
     * if(sleep.isValidWakeupGPIO(33)) {
     *     sleep.enableExt0Wakeup(33, 1)
     * }
     * ```
     *
     * @module sleep
     * @function isValidWakeupGPIO
     * @param pin:number GPIO 引脚号
     * @return boolean 是否可用于唤醒
     */
    JSValue Sleep::isValidWakeupGPIO(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)
        return JS_NewBool(ctx, esp_sleep_is_valid_wakeup_gpio(pin)) ;
#else
        JSTHROW("sleep module is not supported on this platform")
#endif
    }

}

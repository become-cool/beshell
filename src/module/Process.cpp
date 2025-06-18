#include <BeShell.hpp>
#include <JSEngine.hpp>
#include "module/Process.hpp"
#include "esp_err.h"
#include "../js/process.c"

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "soc/soc.h"
#include "esp_efuse.h"
#include "esp_mac.h"
#include "driver/temperature_sensor.h"
#endif

using namespace std;

namespace be {


    /**
     * 系统功能模块, 命名为 `process` 是为了和 nodejs 的习惯一致
     * 
     * @module process
     * @global process
     */
    Process::Process(JSContext *ctx, const char *name, uint8_t flagGlobal)
            : NativeModule(ctx, name, flagGlobal) {

        exportFunction("reboot", reboot);
        exportFunction("sleep", sleep);
        exportFunction("usage", usage);
        exportFunction("setTime", setTime);
        exportFunction("setTimezoneOffset", setTimezone);
        exportFunction("getRunningTime", getRunningTime);
        exportFunction("getChipTemperature", getChipTemperature);
        exportFunction("readEFUSE", readEFUSE);
        exportFunction("readMac", readMac);
        exportFunction("gc", gc);
        exportFunction("ref", ref);
        exportFunction("getTimerCount", JSTimer::getTimerCount);
        exportFunction("getTimerCallback", JSTimer::getTimerCallback);
        exportFunction("resetReason", resetReason);
        exportFunction("_top", top);
        exportFunction("tasks", tasks);
        exportFunction("cpuCount", cpuCount);

        // JS_ComputeMemoryUsage
        exportName("versions") ;
        exportName("platform") ;
        exportName("argv") ;
        exportName("top") ;
    }

    void Process::exports(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        JSValue versions = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, versions, "beshell", JS_NewString(ctx, BESHELL_VERSION));
#ifdef ESP_PLATFORM
        JS_SetPropertyStr(ctx, versions, "esp-idf", JS_NewString(ctx, ESPIDF_VERSION));
#ifdef LVGL_VERSION_MAJOR
        char lvgl_ver[16] ;
        sprintf(lvgl_ver, "%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH) ;
        JS_SetPropertyStr(ctx, versions, "lvgl", JS_NewString(ctx, lvgl_ver));
#endif
#endif
        JS_SetPropertyStr(ctx, versions, "quickjs", JS_NewString(ctx, QUICKJS_VERSION));

        char buff[32] ;
        sprintf(buff, "%s %s", __DATE__, __TIME__) ;
        JS_SetPropertyStr(ctx, versions, "build", JS_NewString(ctx, buff));

        exportValue("argv", JS_NewArray(ctx)) ;
        exportValue("versions", versions) ;

#ifdef ESP_PLATFORM
        exportValue("platform", JS_NewString(ctx, "esp32")) ;
#endif
#ifdef LiNUX_PLATFORM
        exportValue("platform", JS_NewString(ctx, "linux")) ;
#endif

    JSValue DEF_JS_FUNC(jsTop, R"(
function top(detail=false) {
    let list = {}
    if(detail) {
        const cpuCount = process.cpuCount()
        for(let i=0;i<cpuCount;i++) {
            for(let taskName of process.tasks(i)) {
                list[taskName] = {
                    cpu: i
                }
            }
        }
    }
    function parseTop(output) {
        let lines = output.split('\n')
        let result = []
        for(let line of lines) {
            line = line.trim()
            if(!line.trim()) continue
            result.push(line.split(/\t+/).map(item=>item.trim()))
        }
        return result
    }
    for(let [taskName, runCnt, usageRate] of parseTop(process._top())){
        if(!list[taskName]){
            list[taskName] = {}
        }
        list[taskName].runCnt = parseInt(runCnt)||0
        list[taskName].usageRate = parseInt(usageRate)||0
    }
    if(detail) {
        for(let [taskName, status, priority, stack, taskId] of parseTop(process._top(true))){
            if(!list[taskName]){
                list[taskName] = {}
            }
            list[taskName].status = status
            list[taskName].priority = priority
            list[taskName].stack = stack
            list[taskName].taskId = taskId
        }
    }

    return list
}
    )", "process.js", {
            JSEngine::fromJSContext(ctx)->dumpError() ;
            return ;
        })
        exportValue("top", jsTop) ;
    }

    /**
     * 重启系统
     * 
     * @function reboot
     * @return undefined
     */
    JSValue Process::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        esp_restart();
#endif
        return JS_NewBool(ctx, true);
    }

    /**
     * 读硬件地址
     * 
     * @function readMac
     * @param phy:"wifi"|"wifi.ap"|"wifi.softap"|"ble"|"eth"|"base"|"efuse"|"efuse.factory"|"efuse.customer"|"efuse.ext"="wifi" 要读取的硬件类型
     * @param format:bool=false 返回格式，false返回ArrayBuffer，true返回十六进制字符串
     * @return ArrayBuffer|string 硬件地址
     */
    JSValue Process::readMac(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        uint8_t mac[8] ;

        esp_mac_type_t mac_type = ESP_MAC_WIFI_STA;
        if(argc>0) {
            const char *mac_type_str;
            mac_type_str = JS_ToCString(ctx, argv[0]);
            if (!mac_type_str) {
                return JS_ThrowTypeError(ctx, "Invalid argument");
            }
            if (strcmp(mac_type_str, "wifi") == 0) {
                mac_type = ESP_MAC_WIFI_STA;
            } else if (strcmp(mac_type_str, "wifi.sta") == 0) {
                mac_type = ESP_MAC_WIFI_STA;
            } else if (strcmp(mac_type_str, "wifi.ap") == 0) {
                mac_type = ESP_MAC_WIFI_SOFTAP;
            } else if (strcmp(mac_type_str, "bt") == 0) {
                mac_type = ESP_MAC_BT;
            } else if (strcmp(mac_type_str, "eth") == 0) {
                mac_type = ESP_MAC_ETH;
            } else if (strcmp(mac_type_str, "base") == 0) {
                mac_type = ESP_MAC_BASE;
            } else if (strcmp(mac_type_str, "efuse") == 0) {
                mac_type = ESP_MAC_EFUSE_FACTORY;
            } else if (strcmp(mac_type_str, "efuse.factory") == 0) {
                mac_type = ESP_MAC_EFUSE_FACTORY;
            } else if (strcmp(mac_type_str, "efuse.customer") == 0) {
                mac_type = ESP_MAC_EFUSE_CUSTOM;
            } else if (strcmp(mac_type_str, "efuse.ext") == 0) {
                mac_type = ESP_MAC_EFUSE_EXT;
            } else {
                JS_FreeCString(ctx, mac_type_str);
                return JS_ThrowRangeError(ctx, "Unknown MAC type");
            }
            JS_FreeCString(ctx, mac_type_str);
        }

        bool format = false ;
        if(argc>1) {
            format = JS_ToBool(ctx, argv[1]) ;
        }

#ifdef ESP_PLATFORM
        if(esp_read_mac(mac,mac_type)!=ESP_OK) {
            JSTHROW("failed to read wifi mac addr")
        }
#endif

        if(!format) {
            return JS_NewArrayBufferCopy(ctx, mac, sizeof(mac)) ;
        }
        else {
            char macstr[24] ;
            sprintf(macstr,"%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            return JS_NewString(ctx, macstr) ;
        }
    }

    /**
     * 从一次性写入区(只读区) efuse 读取数据
     * 
     * ESP32 一共8个32位寄存器共自定义使用
     * 
     * @function readEFUSE
     * @param field:number 要读取的 efuse 块和位，范围 0-7
     * @return number 读取到的 efuse 数据 (32位整数)
     */
    JSValue Process::readEFUSE(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(1)
        ARGV_TO_UINT8(0, field)
        if(field>7) {
            JSTHROW("metadata field must 0-7")
        }
        
#ifdef ESP_PLATFORM
        int value = esp_efuse_read_reg((esp_efuse_block_t)3, field) ;
#else
        int value = 0 ;
#endif

        return JS_NewInt32(ctx, value) ;
    }


    /**
     * 内存使用情况
     * 
     * 返回对象的格式：
     * 
     * ```javascript
     * {
     *   "heap": {
     *     "total": number,
     *     "used": number,
     *     "free": number
     *   },
     *   "psram": {
     *     "total": number,
     *     "used": number,
     *     "free": number
     *   },
     *   "dma": {
     *     "total": number,
     *     "used": number,
     *     "free": number
     *   }
     * }
     * ```
     * 
     * 其中，`total` 表示总内存大小，`used` 表示已用内存大小，`free` 表示剩余内存大小。
     * 
     * psram 表示伪静态内存，通常是外挂的IC存储器；
     * 
     * @function usage
     * @return object 
     */
    JSValue Process::usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        (void) argc ;
        (void) argv ;

        int heap_total = 0 ;
        int heap_used = 0 ;
        int heap_free = 0 ;
        int psram_total = 0 ;
        int psram_used = 0 ;
        int psram_free = 0 ;
        int dma_total = 0 ;
        int dma_used = 0 ;
        int dma_free = 0 ;

#ifdef ESP_PLATFORM
        multi_heap_info_t info;

        heap_caps_get_info(&info, MALLOC_CAP_DMA);
        dma_total = info.total_free_bytes + info.total_allocated_bytes;
        dma_used = info.total_allocated_bytes;
        dma_free = info.total_free_bytes;

        heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
        heap_total = info.total_free_bytes + info.total_allocated_bytes;
        heap_used = info.total_allocated_bytes;
        heap_free = info.total_free_bytes;

        heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
        psram_total = info.total_free_bytes + info.total_allocated_bytes;
        psram_used = info.total_allocated_bytes;
        psram_free = info.total_free_bytes;
#endif

        JSValue obj = JS_NewObject(ctx) ;

        JSValue objHeap = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "heap", objHeap) ;
        JS_SetPropertyStr(ctx, objHeap, "total", JS_NewUint32(ctx, heap_total)) ;
        JS_SetPropertyStr(ctx, objHeap, "used", JS_NewUint32(ctx, heap_used)) ;
        JS_SetPropertyStr(ctx, objHeap, "free", JS_NewUint32(ctx, heap_free)) ;

        JSValue objPsram = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "psram", objPsram) ;
        JS_SetPropertyStr(ctx, objPsram, "total", JS_NewUint32(ctx, psram_total)) ;
        JS_SetPropertyStr(ctx, objPsram, "used", JS_NewUint32(ctx, psram_used)) ;
        JS_SetPropertyStr(ctx, objPsram, "free", JS_NewUint32(ctx, psram_free)) ;

        JSValue objDma = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "dma", objDma) ;
        JS_SetPropertyStr(ctx, objDma, "total", JS_NewUint32(ctx, dma_total)) ;
        JS_SetPropertyStr(ctx, objDma, "used", JS_NewUint32(ctx, dma_used)) ;
        JS_SetPropertyStr(ctx, objDma, "free", JS_NewUint32(ctx, dma_free)) ;

        return obj ;
    }
    
    /**
     * 延时函数
     * 
     * @function sleep
     * @param ms:number 延时时间，单位为毫秒
     * @return undefined
     */
    JSValue Process::sleep(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT32(0, ms)
        vTaskDelay(ms/portTICK_PERIOD_MS) ;
        return JS_UNDEFINED ;
    }

    /**
     * 设置系统时间
     * 
     * @function setTime
     * @param ms:number UNIX时间戳，单位为毫秒
     * @return undefined
     */
    JSValue Process::setTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, ms)
        
        JSEngine::fromJSContext(ctx)->timer.updateTime(ms) ;

        struct timeval tv;
        tv.tv_sec = ms/1000;  // epoch time (seconds)
        tv.tv_usec = (ms%1000)*1000;    // microseconds

        //printf("%lu.%lu\n",tv.tv_sec,tv.tv_usec);

#ifdef ESP_PLATFORM
        settimeofday(&tv, NULL);
#endif

        return JS_UNDEFINED ;
    }


    /**
     * 设置系统时区时间偏置量(分钟)
     * 
     * ```javascript
     * process.setTimezoneOffset(8*60); // 设置时区为东八区
     * ```
     * 
     * @function setTimezoneOffset
     * @param minute:number 时间偏置分钟
     * @return undefined
     */
    JSValue Process::setTimezone(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        ARGV_TO_INT32(0, minute)

        setTimezoneOffset(minute) ;

        return JS_UNDEFINED ;
    }

    JSValue Process::gc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        JS_RunGC(JS_GetRuntime(ctx)) ;
        return JS_UNDEFINED ;
    }

    static JSValue watchingVar = JS_NULL ;
    JSValue Process::ref(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(argc>0) {
            if( !JS_IsObject(argv[0]) ) {
                return JS_NewUint32(ctx, 0) ;
            }
            watchingVar = argv[0] ;
            return JS_NewUint32(ctx, ((JSRefCountHeader *)JS_VALUE_GET_PTR(watchingVar))->ref_count - 1)  ;
        }
        else {
            if( !JS_IsObject(watchingVar) ) {
                return JS_NewUint32(ctx, 0) ;
            }
            return JS_NewUint32(ctx, ((JSRefCountHeader *)JS_VALUE_GET_PTR(watchingVar))->ref_count)  ;
        }
    }

    /**
     * 获取系统运行时间（毫秒）
     * 
     * @function getRunningTime
     * @return number
     */
    static JSValue Process::getRunningTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int64_t boot_time_us = esp_timer_get_time();
        return JS_NewUint32(ctx, boot_time_us / 1000); // 返回毫秒
    }

    /**
     * 芯片温度
     * 
     * @function getChipTemperature
     * @return number
     */
    JSValue Process::getChipTemperature(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        esp_err_t err ;
        static bool temp_sensor_initialized = false;
        static temperature_sensor_handle_t temp_handle = NULL;
        if(!temp_sensor_initialized) {
            temperature_sensor_config_t temp_sensor = {
                .range_min = -10,  // 最小测量范围
                .range_max = 80    // 最大测量范围
            };
            // 初始化并启用传感器
            err = temperature_sensor_install(&temp_sensor, &temp_handle) ;
            if(err != ESP_OK) {
                JSTHROW("Failed to install temperature sensor, error = %d", err);
            }
            temp_sensor_initialized = true ;
        }
        err = temperature_sensor_enable(temp_handle);
        if(err != ESP_OK) {
            temperature_sensor_uninstall(temp_handle);
            JSTHROW("Failed to enable temperature sensor, error = %d", err);
        }
        
        float temp_c;
        err = temperature_sensor_get_celsius(temp_handle, &temp_c);
        if(err != ESP_OK) {
            temperature_sensor_disable(temp_handle);
            temperature_sensor_uninstall(temp_handle);
            JSTHROW("Failed to get temperature, error = %d", err);
        }
        
        // 禁用以省电
        temperature_sensor_disable(temp_handle);

        return JS_NewFloat64(ctx, temp_c); // 返回摄氏度温度值
    }

    
    /**
     * 芯片复位原因
     * 
     * @function resetReason
     * @return string
     */
    JSValue Process::resetReason(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        // 获取重启原因
        esp_reset_reason_t reason = esp_reset_reason();

        char * reason_str = nullptr;

        // 打印重启原因
        switch (reason) {
            case ESP_RST_POWERON:
                reason_str = "power-on";
                break;
            case ESP_RST_EXT:
                reason_str = "external-pin";
                break;
            case ESP_RST_SW:
                reason_str = "soft-reset";
                break;
            case ESP_RST_PANIC:
                reason_str = "panic";
                break;
            case ESP_RST_INT_WDT:
                reason_str = "watchdog-interrupt";
                break;
            case ESP_RST_TASK_WDT:
                reason_str = "watchdog-task";
                break;
            case ESP_RST_WDT:
                reason_str = "watchdog";
                break;
            case ESP_RST_DEEPSLEEP:
                reason_str = "deep-sleep";
                break;
            case ESP_RST_BROWNOUT:
                reason_str = "brownout";
                break;
            case ESP_RST_SDIO:
                reason_str = "sdio";
                break;
            case ESP_RST_USB:
                reason_str = "usb";
                break;
            case ESP_RST_JTAG:
                reason_str = "jtag";
                break;
            case ESP_RST_EFUSE:
                reason_str = "efuse";
                break;
            case ESP_RST_PWR_GLITCH:
                reason_str = "power-glitch";
                break;
            case ESP_RST_CPU_LOCKUP:
                reason_str = "cpu-lockup";
                break;
            case ESP_RST_UNKNOWN:
            default:
                reason_str = "unknown";
                break;
        }

        return JS_NewString(ctx, reason_str);
    }

    
    /**
     * 返回各个任务执行的情况
     * 
     * > 需要配置 FreeRTOS 的以下选项：
     * > * CONFIG_FREERTOS_USE_TRACE_FACILITY
     * > * CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
     * > * CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
     * 
     * @function top
     * @return string
     */
    JSValue Process::top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        bool detail = false ;
        if(argc>0) {
            detail = JS_ToBool(ctx, argv[0]) ;
        }
        string buff ;
#ifdef ESP_PLATFORM
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
        uint8_t CPU_RunInfo[1024];
        memset(CPU_RunInfo, 0, sizeof(CPU_RunInfo)); /* 信息缓冲区清零 */

        if(detail) {
#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
            vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

            // buff+= "task_name     task_status     priority stack task_id\r\n";
            // buff = "----------------------------------------------------\r\n";
            buff+= (char *)CPU_RunInfo;
#endif
        } else {

#ifdef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS

            vTaskGetRunTimeStats((char *)&CPU_RunInfo);

            // buff+= "----------------------------------------------------\r\n";
            // buff+= "task_name      run_cnt                 usage_rate   \r\n";
            buff+= (char *)CPU_RunInfo;
#endif
            }
#endif
#endif
        return JS_NewString(ctx, buff.c_str()) ;
    }
    /**
     * 返回cpu数量
     * 
     * @function cpuCount
     * @return number
     */
    JSValue Process::cpuCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewInt32(ctx, configNUMBER_OF_CORES) ;
    }
    
    /**
     * 返回各个任务执行的情况
     * 
     * > 需要配置 FreeRTOS 的以下选项：
     * > * CONFIG_FREERTOS_USE_TRACE_FACILITY
     * 
     * @function top
     * @return string
     */
    JSValue Process::tasks(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ARGV_TO_UINT8_OPT(0, core, 0) ;
        JSValue list = JS_NewArray(ctx) ;
        if(core>configNUMBER_OF_CORES-1) {
            JSTHROW("arg core id invalid", configNUMBER_OF_CORES) ;
        }
#ifdef ESP_PLATFORM
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
        TaskStatus_t *pxTaskStatusArray;
        UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();
        pxTaskStatusArray = malloc(uxArraySize * sizeof(TaskStatus_t));
    
        if (pxTaskStatusArray != NULL) {
            uxArraySize = uxTaskGetSystemState(
                pxTaskStatusArray,
                uxArraySize,
                NULL
            );

            int tindex = 0 ;
            for (UBaseType_t i = 0; i < uxArraySize; i++) {
                TaskHandle_t xTask = pxTaskStatusArray[i].xHandle;
                BaseType_t xCoreID = xTaskGetCoreID(xTask);
                if (xCoreID == core) {
                    JS_SetPropertyUint32(ctx, list, tindex++, JS_NewString(ctx, pxTaskStatusArray[i].pcTaskName));
                }
            }
            free(pxTaskStatusArray);
        }
#endif
#endif
        return list ;
    }
}
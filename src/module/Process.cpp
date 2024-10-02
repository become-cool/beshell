#include <BeShell.hpp>
#include <JSEngine.hpp>
#include "module/Process.hpp"

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "soc/soc.h"
#include "esp_efuse.h"
#include "esp_mac.h"
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
        exportFunction("top", top);
        exportFunction("usage", usage);
        exportFunction("setTime", setTime);
        exportFunction("setTimezoneOffset", setTimezone);
        exportFunction("readEFUSE", readEFUSE);
        exportFunction("readMac", readMac);

        // JS_ComputeMemoryUsage

        exportName("versions") ;
        exportName("platform") ;
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

        exportValue("versions", versions) ;

#ifdef ESP_PLATFORM
        exportValue("platform", JS_NewString(ctx, "esp32")) ;
#endif
#ifdef LiNUX_PLATFORM
        exportValue("platform", JS_NewString(ctx, "linux")) ;
#endif
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
     * @return number[] 代表硬件地址的字节数组
     */
    JSValue Process::readMac(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        uint8_t mac[6] ;
#ifdef ESP_PLATFORM
        if(esp_base_mac_addr_get(mac)!=ESP_OK) {
            JSTHROW("failed to read mac addr")
        }
#endif
        JSValue arr = JS_NewArray(ctx) ;
        for(int i=0;i<sizeof(mac);i++) {
            JS_SetPropertyUint32(ctx, arr, i, JS_NewUint32(ctx,mac[i]) ) ;
        }
        return arr ;
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
     * 打印系统任务运行状态，包括任务名称、状态、优先级、栈大小、任务ID、运行次数、运行时间等信息。
     * 
     * @function top
     * @return undefined
     */
    JSValue Process::top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
        // uint8_t CPU_RunInfo[400];
        // memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */
 
        // vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

        // printf("----------------------------------------------------\r\n");
        // printf("task_name     task_status     priority stack task_id\r\n");
        // printf("%s", CPU_RunInfo);
        // printf("----------------------------------------------------\r\n");

        // memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */

        // vTaskGetRunTimeStats((char *)&CPU_RunInfo);

        // printf("task_name      run_cnt                 usage_rate   \r\n");
        // printf("%s", CPU_RunInfo);
        // printf("----------------------------------------------------\r\n");
#endif
#endif

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
    
}
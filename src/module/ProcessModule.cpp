#include <BeShell.hpp>
#include "module/ProcessModule.hpp"

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#endif

using namespace std;

namespace be {
    ProcessModule::ProcessModule(JSContext *ctx, const char *name, uint8_t flagGlobal)
            : NativeModule(ctx, name, flagGlobal) {

        exportFunction("reboot", reboot);
        exportFunction("top", top);
        exportFunction("usage", usage);
        exportFunction("setTime", setTime);

        exportName("versions") ;
        exportName("platform") ;
    }

    void ProcessModule::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        JSValue versions = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, versions, "beshell", JS_NewString(ctx, BESHELL_VERSION));
    #ifdef ESP_PLATFORM
        JS_SetPropertyStr(ctx, versions, "esp-idf", JS_NewString(ctx, ESPIDF_VERSION));
        char lvgl_ver[16] ;
        sprintf(lvgl_ver, "%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH) ;
        JS_SetPropertyStr(ctx, versions, "lvgl", JS_NewString(ctx, lvgl_ver));
    #endif
        JS_SetPropertyStr(ctx, versions, "quickjs", JS_NewString(ctx, QUICKJS_VERSION));
        exportValue("versions", versions) ;

#ifdef ESP_PLATFORM
        exportValue("platform", JS_NewString(ctx, "esp32")) ;
#endif
#ifdef LiNUX_PLATFORM
        exportValue("platform", JS_NewString(ctx, "linux")) ;
#endif
    }

    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        esp_restart();
#endif
        return JS_NewBool(ctx, true);
    }

    JSValue ProcessModule::usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
    
    JSValue ProcessModule::top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
        uint8_t CPU_RunInfo[400];
        memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */
 
        vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息

        printf("----------------------------------------------------\r\n");
        printf("task_name     task_status     priority stack task_id\r\n");
        printf("%s", CPU_RunInfo);
        printf("----------------------------------------------------\r\n");

        memset(CPU_RunInfo, 0, 400); /* 信息缓冲区清零 */

        vTaskGetRunTimeStats((char *)&CPU_RunInfo);

        printf("task_name      run_cnt                 usage_rate   \r\n");
        printf("%s", CPU_RunInfo);
        printf("----------------------------------------------------\r\n");
#endif
#endif

        return JS_UNDEFINED ;
    }

    JSValue ProcessModule::setTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        ARGV_TO_INT64(0, ms)
        
        struct timeval tv;
        tv.tv_sec = ms/1000;  // epoch time (seconds)
        tv.tv_usec = (ms%1000)*1000;    // microseconds

        printf("%lu.%lu\n",tv.tv_sec,tv.tv_usec);

#ifdef ESP_PLATFORM
        settimeofday(&tv, NULL);
#endif

        return JS_UNDEFINED ;
    }
}
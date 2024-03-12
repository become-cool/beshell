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
    }

    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        esp_restart();
#endif
        return JS_NewBool(ctx, true);
    }

    
    JSValue ProcessModule::top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
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

        return JS_UNDEFINED ;
    }

}
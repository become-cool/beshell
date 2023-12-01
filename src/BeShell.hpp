#pragma once

#include "FS.hpp"
#include "NVS.hpp"
#include "debug.h"
#include "Telnet.hpp"
#include "JSEngine.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include<queue>

namespace beshell {

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;


        TaskHandle_t handleLoop = nullptr ;


        SemaphoreHandle_t mutex_code;


        static void main(void * argv) ;
    public:

        JSEngine engine ;
        NVS nvs ;
        FS fs ;
        Telnet telnet ;

        BeShell() ;
        void begin(int coreId=-1);

        void eval(const char * code, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
    } ;

}
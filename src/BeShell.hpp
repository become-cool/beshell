#pragma once

#include "quickjs-libc.h"
#include "FS.hpp"
#include "NVS.hpp"
#include "debug.h"
#include "Telnet.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace beshell {

    class BeShell {

    private:
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;

        NVS nvs ;
        FS fs ;
        Telnet telnet ;

        TaskHandle_t handleLoop = nullptr ;

        void newJSRuntime() ;

        static void main(void * argv) ;
    public:


        BeShell() ;
        void begin(int coreId=-1);
    } ;

}
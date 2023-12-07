#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <queue>

#include "FS.hpp"
#include "NVS.hpp"
#include "debug.h"
#include "Telnet.hpp"
#include "JSEngine.hpp"
#include "REPL.hpp"


namespace beshell {

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;

        TaskHandle_t handleLoop = nullptr ;

        SemaphoreHandle_t mutex_code;

    public:

        NVS nvs ;
        FS fs ;
        Telnet telnet ;
        REPL repl ;

        JSEngine engine ;

        BeShell() ;
        void setup();
        inline void loop() ;
        void main() ;

        static BeShell * fromJSContext(JSContext *) ;
        static BeShell * fromJSRuntime(JSRuntime *) ;

    } ;

}
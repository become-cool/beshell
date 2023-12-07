#pragma once

#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#endif 

#include <queue>

#include "NVS.hpp"
#include "debug.h"
#include "JSEngine.hpp"
#include "REPL.hpp"
#include "fs/FS.hpp"
#include "telnet/Telnet.hpp"


namespace beshell {

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;

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
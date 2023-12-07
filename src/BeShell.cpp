#include "beshell.hpp"
#include <iostream>
#include <string.h>
#include "utils.h"


#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"
#endif

namespace beshell {


    BeShell::BeShell()
        : boot_level(5)
        , telnet(this)
        , repl(this)
        , engine(&telnet)
    {}

    
    void BeShell::setup() {

        // ESP_ERROR_CHECK(esp_event_loop_create_default());
        
        nvs.readOneTime("rst-lv", &boot_level) ;
        nvs.readOneTime("rst-nowifi", (uint8_t *)&nowifi) ;

        // 文件系统
#ifdef PLATFORM_ESP32   
        if(nowifi) {
            cout << "disabled wifi by nvs setting: rst-nowifi" << endl ;
        } else {
            // be_module_wifi_init() ;
        }
        // fs.setPrefix("/fs") ;
        // fs.mountRootTar() ;
#else
#endif

        telnet.setup() ;

        engine.setup(this) ;
    }

    void BeShell::loop() {

        telnet.loop() ;

        js_std_loop(engine.ctx) ;
    }

    void BeShell::main() {

        setup() ;

        while(1) {
            loop() ;

#ifdef PLATFORM_ESP32
            vTaskDelay(1) ;
#endif
        }
    }

    BeShell * BeShell::fromJSContext(JSContext * ctx) {
        return (BeShell *)JS_GetRuntimeOpaque( JS_GetRuntime(ctx) ) ;
    }
    BeShell * BeShell::fromJSRuntime(JSRuntime * rt) {
        return (BeShell *)JS_GetRuntimeOpaque2(rt) ;
    }

}
#include "beshell.hpp"
#include <iostream>
#include <string.h>
#include "utils.h"
#include "eventloop.h"

#include "module_telnet.h"
#include "module_utils.h"


#ifndef PLATFORM_WASM
#include "module_mg.h"
#endif
#include "module_fs.h"

#ifdef PLATFORM_ESP32
#include "psram.h"
#include "sniffer.h"
#include "module_bt.h"
#include "module_wifi.h"
#include "module_gpio.h"
#include "module_serial.h"
#include "module_socks.h"
#include "module_driver.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"
#include "module_media.h"
#endif

namespace beshell {


    BeShell::BeShell()
        : boot_level(5)
        , telnet(this)
        , engine(&telnet)
    {}

    
    void BeShell::begin(int coreId) {
        
        nvs.readOneTime("rst-lv", &boot_level) ;
        nvs.readOneTime("rst-nowifi", (uint8_t *)&nowifi) ;

        // 文件系统
#ifdef PLATFORM_ESP32   
        if(nowifi) {
            cout << "disabled wifi by nvs setting: rst-nowifi" << endl ;
        } else {
            // be_module_wifi_init() ;
        }
        fs.mountRootTar() ;
#else
#endif

        engine.initRuntime() ;

        telnet.setup() ;
        
        if(coreId<0) {
            coreId = xPortGetCoreID() ;
        }
        xTaskCreatePinnedToCore(&BeShell::main, "be-main-loop", 20*1024, this, tskIDLE_PRIORITY, &handleLoop, coreId) ;
    }

    void BeShell::main(void * argv) {
        while(1) {

            ((BeShell*)argv)->telnet.loop() ;

            // std::cout << "xxxx\n" ;

            vTaskDelay(0) ;
        }
    }

    void eval(const char * code, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) {

    }
}
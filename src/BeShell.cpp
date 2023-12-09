#include "BeShell.hpp"
#include <iostream>
#include <string.h>
#include "utils.h"


#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"
#endif

using namespace std ;

namespace be {


    BeShell::BeShell()
        : boot_level(5)
    {
        nvs = new NVS() ;
        fs = new FS() ;
        repl = new REPL(this) ;
        telnet = new Telnet(this) ;
        engine = new JSEngine(telnet) ;

        p1 = this ;
        p2 = this ;

        cout << endl ;
    }
    BeShell::~BeShell() {
        delete nvs ;
        delete fs ;
        delete repl ;
        delete telnet ;
        delete engine ;
        
        nvs = nullptr ;
        fs = nullptr ;
        repl = nullptr ;
        telnet = nullptr ;
        engine = nullptr ;
    }

    
    void BeShell::setup() {

        // ESP_ERROR_CHECK(esp_event_loop_create_default());
        
        nvs->readOneTime("rst-lv", &boot_level) ;
        nvs->readOneTime("rst-nowifi", (uint8_t *)&nowifi) ;

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

        telnet->setup() ;

        engine->setup(this) ;
    }

    void BeShell::loop() {

        telnet->loop() ;

        engine->loop() ;
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


}
#include "BeShell.hpp"
#include "module/FSModule.hpp"
#include <iostream>
#include <string.h>
#include "utils.h"


#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"
#include "esp_event_loop.h"
#endif

using namespace std ;

namespace be {


    BeShell::BeShell()
        : boot_level(5)
        , engine(new JSEngine(this))
    {
        nvs = new NVS() ;
        telnet = new Telnet(this) ;

        cout << endl ;
    }
    #define DELETE_VAR(var) \
        if(var) {           \
            delete var ;    \
            var = nullptr ; \
        }

    BeShell::~BeShell() {
        DELETE_VAR(nvs)
        DELETE_VAR(fs)
        DELETE_VAR(repl)
        DELETE_VAR(telnet)
        DELETE_VAR(engine)
    }
    
    void BeShell::useFS() {
        if(fs) {
            return ;            
        }
        fs = new FS() ;
        engine->mloader.addModule(new FSModule()) ;
    }
    void BeShell::useREPL() {
        if(repl) {
            return ;
        }
        repl = new REPL(this) ;
    }

    void BeShell::setup() {

#ifdef PLATFORM_ESP32   
        ESP_ERROR_CHECK(esp_event_loop_create_default());
#endif
        
        nvs->readOneTime("rst-lv", &boot_level) ;
        nvs->readOneTime("rst-nowifi", (uint8_t *)&nowifi) ;

        telnet->setup() ;

        engine->setup() ;
    }

    void BeShell::loop() {

        telnet->loop() ;

        engine->loop() ;
    }

    void BeShell::run() {
        while(1) {
            loop() ;

#ifdef PLATFORM_ESP32
            vTaskDelay(1) ;
#endif
        }
    }

    void BeShell::main() {
        useFS() ;
        useREPL() ;

        setup() ;
        run() ;
    }


}
#include "BeShell.hpp"
#include "module/FSModule.hpp"
#include "module/NVSModule.hpp"
#include <iostream>
#include <string.h>
#include "utils.h"


#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "esp_event_loop.h"
#endif

using namespace std ;

namespace be {


    BeShell::BeShell()
        : boot_level(5)
        , engine(new JSEngine(this))
    {
        telnet = new Telnet(this) ;

        cout << endl ;
    }
    #define DELETE_VAR(var) \
        if(var) {           \
            delete var ;    \
            var = nullptr ; \
        }

    BeShell::~BeShell() {
        DELETE_VAR(fs)
        DELETE_VAR(repl)
        DELETE_VAR(telnet)
        DELETE_VAR(engine)
    }
    
    void BeShell::useFS(const char * mountPath, FSPartition * partition) {
        if(!fs) {
            fs = new FS() ;
            engine->addModule(new FSModule()) ;
        }
        if(mountPath && partition) {
            fs->mount(mountPath,partition) ;
        }
    }
    void BeShell::useREPL() {
        if(repl) {
            return ;
        }
        repl = new REPL(this) ;
    }
    void BeShell::useNVS() {
        engine->addModule(new NVSModule) ;
    }

    void BeShell::useBasic() {
        useFS() ;
        useREPL() ;
        useNVS() ;
    }

    void BeShell::setup() {

#ifdef PLATFORM_ESP32   
        ESP_ERROR_CHECK(esp_event_loop_create_default());
#endif

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
        useBasic() ;
        setup() ;
        run() ;
    }
    
}
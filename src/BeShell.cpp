#include "BeShell.hpp"
#include "fs/FSModule.hpp"
#include "module/NVSModule.hpp"
#include "module/DeviceModule.hpp"
#include <iostream>
#include <string.h>
#include "qjs_utils.h"
// #include "js/device.h"


#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "esp_event_loop.h"

#include "lv/LVModule.hpp"
#include "module/serial/SerialModule.hpp"
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
#ifdef MODULE_LV
        DELETE_VAR(lv)
#endif
    }

    void BeShell::useBasic() {
        useFS() ;
        useREPL() ;
        NVSModule::use(this) ;
    }
    
    void BeShell::useFS(const char * mountPath, FSPartition * partition) {
        if(!fs) {
            fs = new FS() ;
            engine->mloader.add<FSModule>("fs") ;
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
    
    #ifdef MODULE_SERIAL
    void BeShell::useSerial() {
        SerialModule::use(*this) ;
    }
    #endif

#ifdef MODULE_LV
    void BeShell::useLV() {
        if(lv) {
            return ;
        }
        lv::LVModule::use(*this) ;
        lv = new lv::LV ;
    }
#endif


    void BeShell::addLoopFunction(LoopFunction func) {
        loopFunctions.push_back(func) ;
    }

    void BeShell::setup() {

// #ifdef ESP_PLATFORM
//         ESP_ERROR_CHECK(esp_event_loop_create_default());
// #endif

        telnet->setup() ;

        engine->setup() ;

#ifdef MODULE_LV
        if(lv) {
            lv->setup(*this) ;
        }
#endif

        for(auto func:loopFunctions) {
            func(*this) ;
        }
    }

    void BeShell::useDeviceJSON(const char * path) {
        DeviceModule::use(*this) ;
        DeviceModule::setDeviceJsonPath(path) ;
    }

    void BeShell::loop() {

        telnet->loop() ;

        engine->loop() ;

#ifdef MODULE_LV
        if(lv) {
            lv->loop() ;
        }
#endif
    }

    void BeShell::run() {
        while(1) {
            loop() ;

#ifdef ESP_PLATFORM
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
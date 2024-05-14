#include "BeShell.hpp"
#include "fs/FS.hpp"
#include "module/NVSModule.hpp"
#include <iostream>
#include <string.h>
#include "qjs_utils.h"


#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "esp_event_loop.h"

#include "lv/LV.hpp"
#include "module/serial/SerialModule.hpp"
#include "js/device.c"
#include "module/DeviceModule.hpp"
#endif

using namespace std ;

namespace be {


    BeShell::BeShell()
        : boot_level(5)
        , engine(new JSEngine(this))
        , telnet(new Telnet(this))
        , repl(new REPL(this))
    {}
    
    #define DELETE_VAR(var) \
        if(var) {           \
            delete var ;    \
            var = nullptr ; \
        }

    BeShell::~BeShell() {
        DELETE_VAR(repl)
        DELETE_VAR(telnet)
        DELETE_VAR(engine)
    }
    
    void BeShell::addLoopFunction(LoopFunction func, void * opaque, bool ignoreRepeat) {
        if(ignoreRepeat) {
            for(auto _pair:loopFunctions) {
                if(func==_pair.first) {
                    return ;
                }
            }
        }
        loopFunctions.push_back( std::pair<LoopFunction,void *>(func,opaque) ) ;
    }

    void BeShell::setup() {

        telnet->setup() ;

        engine->setup() ;

#ifdef ESP_PLATFORM
        if(bUseDeviceJSON) {
            JSEngineEvalEmbeded(engine->ctx, device)
        }
#endif
    }

#ifdef ESP_PLATFORM
    void BeShell::useDeviceJSON(const char * path) {
        useModule<DeviceModule>() ;
        DeviceModule::setDeviceJsonPath(path) ;
        bUseDeviceJSON = true ;
    }
#endif

    void BeShell::loop() {

        telnet->loop() ;

        engine->loop() ;

#ifdef MODULE_LV
        if(lv) {
            lv->loop() ;
        }
#endif

        for(auto pair:loopFunctions) {
            pair.first(*this, pair.second) ;
        }
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
        setup() ;
        run() ;
    }
    
}
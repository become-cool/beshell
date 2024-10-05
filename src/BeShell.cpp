#include "BeShell.hpp"
#include "fs/FS.hpp"
#include "module/NVS.hpp"
#include <iostream>
#include <string.h>
#include "qjs_utils.h"


#ifdef ESP_PLATFORM
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

    void BeShell::setup(const char * mainScriptPath) {
        telnet->setup() ;
        engine->setup() ;

        // output banner
        cout << R"(
 ____      ____                                     ____                       
|    ~.   |                       ..''''|         ||           |       |       
|____.'_  |______              .''      |_________||______     |       |       
|       ~.|                 ..'         |         ||           |       |       
|_______.'|___________....''            |         ||___________|_______|_______
 (c) 2020-2024 https://become.cool

)" ;
        cout    << "version BeShell: " << BESHELL_VERSION 
                << "; quickjs: " << QUICKJS_VERSION
                << " ; esp-idf: " << ESPIDF_VERSION 
                << "\r\n" ;
        cout    << "build: " << __DATE__ << " " << __TIME__ << "\r\n" ;
        cout    << "type JavaScript code to run, or 'help' for more information\r\n\r\n" ;

        if(mainScriptPath) {
#ifdef ESP_PLATFORM
            esp_reset_reason_t reset_reason = esp_reset_reason();
            if(reset_reason==ESP_RST_PANIC) {
                printf("BeShell was rebooted due to a crash\n") ;
            } else {
                engine->evalScript(mainScriptPath) ;
            }
#else
            engine->evalScript(mainScriptPath) ;
#endif
        }
    }

    void BeShell::run() {
        while(1) {
            loop() ;
        }
    }

    void BeShell::main(const char * mainScriptPath) {
        setup(mainScriptPath) ;
        run() ;
    }
}
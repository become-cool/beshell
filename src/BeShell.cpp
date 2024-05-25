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

    void BeShell::setup() {
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
    }

    void BeShell::loop() {

        telnet->loop() ;

        engine->loop() ;

        for(auto pair:loopFunctions) {
            pair.first(*this, pair.second) ;
        }

#ifdef ESP_PLATFORM
        vTaskDelay(1) ;
#endif
    }

    void BeShell::run() {
        while(1) {
            loop() ;
        }
    }

    void BeShell::main() {
        setup() ;
        run() ;
    }
    
}
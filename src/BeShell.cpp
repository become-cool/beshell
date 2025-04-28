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
 (c) 2020-2025 https://become.cool

)" ;
        cout    << "version BeShell: " << BESHELL_VERSION 
                << "; quickjs: " << QUICKJS_VERSION
                << " ; esp-idf: " << ESPIDF_VERSION 
                << "\r\n" ;
        cout    << "build: " << __DATE__ << " " << __TIME__ << "\r\n" ;
        cout    << "type JavaScript code to run, or 'help' for more information\r\n\r\n" ;

        vTaskPrioritySet(NULL, 10) ;
    }

    void BeShell::run() {

        uint32_t lastLoopTime = 0 ;

        while(1) {
            loop() ;

#ifdef ESP_PLATFORM
            vTaskDelay( (lastLoopTime++%10)==0? 0: 1 ) ;
#endif
        }
    }

    void BeShell::main(const char * mainScriptPath) {
        setup() ;
        
        char mainRunPath[256] = {0} ;
        if( NVS::readString("main-run", mainRunPath, sizeof(mainRunPath)-1)==0 ){
            NVS::erase("main-run") ;
            engine->evalScript(mainRunPath) ;
        }

        else if(mainScriptPath) {
#ifdef ESP_PLATFORM
            bool delayRunScript = false ;
            esp_reset_reason_t reset_reason = esp_reset_reason();
            if(reset_reason==ESP_RST_PANIC) {
                printf("\nBeShell was rebooted due to a crash\n") ;
                delayRunScript = true ;
            }
            else if(reset_reason==ESP_RST_INT_WDT) {
                printf("\nBeShell was rebooted due to an interrupt watchdog timeout\n") ;
                delayRunScript = true ;
            }
            
            #define BESHELL_CRASH_RECOVERY_DELAY_SEC 30

            if(delayRunScript) {
                printf("Waiting %d seconds before executing main script...\n", BESHELL_CRASH_RECOVERY_DELAY_SEC);
                
                // Use the existing loop structure for timing
                uint32_t startTick = xTaskGetTickCount();
                uint32_t lastLoopTime = 0;
                uint32_t lastPrintTime = 0;
                
                // Wait using existing loop timing
                while((xTaskGetTickCount() - startTick) < (BESHELL_CRASH_RECOVERY_DELAY_SEC * 1000 / portTICK_PERIOD_MS)) {
                    // Run the regular loop
                    loop();
                    
                    // Print countdown approximately once per second
                    uint32_t currentTick = xTaskGetTickCount();
                    uint32_t elapsedSec = (currentTick - startTick) / (1000 / portTICK_PERIOD_MS);
                    uint32_t remainingSec = BESHELL_CRASH_RECOVERY_DELAY_SEC - elapsedSec;
                    
                    if(currentTick - lastPrintTime >= (5000 / portTICK_PERIOD_MS)) {
                        printf("Starting in %d seconds...\n", remainingSec);
                        fflush(stdout);
                        lastPrintTime = currentTick;
                    }
                    
                    // Maintain original loop timing
                    vTaskDelay((lastLoopTime++%10)==0? 0: 1);
                }
                
                printf("\nResuming normal operation\n");
            }
#endif

            engine->evalScript(mainScriptPath) ;
        }

        run() ;
    }
}
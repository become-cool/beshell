#pragma once

#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#endif 

#include <queue>

#include "debug.h"
#include "JSEngine.hpp"
#include "repl/REPL.hpp"
#include "fs/FS.hpp"
#include "telnet/Telnet.hpp"



#define BESHELL_VERSION "0.3.0"
#define ESPIDF_VERSION IDF_VER
#define QUICKJS_VERSION "2021-03-27"


namespace be {

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;

    public:
        FS * fs = nullptr ;
        Telnet * telnet = nullptr ;
        REPL * repl = nullptr ;

        JSEngine * engine = nullptr ;

        BeShell() ;
        ~BeShell() ;
        void setup();
        inline void loop() ;
        void run() ;
        void main() ;


        template <typename M>
        void addModule(const char * name) {
            engine->mloader.add<M>(name) ;
        }

        void useBasic() ;
        void useFS(const char * mountPath=nullptr, FSPartition * partition=nullptr) ;
        void useREPL() ;
        void useSerial() ;
    } ;

}
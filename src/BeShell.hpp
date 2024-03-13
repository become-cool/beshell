#pragma once

#include <vector>
#include "debug.h"
#include "JSEngine.hpp"
#include "repl/REPL.hpp"
#include "fs/FS.hpp"
#include "telnet/Telnet.hpp"

#ifdef ESP_PLATFORM
#include "lv/LV.hpp"
#endif


#define BESHELL_VERSION "0.3.0"
#define ESPIDF_VERSION IDF_VER
#define QUICKJS_VERSION "2021-03-27"


#ifdef ESP_PLATFORM
#define MODULE_SERIAL
#define MODULE_LV
#endif



namespace be {
    
    class BeShell ;
	typedef void (*LoopFunction)(const BeShell &) ;

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;
        bool bUseDeviceJSON = false ;

        std::vector<LoopFunction> loopFunctions ; 

    public:
        FS * fs = nullptr ;
        Telnet * telnet = nullptr ;
        REPL * repl = nullptr ;
#ifdef MODULE_LV
        lv::LV * lv = nullptr ;
#endif

        JSEngine * engine = nullptr ;

        BeShell() ;
        ~BeShell() ;
        void setup();
        inline void loop() ;
        void run() ;
        void main() ;


        template <typename M>
        void addModule(const char * name=nullptr) {
            engine->mloader.add<M>(name) ;
        }

        void addLoopFunction(LoopFunction func) ;

        void useBasic() ;
        void useFS(const char * mountPath=nullptr, FSPartition * partition=nullptr) ;
        void useREPL() ;
        
        void useSerial() ;
#ifdef MODULE_LV
        void useLV() ;
#endif
        void useDeviceJSON(const char * path="/config/device.json") ;
    } ;

}
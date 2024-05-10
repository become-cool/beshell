#pragma once

#include <vector>
#include "include.hpp"

#ifdef ESP_PLATFORM
#include "lv/LV.hpp"
#endif


#define BESHELL_VERSION "0.3.1"
#define ESPIDF_VERSION IDF_VER
#define QUICKJS_VERSION "2021-03-27"


#ifdef ESP_PLATFORM
#define MODULE_SERIAL
#define MODULE_LV
#endif



namespace be {
    
    class BeShell ;
	typedef void (*LoopFunction)(const BeShell &, void *) ;

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;
        bool bUseDeviceJSON = false ;

        std::vector<std::pair<LoopFunction,void *>> loopFunctions ; 

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

        void addLoopFunction(LoopFunction func, void * opaque=nullptr, bool ignoreRepeat=true) ;

        void useBasic() ;
        void useFS(const char * mountPath=nullptr, FSPartition * partition=nullptr) ;
        void useREPL() ;
        
        void useSerial() ;
#ifdef MODULE_LV
        void useLV() ;
#endif
#ifdef ESP_PLATFORM
        void useDeviceJSON(const char * path="/config/device.json") ;
#endif
        
        template <typename ModuleClass>
        void use(const char * name=nullptr) {
            engine->mloader.add<ModuleClass>(this, name) ;
        }
        template <typename ModuleClass>
        void useModule(const char * name=nullptr) {
            use<ModuleClass>(name) ;
        }

        template <typename DriverClass>
        void useDriver() {
            be::driver::DriverModule::useDriver<DriverClass>(this) ;
        }
    } ;

}
#pragma once

#include <vector>
#include "include.hpp"


#define BESHELL_VERSION "0.3.1"
#define ESPIDF_VERSION IDF_VER
#define QUICKJS_VERSION "2021-03-27"

namespace be {
    
    class BeShell ;
	typedef void (*LoopFunction)(const BeShell &, void *) ;

    class BeShell {

    private:

        uint8_t boot_level = 5 ;
        bool requst_reset = false ;
        bool nowifi = false ;

        std::vector<std::pair<LoopFunction,void *>> loopFunctions ; 

    public:
        JSEngine * engine ;
        Telnet * telnet ;
        REPL * repl ;


        BeShell() ;
        ~BeShell() ;
        void setup();
        void loop() ;
        void run() ;
        void main() ;

        void addLoopFunction(LoopFunction func, void * opaque=nullptr, bool ignoreRepeat=true) ;
        
        template <typename ModuleClass>
        void use(const char * name=nullptr) {
            useModule<ModuleClass>(name) ;
        }
        template <typename ModuleClass>
        void useModule(const char * name=nullptr) {
            engine->mloader.add<ModuleClass>(this, name) ;
        }

        template <typename DriverClass>
        void useDriver() {
            be::driver::DriverModule::useDriver<DriverClass>(this) ;
        }
    } ;

}
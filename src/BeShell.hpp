#pragma once

#include <vector>
#include "include.hpp"


#define BESHELL_VERSION "0.3.6"
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
        void setup() ;
        inline void loop() {
            telnet->loop() ;
            engine->loop() ;
            for(auto pair:loopFunctions) {
                pair.first(*this, pair.second) ;
            }
        }
        void run() ;
        void main(const char * mainScriptPath=nullptr) ;

        void addLoopFunction(LoopFunction func, void * opaque=nullptr, bool ignoreRepeat=true) ;

        
        // NativeModule 类
        template <typename CLASS, typename std::enable_if<std::is_base_of<NativeModule, CLASS>::value, int>::type = 0>
        void use() {
            engine->mloader.add<CLASS>(this, nullptr) ;
        }

        // driver (NativeClass 类)
        template <typename CLASS, typename std::enable_if<std::is_base_of<NativeClass, CLASS>::value, int>::type = 0>
        void use() {
            be::driver::DriverModule::useDriver<CLASS>() ;
        }

        // TelnetChannel 类
        template <typename CLASS, typename std::enable_if<std::is_base_of<TelnetChannel, CLASS>::value, int>::type = 0>
        void use() {
            CLASS * channel = new CLASS(telnet) ;
            telnet->addChannel(channel) ;
        }
    } ;

}
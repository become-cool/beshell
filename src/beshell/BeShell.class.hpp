#pragma once

#include <vector>
#include "./JSEngine.hpp"
#include "./NativeModule.hpp"
#include "./cammonds/Cammonds.hpp"
#include "./repl/REPL.hpp"
#include "./driver/DriverModule.hpp"
#include "./deps/quickjs/quickjs.h"

#define BESHELL_VERSION "1.0.14"
#define ESPIDF_VERSION IDF_VER

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
        Cammonds * cammonds ;
        REPL * repl ;


        BeShell() ;
        ~BeShell() ;
        void setup() ;
        inline void loop() {
            repl->loop() ;
            engine->loop() ;
            for(auto pair:loopFunctions) {
                pair.first(*this, pair.second) ;
            }
        }
        void evalMain(const char * mainScriptPath) ;
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

        // REPLChannel 类
        template <typename CLASS, typename std::enable_if<std::is_base_of<REPLChannel, CLASS>::value, int>::type = 0>
        void use() {
            CLASS * channel = new CLASS(repl) ;
            repl->addChannel(channel) ;
        }
    } ;

}

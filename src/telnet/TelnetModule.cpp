#include "TelnetModule.hpp"
#include "TelnetChannelNClass.hpp"
#include "../js/telnet.c"

namespace be{

    char const * const TelnetModule::name = "telnet" ;

    std::vector<NativeModuleExportorFunc> TelnetModule::exportors ;

    TelnetModule::TelnetModule(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportClass<TelnetChannelNClass>() ;
        exportName("ws") ;
    }

    void TelnetModule::exports(JSContext *ctx) {
        for(auto func : exportors) {
            func(ctx, this) ;
        }

        JSEngineEvalEmbeded(ctx, telnet)
    }

}
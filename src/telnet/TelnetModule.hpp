#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class TelnetModule: public be::NativeModule {
    public:
        static const char * const name ;

        TelnetModule(JSContext * ctx, const char * name) ;
        void import(JSContext *ctx) ;
    } ;
}

#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {
    class SerialModule: public NativeModule {
    protected:
        void import(JSContext *ctx) ;
    public:

        static const char *name ;

        SerialModule(JSContext * ctx, const char * name) ;
        ~SerialModule() ;
        
    } ;
}
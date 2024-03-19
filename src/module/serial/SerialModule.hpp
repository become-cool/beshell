#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {
    class SerialModule: public NativeModule {
    protected:
        void import(JSContext *ctx) ;
    public:

        SerialModule(JSContext * ctx, const char * name) ;
        ~SerialModule() ;

        inline static void use(BeShell & beshell) {
            beshell.addModule<SerialModule>("serial") ;
        }
        
    } ;
}
#pragma once

#include "NativeModule.hpp"

namespace be {
    class Serial: public NativeModule {
    protected:
        void exports(JSContext *ctx) ;
    public:

        static const char *name ;

        Serial(JSContext * ctx, const char * name) ;
        ~Serial() ;
        
    } ;
}
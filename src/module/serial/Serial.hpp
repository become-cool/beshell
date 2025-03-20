#pragma once

#include "NativeModule.hpp"

namespace be {
    class Serial: public NativeModule {
    protected:
        void exports(JSContext *ctx) ;
    public:

        static char const * const name ;

        Serial(JSContext * ctx, const char * name) ;
        ~Serial() ;
        
    } ;
}
#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    

    class Flash: public be::NativeModule {
    public:
        static const char * name ;
        Flash(JSContext * ctx, const char * name) ;
        static JSValue allPartitions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue partition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}

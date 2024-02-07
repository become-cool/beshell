#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {

class ParallelModule: public be::NativeModule {
public:
    ParallelModule(JSContext * ctx, const char * name) ;

    void import() ;

    inline static void use(be::BeShell & beshell) {
        beshell.addModule<ParallelModule>("parallel") ;
    }

    static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
} ;

}

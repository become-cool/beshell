#include "BTModule.hpp"

namespace be {

    const char * const BTModule::name = "bt" ;

    BTModule::BTModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("func",jsFunc,0) ;
    }

    // void BTModule::import(JSContext *ctx) {
    // }

    
}
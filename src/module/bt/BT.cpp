#include "BT.hpp"

namespace be {

    const char * const BT::name = "bt" ;

    BT::BT(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("func",jsFunc,0) ;
    }

    // void BT::import(JSContext *ctx) {
    // }

    
}
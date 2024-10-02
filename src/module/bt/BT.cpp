#include "BT.hpp"

namespace be {

    const char * const BT::name = "bt" ;

    BT::BT(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
    }

    // void BT::exports(JSContext *ctx) {
    // }

    
}
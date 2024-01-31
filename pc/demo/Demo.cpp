#include "Demo.hpp"

using namespace std ;
using namespace be ;


JSClassID DemoClass::classID ;
std::map<JSContext*, be::NativeClass*> DemoClass::mapCtxClasses ;


JSValue DemoClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    std::cout << "111111111111" << std::endl ;
    return JS_UNDEFINED ;
}

void useDemo(JSContext * ctx) {
    dd
    DemoClass::create(ctx) ;
}
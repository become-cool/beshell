#include "Demo.hpp"

using namespace std ;
using namespace be ;


const char * DemoClass::className = "DemoClass" ;
JSClassID DemoClass::classID = 0 ;
std::map<JSContext*, be::NativeClass<DemoClass>*> DemoClass::mapCtxClasses ;


// JSValue DemoClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     return JS_UNDEFINED ;
// }

void useDemo(JSContext * ctx) {
    // DemoClass::create(ctx) ;
}
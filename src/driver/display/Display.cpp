#include <iostream>
#include "Display.hpp"
#include "qjs_utils.h"

using namespace std ;


namespace be {
namespace driver {
namespace display {


DEFINE_NCLASS_META(Display, NativeClass)
std::vector<JSCFunctionListEntry> Display::methods = {
    JS_CFUNC_DEF("drawRect", 0, Display::jsDrawRect),
} ;

Display::Display(JSContext * ctx, JSValue _jsobj)
    : NativeClass(ctx, build(ctx,_jsobj))
{
    dd
}

void Display::drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) {
    cout << "not implements" << endl ;
}

JSValue Display::jsDrawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)

    return JS_UNDEFINED ;
}

JSValue Display::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    auto obj = new Display(ctx) ;
    obj->self = std::shared_ptr<Display> (obj) ;
    return obj->jsobj ;
}

}}}
#include <iostream>
#include "Display.hpp"
#include "qjs_utils.h"

using namespace std ;


namespace be {
namespace driver {
namespace display {


DEFINE_NCLASS_META(Display)
std::vector<JSCFunctionListEntry> Display::methods = {
    JS_CFUNC_DEF("drawRect", 0, Display::jsDrawRect),
} ;

Display::Display(JSContext * ctx)
    : NativeClass(ctx)
{}

void Display::drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) {
    cout << "not implements" << endl ;
}

JSValue Display::jsDrawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(5)
    return JS_UNDEFINED ;
}

    
}}}
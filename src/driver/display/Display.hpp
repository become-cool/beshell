#pragma once

#include <NativeClass.hpp>

namespace be {
namespace driver {
namespace display {

    #define coord_t uint16_t
    #define color_t uint16_t

class Display: public be::NativeClass<Display> {
    DECLARE_NCLASS_META
    static std::vector<JSCFunctionListEntry> methods ;
    // static std::vector<JSCFunctionListEntry> staticMethods ;

public:
    Display(JSContext * ctx) ;

    virtual void drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) ;

    static JSValue jsDrawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

} ;

}}}
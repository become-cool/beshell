#pragma once

#include <NativeClass.hpp>

class SPI: public be::NativeClass<SPI> {
    DECLARE_NCLASS_META
    static std::vector<JSCFunctionListEntry> methods ;
    // static std::vector<JSCFunctionListEntry> staticMethods ;

private:
    uint8_t busnum ;

public:
    SPI(JSContext * ctx, uint8_t busnum=1) ;

    static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

} ;
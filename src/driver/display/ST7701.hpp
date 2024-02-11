#pragma once

#include "RGB565.hpp"

namespace be {
namespace driver {
namespace display {

class ST7701: public RGB565 {
    DECLARE_NCLASS_META
    static std::vector<JSCFunctionListEntry> methods ;
    // static std::vector<JSCFunctionListEntry> staticMethods ;

private:
    void initGPIO() ;
    void initReg() ;
    
    void writeData(uint8_t data) ;
    void writeCmd(uint8_t cmd) ;
    void write9b(uint16_t data) ;

public:
    ST7701(JSContext * ctx, JSValue jsobj=JS_NULL) ;
    
    static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    static JSValue jsTest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

} ;

}}}
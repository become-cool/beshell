#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sensor {
    class VL53L0X: public I2CDevice {
        DECLARE_NCLASS_META
    public:
        typedef enum {Single = 0, Continuous = !Single} ModeState;
        typedef enum {High = 0, Low = !High} PrecisionState;
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        ModeState mode = Continuous ;
        PrecisionState precision = High ;
	    
        uint16_t lastDistance = 0;

        bool dataInit() ;
    public:

        VL53L0X(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup() ;
        bool setMode(ModeState mode, PrecisionState prec) ;
        bool start() ;
        bool stop() ;
        bool readDistance(uint16_t & dist) ;

        static JSValue start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readDistance(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}
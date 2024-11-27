#pragma once

#include <EventEmitter.hpp>
#include "driver/i2s.h"
#include "./soc_serial.h"

namespace be {
    class I2S: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        static I2S * i2s0 ;

        #if SOC_I2S_NUM > 1
        static I2S * i2s1 ;
        #endif
        
        i2s_port_t busnum ;

        TaskHandle_t taskHandle = nullptr ;
        static void taskRecording(void *arg) ;

    public:
        I2S(JSContext * ctx, i2s_port_t busnum) ;

        static I2S * flyweight(JSContext *, i2s_port_t) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        void onNativeEvent(JSContext *ctx, void * param) ;
        static JSValue startRecord(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopRecord(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}
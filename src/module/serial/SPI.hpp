#pragma once

#include <NativeClass.hpp>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include <map>

namespace be {

    class SPI: public NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        spi_host_device_t busnum ;

        static SPI * spi1 ;
        static SPI * spi2 ;
        static SPI * spi3 ;

        std::map<gpio_num_t, spi_device_handle_t> devices ;

    public:
        SPI(JSContext * ctx, JSValue jsobj=JS_NULL, spi_host_device_t busnum=SPI1_HOST) ;

        static SPI * flyweight(JSContext *, spi_host_device_t) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sendU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sendU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sendU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recvU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recvU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recvU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue transU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue transU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue transU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}
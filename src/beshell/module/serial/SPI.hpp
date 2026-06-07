#pragma once

#include "../../NativeClass.hpp"
#include "./soc_serial.h"
#include <map>
#include <driver/spi_master.h>

namespace be {

    class SPI: public NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;

    private:
        int busnum ;
        int nextDeviceId = 1 ;
        std::map<int, spi_device_handle_t> devices ;

        static SPI * spi0 ;
        #if SOC_SPI_PERIPH_NUM > 1
        static SPI * spi1 ;
        #endif
        #if SOC_SPI_PERIPH_NUM > 2
        static SPI * spi2 ;
        #endif
        #if SOC_SPI_PERIPH_NUM > 3
        static SPI * spi3 ;
        #endif

    public:
        SPI(JSContext * ctx, int busnum) ;

        int spiNum() const ;

        static SPI * flyweight(JSContext *, int) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue spiNum(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue trans(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
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
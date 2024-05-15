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
        #if SOC_SPI_PERIPH_NUM > 2
        static SPI * spi2 ;
        #endif
        #if SOC_SPI_PERIPH_NUM > 3
        static SPI * spi3 ;
        #endif
        #if SOC_SPI_PERIPH_NUM > 4
        static SPI * spi4 ;
        #endif

        std::map<gpio_num_t, spi_device_handle_t> devices ;

    public:
        SPI(JSContext * ctx, spi_host_device_t busnum=SPI1_HOST) ;

        spi_host_device_t spiNum() const ;

        static SPI * flyweight(JSContext *, spi_host_device_t) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue spiNum(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
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
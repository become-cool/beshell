#pragma once
#include "sdkconfig.h"
#if CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY

#include "../../NativeClass.hpp"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "soc/soc_caps.h"
#include "./soc_serial.h"

namespace be {

    /**
     * 该类的构造函数没有绑定给 JS , 无法从 JS 创建实例。
     *
     * 模块 [serial](overview.md) 创建了和硬件对应的 I2S 实例，`import serial` 即可。
     *
     * @module serial
     * @class LegacyI2S
     */
    class I2S: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;

    private:
        i2s_port_t busnum ;
        bool installed = false ;

        gpio_num_t _mck = GPIO_NUM_NC ;
        gpio_num_t _bck = GPIO_NUM_NC ;
        gpio_num_t _ws = GPIO_NUM_NC ;
        gpio_num_t _dout = GPIO_NUM_NC ;
        gpio_num_t _din = GPIO_NUM_NC ;

        i2s_mode_t mode = (i2s_mode_t)0 ;
        SemaphoreHandle_t sema = nullptr ;

        static I2S * i2s0 ;
        #if SOC_I2S_NUM > 1
        static I2S * i2s1 ;
        #endif

    public:
        I2S(JSContext * ctx, i2s_port_t busnum) ;
        ~I2S() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static I2S * flyweight(JSContext *, i2s_port_t) ;

        inline void take() {
            if(!sema) {
                return ;
            }
            xSemaphoreTake(sema, portMAX_DELAY) ;
        }
        inline void give() {
            if(!sema) {
                return ;
            }
            xSemaphoreGive(sema) ;
        }
        void enableMutex() ;

        i2s_port_t number() const ;

        bool isInstalled() ;

        size_t write(const void * data, size_t size, uint32_t timeout_ms=1000) ;
        size_t read(void * buff, size_t size, uint32_t timeout_ms=1000) ;

        /**
         * 设置 i2s 外设, 若遇到错误则抛出异常
         *
         *     {
         *         bck: number,
         *         ws: number,
         *         dout: number,
         *         din: number,
         *         mck: number,
         *         role: number = 0      // 0=master, 1=slave
         *         rate: number = 44100
         *         bits: number = 16     // 8/16/24/32
         *         channels: number = 2  // 1=mono, 2=stereo
         *         fmt: number = 0       // 0=philips, 1=msb, 2=pcm
         *         dma_desc_num: number = 6
         *         dma_frame_num: number = 240
         *     }
         *
         * @module serial
         * @class LegacyI2S
         * @method setup
         *
         * @param options:object
         * @return bool
         *
         */
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue preload(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue enableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue zeroTxBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}

#endif

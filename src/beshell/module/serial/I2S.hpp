#pragma once
#include "sdkconfig.h"
#if !CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY

#include <vector>
#include "../../NativeClass.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "hal/i2s_types.h"
#include "soc/soc_caps.h"
#include "../../debug.h"

namespace be {

    /**
     * 该类的构造函数没有绑定给 JS , 无法从 JS 创建实例。
     *
     * 模块 [serial](overview.md) 创建了和硬件对应的 I2S 实例，`import serial` 即可。
     *
     * @module serial
     * @class I2S
     */
    class I2S: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        I2S(JSContext * ctx, i2s_port_t busnum) ;
        ~I2S() ;

        inline void take() {
            if(!sema) {
                sema = xSemaphoreCreateMutex();
            }
            if(sema) {
                xSemaphoreTake(sema, portMAX_DELAY);
            }
        }
        inline void give() {
            if(sema) {
                xSemaphoreGive(sema);
            }
        }

        static void s_take();
        static void s_give();

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static I2S * flyweight(JSContext *, i2s_port_t) ;
        // 返回已创建的 flyweight 实例（不会创建新实例），供 C 代码桥接使用
        static I2S * sharedInstance(i2s_port_t busnum) ;

        i2s_port_t number() const ;
        i2s_chan_handle_t txHandle() const ;
        i2s_chan_handle_t rxHandle() const ;

        bool isInstalled() const ;

        size_t write(const void * data, size_t size, uint32_t timeout_ms=1000) ;
        size_t read(void * buff, size_t size, uint32_t timeout_ms=1000) ;
        size_t preload(const void * data, size_t size, uint32_t timeout_ms=1000) ;

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

    private:
        i2s_port_t busnum ;

        i2s_chan_handle_t tx_handle = nullptr ;
        i2s_chan_handle_t rx_handle = nullptr ;

        // setup 时估算的 TX DMA 缓冲区总字节数（用于 zeroTxBuffer，
        // 兼容旧版 IDF —— 5.3 之前 i2s_chan_info_t 没有 total_dma_buf_size 字段）
        uint32_t _dma_buf_size = 0 ;

        gpio_num_t _mck = GPIO_NUM_NC ;
        gpio_num_t _bck = GPIO_NUM_NC ;
        gpio_num_t _ws = GPIO_NUM_NC ;
        gpio_num_t _dout = GPIO_NUM_NC ;
        gpio_num_t _din = GPIO_NUM_NC ;

        SemaphoreHandle_t sema = nullptr ;

        static I2S * i2s0 ;
        #if SOC_I2S_NUM > 1
        static I2S * i2s1 ;
        #endif

        static SemaphoreHandle_t s_flyweightMutex ;

        void cleanupChannels() ;
    } ;
}

#endif

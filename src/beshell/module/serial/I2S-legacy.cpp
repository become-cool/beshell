#include "sdkconfig.h"
#if CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY

#include "I2S-legacy.hpp"
#include "qjs_utils.h"
#include <JSEngine.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <malloc.h>

using namespace std ;

namespace be {

    /**
     * I2S 总线类（Legacy 实现）
     *
     * 基于 ESP-IDF 旧版 I2S 驱动（`driver/i2s.h`）。
     * 通过 Kconfig 选项 `BESHELL_SERIAL_I2S_USE_LEGACY` 启用。
     *
     * @class LegacyI2S
     * @module serial
     * @extends NativeClass
     */
    DEFINE_NCLASS_META(I2S, NativeClass)

    I2S * I2S::i2s0 = nullptr ;
    #if SOC_I2S_NUM > 1
    I2S * I2S::i2s1 = nullptr ;
    #endif

    std::vector<JSCFunctionListEntry> I2S::methods = {
        JS_CFUNC_DEF("setup", 1, I2S::setup),
        JS_CFUNC_DEF("unsetup", 0, I2S::unsetup),
        JS_CFUNC_DEF("isInstalled", 0, I2S::isInstalled),
        JS_CFUNC_DEF("write", 1, I2S::write),
        JS_CFUNC_DEF("read", 1, I2S::read),
        JS_CFUNC_DEF("preload", 1, I2S::preload),
        JS_CFUNC_DEF("enableTx", 0, I2S::enableTx),
        JS_CFUNC_DEF("disableTx", 0, I2S::disableTx),
        JS_CFUNC_DEF("enableRx", 0, I2S::enableRx),
        JS_CFUNC_DEF("disableRx", 0, I2S::disableRx),
        JS_CFUNC_DEF("zeroTxBuffer", 0, I2S::zeroTxBuffer),
    } ;

    I2S::I2S(JSContext * ctx, i2s_port_t busnum)
        : NativeClass(ctx, build(ctx))
        , busnum(busnum)
    {
        enableMutex() ;
    }

    I2S::~I2S() {
        vSemaphoreDelete(sema) ;
    }

    JSValue I2S::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        i2s_port_t busnum = I2S_NUM_0 ;
        if(argc>0) {
            JS_ToUint32(ctx, (uint32_t*)&busnum, argv[0]) ;
        }
        auto obj = new I2S(ctx,busnum) ;
        obj->self = std::shared_ptr<I2S> (obj) ;
        return obj->jsobj ;
    }

    void I2S::enableMutex() {
        if(!sema) {
            sema = xSemaphoreCreateMutex() ;
        }
    }

    I2S * I2S::flyweight(JSContext * ctx, i2s_port_t bus) {
        if(bus==I2S_NUM_0) {
            if(!i2s0) {
                i2s0 = new I2S(ctx, I2S_NUM_0) ;
            }
            return i2s0 ;
        }
        #if SOC_I2S_NUM > 1
        else if(bus==I2S_NUM_1) {
            if(!i2s1) {
                i2s1 = new I2S(ctx, I2S_NUM_1) ;
            }
            return i2s1 ;
        }
        #endif
        return nullptr ;
    }

    i2s_port_t I2S::number() const {
        return busnum ;
    }

    bool I2S::isInstalled() {
        return installed ;
    }

    size_t I2S::write(const void * data, size_t size, uint32_t timeout_ms) {
        if(!installed || !data || size==0) {
            return 0 ;
        }
        size_t written = 0 ;
        i2s_write(busnum, data, size, &written, pdMS_TO_TICKS(timeout_ms)) ;
        return written ;
    }

    size_t I2S::read(void * buff, size_t size, uint32_t timeout_ms) {
        if(!installed || !buff || size==0) {
            return 0 ;
        }
        size_t readn = 0 ;
        i2s_read(busnum, buff, size, &readn, pdMS_TO_TICKS(timeout_ms)) ;
        return readn ;
    }

    /**
     * 配置并启动 I2S 总线。
     *
     * 安装 I2S 驱动，配置时钟、引脚、数据格式等参数。
     * 给出 `dout` 时启用 TX（发送/播放）；给出 `din` 时启用 RX（接收/录音）。
     * 两者可同时给出，共用同一组时钟。
     *
     * options 参数说明：
     * ```js
     * {
     *     bck: number,                // BCK 位时钟引脚（必需）
     *     ws: number,                 // WS(LRCK) 字时钟引脚（必需）
     *     dout: number,               // 数据输出引脚（播放时必需）
     *     din: number,                // 数据输入引脚（录音时必需）
     *     mck: number,                // MCK 主时钟引脚（可选，默认不输出）
     *     role: number = 0,           // 角色：0=主机（默认）, 1=从机
     *     rate: number = 44100,       // 采样率（Hz）
     *     bits: number = 16,          // 数据位宽：8/16/24/32
     *     channels: number = 2,       // 声道数：1=单声道, 2=立体声
     *     fmt: number = 0,            // 格式：0=Philips(I2S 标准), 1=MSB, 2=PCM(短帧)
     *     dma_desc_num: number = 6,   // DMA 描述符数量
     *     dma_frame_num: number = 240 // 每个描述符的帧数
     * }
     * ```
     *
     * @module serial
     * @class I2S
     * @function setup
     * @param options:object 配置选项对象
     * @return bool 成功返回 true
     * @throws 缺少 bck/ws，或 dout 与 din 均未给出
     * @throws bits/channels/fmt/role 参数无效
     * @throws I2S 驱动安装或引脚配置失败
     */
    JSValue I2S::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        ASSERT_ARGC(1)

        if(!JS_IsObject(argv[0])) {
            JSTHROW("setup options must be an object")
        }

        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "mck", mck, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "bck", bck, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "ws", ws, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "dout", dout, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "din", din, GPIO_NUM_NC)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "rate", rate, 44100)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "bits", bits, 16)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "channels", channels, 2)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "role", role, 0)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "fmt", fmt, 0)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "dma_desc_num", dma_desc_num, 6)
        uint32_t GET_UINT32_PROP_OPT(argv[0], "dma_frame_num", dma_frame_num, 240)

        if(bck==GPIO_NUM_NC || ws==GPIO_NUM_NC) {
            JSTHROW("property bck and ws are required")
        }
        if(dout==GPIO_NUM_NC && din==GPIO_NUM_NC) {
            JSTHROW("at least one of dout or din is required")
        }
        if(bits!=8 && bits!=16 && bits!=24 && bits!=32) {
            JSTHROW("invalid bits: %d (must be 8/16/24/32)", bits)
        }
        if(channels!=1 && channels!=2) {
            JSTHROW("invalid channels: %d (must be 1 or 2)", channels)
        }
        if(role>1) {
            JSTHROW("invalid role: %d (0=master, 1=slave)", role)
        }
        if(fmt>2) {
            JSTHROW("invalid fmt: %d (0=philips, 1=msb, 2=pcm)", fmt)
        }

        i2s_mode_t mode = (i2s_mode_t)( role? I2S_MODE_SLAVE: I2S_MODE_MASTER ) ;
        if(dout!=GPIO_NUM_NC) {
            mode = (i2s_mode_t)( mode | I2S_MODE_TX ) ;
        }
        if(din!=GPIO_NUM_NC) {
            mode = (i2s_mode_t)( mode | I2S_MODE_RX ) ;
        }

        i2s_comm_format_t comm_fmt = I2S_COMM_FORMAT_STAND_I2S ;
        if(fmt==1) {
            comm_fmt = I2S_COMM_FORMAT_STAND_MSB ;
        }
        else if(fmt==2) {
            comm_fmt = I2S_COMM_FORMAT_STAND_PCM_SHORT ;
        }

        i2s_config_t i2s_config = {
            .mode = mode,
            .sample_rate = rate,
            .bits_per_sample = (i2s_bits_per_sample_t)bits,
            .channel_format = channels==1? I2S_CHANNEL_FMT_ONLY_LEFT: I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = comm_fmt,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = (int)dma_desc_num,
            .dma_buf_len = (int)dma_frame_num,
            .use_apll = false,
            .tx_desc_auto_clear = true,
            .fixed_mclk = 0
        } ;

        i2s_pin_config_t pin_config = {
            .mck_io_num = mck,
            .bck_io_num = bck,
            .ws_io_num = ws,
            .data_out_num = dout,
            .data_in_num = din,
        } ;

        that->take();
        if(that->installed) {
            i2s_driver_uninstall(that->busnum) ;
            that->installed = false ;
        }

        esp_err_t res = i2s_driver_install(that->busnum, &i2s_config, 0, NULL) ;
        if(res==ESP_OK) {
            res = i2s_set_pin(that->busnum, &pin_config) ;
        }
        if(res!=ESP_OK) {
            that->give();
            JSTHROW("failed to setup i2s: %d", res)
        }

        that->mode = mode ;
        that->installed = true ;
        that->_mck = mck ;
        that->_bck = bck ;
        that->_ws = ws ;
        that->_dout = dout ;
        that->_din = din ;
        that->give();

        return JS_TRUE ;
    }

    /**
     * 关闭并释放 I2S 总线资源。
     *
     * 卸载 I2S 驱动，释放 DMA 资源。
     * 调用后 I2S 恢复为未初始化状态。
     *
     * @module serial
     * @class I2S
     * @function unsetup
     * @return bool 成功返回 true
     */
    JSValue I2S::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->installed) {
            that->give();
            return JS_TRUE ;
        }
        esp_err_t res = i2s_driver_uninstall(that->busnum) ;
        if(res==ESP_OK) {
            that->installed = false ;
        }
        that->give();
        return res==ESP_OK? JS_TRUE: JS_FALSE ;
    }

    /**
     * 检查 I2S 总线是否已初始化。
     *
     * @module serial
     * @class I2S
     * @function isInstalled
     * @return bool 已初始化返回 true，否则返回 false
     */
    JSValue I2S::isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        return that->isInstalled()? TRUE: FALSE ;
    }

    /**
     * 向 I2S 总线写入（播放）音频数据。
     *
     * 数据写入 DMA 缓冲区，由 I2S 硬件自动输出。
     * 采样格式由 setup() 的 rate/bits/channels/fmt 决定。
     *
     * @module serial
     * @class I2S
     * @function write
     * @param data:ArrayBuffer|number[] 要写入的 PCM 音频数据
     * @param timeout_ms:number=1000 超时时间（毫秒，默认 1000）
     * @return number 实际写入的字节数
     * @throws 参数数量不足
     * @throws data 不是 ArrayBuffer 或字节数组
     * @throws I2S 未初始化（未调用 setup）
     */
    JSValue I2S::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)

        uint8_t * data = nullptr ;
        size_t len = 0 ;
        bool need_free = false ;
        if(JS_IsArray(ctx, argv[0])) {
            int ilen = 0 ;
            data = JS_ArrayToBufferUint8(ctx, argv[0], &ilen) ;
            len = (size_t)ilen ;
            need_free = true ;
        } else {
            data = (uint8_t *)JS_GetArrayBuffer(ctx, &len, argv[0]) ;
        }
        if(!data || len==0) {
            if(need_free && data) {
                free(data) ;
            }
            JSTHROW("data must be an ArrayBuffer or array of bytes")
        }

        ARGV_TO_UINT32_OPT(1, timeout_ms, 1000)

        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->installed) {
            that->give();
            if(need_free) {
                free(data) ;
            }
            JSTHROW("i2s is not installed")
        }
        size_t written = that->write(data, len, timeout_ms) ;
        that->give();

        if(need_free) {
            free(data) ;
        }
        return JS_NewUint32(ctx, written) ;
    }

    /**
     * 从 I2S 总线读取（录制）音频数据。
     *
     * 从 DMA 缓冲区读取指定字节数的数据。
     * 采样格式由 setup() 的 rate/bits/channels/fmt 决定。
     *
     * @module serial
     * @class I2S
     * @function read
     * @param length:number 要读取的字节数
     * @param timeout_ms:number=1000 超时时间（毫秒，默认 1000）
     * @return ArrayBuffer|null 读取到的数据，失败返回 null
     * @throws 参数数量不足
     * @throws 读取长度无效
     * @throws 内存分配失败
     * @throws I2S 未初始化（未调用 setup）
     */
    JSValue I2S::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, len)
        ARGV_TO_UINT32_OPT(1, timeout_ms, 1000)
        if(len < 1) {
            JSTHROW("invalid read length")
        }
        uint8_t * buffer = (uint8_t*)malloc(len) ;
        if(!buffer) {
            JSTHROW("out of memory?")
        }

        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->installed) {
            that->give();
            free(buffer);
            JSTHROW("i2s is not installed")
        }
        size_t readn = that->read(buffer, len, timeout_ms) ;
        that->give();

        if(readn < 1) {
            free(buffer) ;
            return JS_NULL ;
        }
        return JS_NewArrayBuffer(ctx, buffer, readn, freeArrayBuffer, NULL, false) ;
    }

    /**
     * 预加载数据到 DMA 缓冲区。
     *
     * 在播放开始前预填充 DMA 缓冲区，避免播放开始时的爆音。
     * legacy 驱动没有独立的预加载 API，本方法等效于 write()。
     *
     * @module serial
     * @class I2S
     * @function preload
     * @param data:ArrayBuffer|number[] 要预加载的 PCM 数据
     * @param timeout_ms:number=1000 超时时间（毫秒，默认 1000）
     * @return number 实际写入的字节数
     * @throws 参数数量不足
     * @throws data 不是 ArrayBuffer 或字节数组
     * @throws I2S 未初始化（未调用 setup）
     */
    JSValue I2S::preload(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        // legacy 驱动无预加载 API，直接写入 DMA 缓冲区
        return write(ctx, this_val, argc, argv) ;
    }

    /**
     * 使能 TX（发送/播放）通道。
     *
     * legacy 驱动不支持单独启停 TX/RX，本方法为空实现，
     * 仅为与新版驱动的 JS 接口保持兼容。
     *
     * @module serial
     * @class I2S
     * @function enableTx
     * @return undefined
     */
    // legacy 驱动不支持单独启停 TX/RX，保留同名 API 的空实现以兼容 JS 接口
    JSValue I2S::enableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    /**
     * 禁用 TX（发送/播放）通道。
     *
     * legacy 驱动不支持单独启停 TX/RX，本方法为空实现，
     * 仅为与新版驱动的 JS 接口保持兼容。
     *
     * @module serial
     * @class I2S
     * @function disableTx
     * @return undefined
     */
    JSValue I2S::disableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    /**
     * 使能 RX（接收/录音）通道。
     *
     * legacy 驱动不支持单独启停 TX/RX，本方法为空实现，
     * 仅为与新版驱动的 JS 接口保持兼容。
     *
     * @module serial
     * @class I2S
     * @function enableRx
     * @return undefined
     */
    JSValue I2S::enableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    /**
     * 禁用 RX（接收/录音）通道。
     *
     * legacy 驱动不支持单独启停 TX/RX，本方法为空实现，
     * 仅为与新版驱动的 JS 接口保持兼容。
     *
     * @module serial
     * @class I2S
     * @function disableRx
     * @return undefined
     */
    JSValue I2S::disableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    /**
     * 将 DMA 缓冲区清零。
     *
     * 清零后播放静音数据，避免残留旧音频。
     * legacy 驱动通过 `i2s_zero_dma_buffer()` 实现，无需先禁用通道。
     *
     * @module serial
     * @class I2S
     * @function zeroTxBuffer
     * @return bool 成功返回 true
     * @throws I2S 未初始化（未调用 setup）
     */
    JSValue I2S::zeroTxBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->installed) {
            that->give();
            JSTHROW("i2s is not installed")
        }
        esp_err_t res = i2s_zero_dma_buffer(that->busnum) ;
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

}

#endif

#include "sdkconfig.h"
#if !CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY

#include "driver/i2s_std.h"

#include "I2S.hpp"
#include "qjs_utils.h"
#include <JSEngine.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <malloc.h>

using namespace std ;

namespace be {

    /**
     * I2S 总线类
     *
     * 用于配置和管理 ESP32 的 I2S 音频总线。I2S 是数字音频传输协议，
     * 常用于连接音频编解码芯片（DAC/ADC）、麦克风、功放等外设。
     *
     * ESP32 通常有 2 个 I2S 总线（I2S0、I2S1），部分型号只有 1 个。
     * serial 模块会自动创建 I2S 实例并通过 `i2s0`, `i2s1` 等导出。
     * 用户直接通过 serial 模块访问这些实例，无需手动创建。
     *
     * **不同芯片型号导出的 I2S 对象不同**：
     * - ESP32/ESP32-S3/ESP32-P4：i2s0, i2s1（2 个 I2S）
     * - ESP32-S2/C2/C3/C6/H2：i2s0（只有 1 个 I2S）
     *
     * 当前实现基于 ESP-IDF 新版 I2S 驱动（`driver/i2s_std.h`），
     * 仅支持标准（STD）模式，不支持 PDM/TDM。
     *
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     *
     * // 访问 I2S0 总线实例（推荐）
     * const i2s = serial.i2s0
     *
     * // 配置 I2S 总线（连接 MAX98357A 功放）
     * i2s.setup({
     *     bck: 26,   // BCK 位时钟引脚
     *     ws: 25,    // WS(LRCK) 字时钟引脚
     *     dout: 22,  // 数据输出引脚（播放必需）
     *     rate: 44100,
     *     bits: 16,
     *     channels: 2
     * })
     *
     * // 播放音频数据
     * i2s.write(pcmBuffer)
     *
     * // 释放
     * i2s.unsetup()
     * ```
     *
     * @class I2S
     * @module serial
     * @extends NativeClass
     */
    DEFINE_NCLASS_META(I2S, NativeClass)

    I2S * I2S::i2s0 = nullptr ;
    #if SOC_I2S_NUM > 1
    I2S * I2S::i2s1 = nullptr ;
    #endif

    SemaphoreHandle_t I2S::s_flyweightMutex = nullptr ;

    void I2S::s_take() {
        if(!s_flyweightMutex) {
            static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
            portENTER_CRITICAL(&mux);
            if(!s_flyweightMutex) {
                s_flyweightMutex = xSemaphoreCreateMutex();
            }
            portEXIT_CRITICAL(&mux);
        }
        if(s_flyweightMutex) {
            xSemaphoreTake(s_flyweightMutex, portMAX_DELAY);
        }
    }

    void I2S::s_give() {
        if(s_flyweightMutex) {
            xSemaphoreGive(s_flyweightMutex);
        }
    }

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
    }

    I2S::~I2S() {
        cleanupChannels();
        if(sema) {
            vSemaphoreDelete(sema);
            sema = nullptr;
        }
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

    I2S * I2S::flyweight(JSContext * ctx, i2s_port_t bus) {
        I2S * result = nullptr;
        s_take();
        if(bus==I2S_NUM_0) {
            if(!i2s0) {
                i2s0 = new I2S(ctx, I2S_NUM_0) ;
            }
            result = i2s0 ;
        }
        #if SOC_I2S_NUM > 1
        else if(bus==I2S_NUM_1) {
            if(!i2s1) {
                i2s1 = new I2S(ctx, I2S_NUM_1) ;
            }
            result = i2s1 ;
        }
        #endif
        s_give();
        return result ;
    }

    i2s_port_t I2S::number() const {
        return busnum ;
    }
    i2s_chan_handle_t I2S::txHandle() const {
        return tx_handle ;
    }
    i2s_chan_handle_t I2S::rxHandle() const {
        return rx_handle ;
    }

    bool I2S::isInstalled() const {
        return tx_handle != nullptr || rx_handle != nullptr ;
    }

    void I2S::cleanupChannels() {
        if(tx_handle) {
            i2s_channel_disable(tx_handle);
            i2s_del_channel(tx_handle);
            tx_handle = nullptr;
        }
        if(rx_handle) {
            i2s_channel_disable(rx_handle);
            i2s_del_channel(rx_handle);
            rx_handle = nullptr;
        }
    }

    /**
     * 配置并启动 I2S 总线。
     *
     * 初始化 I2S 通道，配置时钟、引脚、数据格式等参数。
     * 给出 `dout` 时创建 TX（发送/播放）通道；给出 `din` 时创建 RX（接收/录音）通道。
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
     *     mclk_multiple: number = 256,// MCLK 倍频（仅主机模式有效）
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
     * @throws I2S 通道创建或初始化失败
     */
    JSValue I2S::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(I2S, that)

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
        uint32_t GET_UINT32_PROP_OPT(argv[0], "mclk_multiple", mclk_multiple, 256)
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

        i2s_chan_handle_t new_tx = nullptr ;
        i2s_chan_handle_t new_rx = nullptr ;
        esp_err_t res = ESP_OK ;

        i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(that->busnum, role? I2S_ROLE_SLAVE: I2S_ROLE_MASTER) ;
        chan_cfg.dma_desc_num = dma_desc_num ;
        chan_cfg.dma_frame_num = dma_frame_num ;

        res = i2s_new_channel(&chan_cfg,
            dout!=GPIO_NUM_NC? &new_tx: nullptr,
            din!=GPIO_NUM_NC? &new_rx: nullptr) ;
        if(res != ESP_OK) {
            JSTHROW("failed to create i2s channel: %d", res)
        }

        i2s_std_slot_config_t slot_cfg ;
        i2s_slot_mode_t slot_mode = channels==1? I2S_SLOT_MODE_MONO: I2S_SLOT_MODE_STEREO ;
        if(fmt==1) {
            slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bits, slot_mode) ;
        }
        else if(fmt==2) {
            slot_cfg = I2S_STD_PCM_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bits, slot_mode) ;
        }
        else {
            slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bits, slot_mode) ;
        }

        i2s_std_config_t std_cfg = {
            .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(rate),
            .slot_cfg = slot_cfg,
            .gpio_cfg = {
                .mclk = mck,
                .bclk = bck,
                .ws = ws,
                .dout = dout,
                .din = din,
                .invert_flags = {
                    .mclk_inv = false,
                    .bclk_inv = false,
                    .ws_inv = false,
                },
            },
        } ;
        std_cfg.clk_cfg.mclk_multiple = (i2s_mclk_multiple_t)mclk_multiple ;

        if(new_tx) {
            res = i2s_channel_init_std_mode(new_tx, &std_cfg) ;
            if(res == ESP_OK) {
                res = i2s_channel_enable(new_tx) ;
            }
            if(res != ESP_OK) {
                i2s_del_channel(new_tx) ;
                if(new_rx) {
                    i2s_del_channel(new_rx) ;
                }
                JSTHROW("failed to init i2s tx channel: %d", res)
            }
        }
        if(new_rx) {
            res = i2s_channel_init_std_mode(new_rx, &std_cfg) ;
            if(res == ESP_OK) {
                res = i2s_channel_enable(new_rx) ;
            }
            if(res != ESP_OK) {
                if(new_tx) {
                    i2s_channel_disable(new_tx) ;
                    i2s_del_channel(new_tx) ;
                }
                i2s_del_channel(new_rx) ;
                JSTHROW("failed to init i2s rx channel: %d", res)
            }
        }

        that->take();
        that->cleanupChannels();
        that->_mck = mck ;
        that->_bck = bck ;
        that->_ws = ws ;
        that->_dout = dout ;
        that->_din = din ;
        // 24bit 采样在 DMA 中占 4 字节容器
        that->_dma_buf_size = dma_desc_num * dma_frame_num * channels * (bits>=24? 4: bits/8) ;
        that->tx_handle = new_tx ;
        that->rx_handle = new_rx ;
        that->give();

        return JS_TRUE ;
    }

    /**
     * 关闭并释放 I2S 总线资源。
     *
     * 禁用并删除 TX/RX 通道，释放 DMA 资源。
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
        that->cleanupChannels();
        that->give();
        return JS_TRUE ;
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
        that->take();
        bool installed = that->isInstalled();
        that->give();
        return installed ? JS_TRUE : JS_FALSE ;
    }

    size_t I2S::write(const void * data, size_t size, uint32_t timeout_ms) {
        if(!tx_handle || !data || size==0) {
            return 0 ;
        }
        size_t written = 0 ;
        i2s_channel_write(tx_handle, data, size, &written, pdMS_TO_TICKS(timeout_ms)) ;
        return written ;
    }

    size_t I2S::read(void * buff, size_t size, uint32_t timeout_ms) {
        if(!rx_handle || !buff || size==0) {
            return 0 ;
        }
        size_t readn = 0 ;
        i2s_channel_read(rx_handle, buff, size, &readn, pdMS_TO_TICKS(timeout_ms)) ;
        return readn ;
    }

    size_t I2S::preload(const void * data, size_t size, uint32_t timeout_ms) {
        if(!tx_handle || !data || size==0) {
            return 0 ;
        }
        size_t preloaded = 0 ;
        i2s_channel_preload_data(tx_handle, data, size, &preloaded) ;
        (void)timeout_ms ; // IDF 5.x 各版本的 i2s_channel_preload_data 均无超时参数
        return preloaded ;
    }

    /**
     * 向 I2S 总线写入（播放）音频数据。
     *
     * 数据写入 TX 通道的 DMA 缓冲区，由 I2S 硬件自动输出。
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
     * @throws TX 通道未初始化（setup 时未提供 dout）
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
        if(!that->tx_handle) {
            that->give();
            if(need_free) {
                free(data) ;
            }
            JSTHROW("i2s tx channel is not installed")
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
     * 从 RX 通道的 DMA 缓冲区读取指定字节数的数据。
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
     * @throws RX 通道未初始化（setup 时未提供 din）
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
        if(!that->rx_handle) {
            that->give();
            free(buffer);
            JSTHROW("i2s rx channel is not installed")
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
     * 预加载数据到 TX 通道的 DMA 缓冲区。
     *
     * 在通道启动前预填充 DMA 缓冲区，避免播放开始时的爆音。
     * 通常在 setup() 之后、disableTx() 状态下调用，随后再 enableTx()。
     *
     * @module serial
     * @class I2S
     * @function preload
     * @param data:ArrayBuffer|number[] 要预加载的 PCM 数据
     * @param timeout_ms:number=1000 超时时间（毫秒，默认 1000）
     * @return number 实际预加载的字节数
     * @throws 参数数量不足
     * @throws data 不是 ArrayBuffer 或字节数组
     * @throws TX 通道未初始化
     */
    JSValue I2S::preload(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
        if(!that->tx_handle) {
            that->give();
            if(need_free) {
                free(data) ;
            }
            JSTHROW("i2s tx channel is not installed")
        }
        size_t preloaded = that->preload(data, len, timeout_ms) ;
        that->give();

        if(need_free) {
            free(data) ;
        }
        return JS_NewUint32(ctx, preloaded) ;
    }

    /**
     * 使能 TX（发送/播放）通道。
     *
     * @module serial
     * @class I2S
     * @function enableTx
     * @return bool 成功返回 true
     * @throws TX 通道未初始化
     */
    JSValue I2S::enableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->tx_handle) {
            that->give();
            JSTHROW("i2s tx channel is not installed")
        }
        esp_err_t res = i2s_channel_enable(that->tx_handle) ;
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

    /**
     * 禁用 TX（发送/播放）通道。
     *
     * @module serial
     * @class I2S
     * @function disableTx
     * @return bool 成功返回 true
     * @throws TX 通道未初始化
     */
    JSValue I2S::disableTx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->tx_handle) {
            that->give();
            JSTHROW("i2s tx channel is not installed")
        }
        esp_err_t res = i2s_channel_disable(that->tx_handle) ;
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

    /**
     * 使能 RX（接收/录音）通道。
     *
     * @module serial
     * @class I2S
     * @function enableRx
     * @return bool 成功返回 true
     * @throws RX 通道未初始化
     */
    JSValue I2S::enableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->rx_handle) {
            that->give();
            JSTHROW("i2s rx channel is not installed")
        }
        esp_err_t res = i2s_channel_enable(that->rx_handle) ;
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

    /**
     * 禁用 RX（接收/录音）通道。
     *
     * @module serial
     * @class I2S
     * @function disableRx
     * @return bool 成功返回 true
     * @throws RX 通道未初始化
     */
    JSValue I2S::disableRx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->rx_handle) {
            that->give();
            JSTHROW("i2s rx channel is not installed")
        }
        esp_err_t res = i2s_channel_disable(that->rx_handle) ;
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

    /**
     * 将 TX 通道的 DMA 缓冲区清零。
     *
     * 需要在 TX 通道禁用（disableTx）状态下调用。
     * 清零后再 enableTx，播放静音数据，避免残留旧音频。
     *
     * @module serial
     * @class I2S
     * @function zeroTxBuffer
     * @return bool 成功返回 true
     * @throws TX 通道未初始化
     */
    JSValue I2S::zeroTxBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2S, that)
        that->take();
        if(!that->tx_handle) {
            that->give();
            JSTHROW("i2s tx channel is not installed")
        }
        // 在通道禁用状态下 preload 一块全零数据填满 DMA 缓冲区，
        // 达到清零效果（兼容各版本 IDF ，不依赖 5.3+ 的 total_dma_buf_size 字段）
        esp_err_t res = ESP_OK ;
        if(that->_dma_buf_size>0) {
            void * zeros = calloc(1, that->_dma_buf_size) ;
            if(zeros) {
                size_t loaded = 0 ;
                res = i2s_channel_preload_data(that->tx_handle, zeros, that->_dma_buf_size, &loaded) ;
                free(zeros) ;
            } else {
                res = ESP_ERR_NO_MEM ;
            }
        }
        that->give();
        return res==ESP_OK ? JS_TRUE : JS_FALSE ;
    }

}

#endif

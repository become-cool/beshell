#include "driver/i2c_master.h"
#if !BESHELL_SERIAL_I2C_USE_LEGACY

#include "I2C.hpp"
#include "hal/i2c_types.h"
#include "qjs_utils.h"
#include "thread.hpp"
#include <JSEngine.hpp>
#include <esp_log.h>
#include <cstdlib>
#include <stdlib.h>
#include <malloc.h>
#include <cstdlib>
#include <climits>

using namespace std ;

#define JSCHECK_MASTER                              \
    if(that->mode!=I2C_MODE_MASTER) {               \
        JSTHROW("I2C is not in %s mode", "master") ;\
    }
#define JSCHECK_SLAVE                               \
    if(that->mode!=I2C_MODE_SLAVE) {                \
        JSTHROW("I2C is not in %s mode", "slave") ; \
    }
    
namespace be {

    /**
     * I2C 总线类
     * 
     * 用于配置和管理 ESP32 的 I2C 总线。I2C 是一种两线串行通信协议，
     * 常用于连接传感器、显示屏、EEPROM 等外设。
     * 
     * ESP32 通常有 2 个 I2C 总线（I2C0、I2C1），部分型号还有低功耗 I2C（I2C LP）。
     * serial 模块会自动创建 I2C 实例并通过 `i2c0`, `i2c1` 等导出。
     * 用户直接通过 serial 模块访问这些实例，无需手动创建。
     * 
     * **不同芯片型号导出的 I2C 对象不同**：
     * - ESP32/ESP32-S2/S3/H2/P4：i2c0, i2c1（2 个 I2C）
     * - ESP32-C2/C3/C6：i2c0（只有 1 个 I2C）
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 访问 I2C0 总线实例（推荐）
     * const i2c = serial.i2c0
     * 
     * // 配置 I2C 总线
     * i2c.setup({
     *     sda: 21,  // SDA 引脚
     *     scl: 22,  // SCL 引脚
     *     freq: 400000  // 时钟频率 400kHz
     * })
     * 
     * // 扫描总线上的设备
     * i2c.scan()
     * 
     * // 添加设备配置
     * i2c.addDevice({
     *     addr: 0x68,  // 设备地址（如 MPU6050）
     *     freq: 100000
     * })
     * 
     * // 读写寄存器
     * i2c.write8(0x68, 0x6B, 0x00)  // 向地址 0x68 的寄存器 0x6B 写入 0x00
     * const data = i2c.readU8(0x68, 0x75)  // 从寄存器 0x75 读取数据
     * console.log("Who am I:", data)
     * ```
     * 
     * @class I2C
     * @module serial
     * @extends NativeClass
     */
    DEFINE_NCLASS_META(I2C, NativeClass)

    I2C * I2C::i2c0 = nullptr ;
    #if SOC_HP_I2C_NUM > 1
    I2C * I2C::i2c1 = nullptr ;
    #endif
    #if SOC_LP_I2C_NUM > 0
    I2C * I2C::i2clp0 = nullptr ;
    #endif

    SemaphoreHandle_t I2C::s_flyweightMutex = nullptr ;

    void I2C::s_take() {
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

    void I2C::s_give() {
        if(s_flyweightMutex) {
            xSemaphoreGive(s_flyweightMutex);
        }
    }

    std::vector<JSCFunctionListEntry> I2C::methods = {
        JS_CFUNC_DEF("setup", 1, I2C::setup),
        JS_CFUNC_DEF("unsetup", 1, I2C::unsetup),
        JS_CFUNC_DEF("isInstalled", 1, I2C::isInstalled),
        JS_CFUNC_DEF("addDevice", 1, I2C::addDevice),
        JS_CFUNC_DEF("removeDevice", 1, I2C::removeDevice),
        JS_CFUNC_DEF("ping", 1, I2C::ping),
        JS_CFUNC_DEF("scan", 1, I2C::scan),
        JS_CFUNC_DEF("send", 2, I2C::send),
        JS_CFUNC_DEF("recv", 2, I2C::recv),
        JS_CFUNC_DEF("recvU8", 2, I2C::recvU8),
        
        JS_CFUNC_DEF("write8", 2, I2C::write8),
        JS_CFUNC_DEF("write16", 2, I2C::write16),
        JS_CFUNC_DEF("write32", 2, I2C::write32),
        JS_CFUNC_DEF("readI8", 2, I2C::readI8),
        JS_CFUNC_DEF("readI16", 2, I2C::readI16),
        JS_CFUNC_DEF("readI32", 2, I2C::readI32),
        JS_CFUNC_DEF("readU8", 2, I2C::readU8),
        JS_CFUNC_DEF("readU16", 2, I2C::readU16),
        JS_CFUNC_DEF("readU32", 2, I2C::readU32),

        #if SOC_I2C_SUPPORT_SLAVE
        // JS_CFUNC_DEF("listen", 2, I2C::listen),
        // JS_CFUNC_DEF("slaveWrite", 2, I2C::slaveWrite),
        // JS_CFUNC_DEF("slaveWriteReg", 2, I2C::slaveWriteReg),
        // JS_CFUNC_DEF("slaveWriteBuff", 2, I2C::slaveWriteBuff),
        // JS_CFUNC_DEF("slaveReadBuff", 2, I2C::slaveReadBuff),
        #endif
    } ;

    I2C::I2C(JSContext * ctx, i2c_port_t busnum)
        : NativeClass(ctx, build(ctx))
        , busnum(busnum)
    {
    }
    
    I2C::~I2C() {
        if(sema) {
            vSemaphoreDelete(sema);
            sema = nullptr;
        }
    }
    
    JSValue I2C::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        i2c_port_t busnum = I2C_NUM_0 ;
        if(argc>0) {
            JS_ToUint32(ctx, (uint32_t*)&busnum, argv[0]) ;
        }
        auto obj = new I2C(ctx,busnum) ;
        obj->self = std::shared_ptr<I2C> (obj) ;
        return obj->jsobj ;
    }
    
    I2C * I2C::flyweight(JSContext * ctx, i2c_port_t bus) {
        I2C * result = nullptr;
        s_take();
        if(bus==I2C_NUM_0) {
            if(!i2c0) {
                i2c0 = new I2C(ctx, I2C_NUM_0) ;
            }
            result = i2c0 ;
        }
        #if SOC_HP_I2C_NUM > 1
        else if(bus==I2C_NUM_1) {
            if(!i2c1) {
                i2c1 = new I2C(ctx, I2C_NUM_1) ;
            }
            result = i2c1 ;
        }
        #endif
        #if SOC_LP_I2C_NUM > 0
        else if(bus==LP_I2C_NUM_0) {
            if(!i2clp0) {
                i2clp0 = new I2C(ctx, LP_I2C_NUM_0) ;
            }
            result = i2clp0 ;
        }
        #endif
        s_give();
        return result ;
    }

    
    bool I2C::addDevice(i2c_device_config_t & config, uint8_t regAddrBits, uint8_t regBits) {
        
        if (!isInstalled()) {
            return false;
        }
        
        uint16_t addr_key = (config.dev_addr_length==I2C_ADDR_BIT_LEN_10)? (config.device_address | (30<<10)) : config.device_address ;
        if(devices.count(addr_key)>0) {
            devices[addr_key].addr = config.device_address ;
            devices[addr_key].addr_len = config.dev_addr_length ;
            devices[addr_key].reg_addr_bits = regAddrBits ;
            devices[addr_key].reg_bits = regBits ;

            return true;
        }

        else {
            inner::i2c_device_memo_t dev ;
            dev.addr = config.device_address ;
            dev.addr_len = config.dev_addr_length ;
            dev.reg_addr_bits = regAddrBits ;
            dev.reg_bits = regBits ;
        
            esp_err_t res = i2c_master_bus_add_device(bus_handle, &config, &dev.dev_handle);
            if (res != ESP_OK) {
                return false ;
            }
            
            devices[addr_key] = dev;
            return true ;
        }
    }


    bool I2C::addDevice(JSContext *ctx, JSValue devOption) {

        if(!bus_handle || mode!=I2C_MODE_MASTER) {
            JS_ThrowReferenceError(ctx, "I2C bus is not initialized in master mode") ;
            return false;
        }

        i2c_device_config_t devConfig = {} ;
        GET_UINT_PROP(devOption, "addr", devConfig.device_address, uint16_t, {
            JS_ThrowReferenceError(ctx, "device address is required") ;
            return false ;
        })
        bool GET_BOOL_PROP_OPT(devOption, "addrBit10", addrBit10, false)
        GET_UINT32_PROP_OPT(devOption, "freq", devConfig.scl_speed_hz, 100000)
        GET_UINT32_PROP_OPT(devOption, "timeout", devConfig.scl_wait_us, 1000)
        GET_BOOL_PROP_OPT(devOption, "ackCheck", devConfig.flags.disable_ack_check, true)

        uint8_t GET_UINT8_PROP_OPT(devOption, "regAddrBits", regAddrBits, 8)
        uint8_t GET_UINT8_PROP_OPT(devOption, "regBits", regBits, regAddrBits)

        if(regAddrBits!=8 && regAddrBits!=16 && regAddrBits!=32) {
            JS_ThrowReferenceError(ctx, "invalid regBits: %d", regAddrBits) ;
            return false;
        }
        if(regBits!=8 && regBits!=16 && regBits!=32) {
            JS_ThrowReferenceError(ctx, "invalid regBits: %d", regBits) ;
            return false;
        }

        devConfig.dev_addr_length = addrBit10 ? I2C_ADDR_BIT_LEN_10 : I2C_ADDR_BIT_LEN_7 ;
        if(!addDevice(devConfig, regAddrBits, regBits)) {
            JS_ThrowReferenceError(ctx, "failed to add i2c device") ;
            return false ;
        }

        return true;
    }
    
    /**
     * 配置并启动 I2C 总线。
     * 
     * 初始化 I2C 总线，配置 SDA/SCL 引脚、主从模式等参数。
     * 当前仅支持主模式（master mode），从模式尚未实现。
     * 
     * options 参数说明：
     * ```js
     * {
     *     sda: number,              // SDA 引脚 GPIO 编号（必需）
     *     scl: number,              // SCL 引脚 GPIO 编号（必需）
     *     mode: number = 1,         // 模式：0=从机, 1=主机（默认主机）
     *     core: number = 0,         // 运行 I2C 驱动的 CPU 核心编号
     *     clk_source: number,       // 时钟源（默认 I2C_CLK_SRC_DEFAULT）
     *     glitch_ignore_cnt: number = 7,  // 毛刺过滤计数
     *     internal_pullup: bool = true,   // 使能内部上拉
     *     allow_pd: bool = false,         // 允许断电
     *     intr_priority: number = 0,      // 中断优先级
     *     trans_queue_depth: number = 0,  // 事务队列深度
     *     dev: [                      // 预配置设备数组（可选）
     *         {
     *             addr: number,           // 设备地址（必需）
     *             addrBit10: bool = false,    // 是否使用 10 位地址
     *             regAddrBits: number = 8,    // 寄存器地址位数（8/16/32）
     *             regBits: number = 8,        // 寄存器值位数（8/16/32）
     *             freq: number = 100000,      // 时钟频率（Hz）
     *             timeout: number = 1000,     // 超时时间（微秒）
     *             ackCheck: bool = true       // 是否启用 ACK 检查
     *         }
     *     ]
     * }
     * ```
     * 
     * @module serial
     * @class I2C
     * @function setup
     * @param options:object 配置选项对象
     * @return undefined
     * @throws 缺少 sda 或 scl 配置
     * @throws I2C 总线初始化失败
     * @throws 添加预配置设备失败
     * @throws 从模式不受支持
     */
    JSValue I2C::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(argc < 1) {
            JSTHROW("Missing param");
        }

        THIS_NCLASS(I2C, that)
        JSValue ret = JS_UNDEFINED;
        bool locked = false;
        esp_err_t res = ESP_OK;
        i2c_mode_t local_mode = I2C_MODE_MASTER;
        uint32_t clk_source = I2C_CLK_SRC_DEFAULT;
        uint32_t glitch_ignore_cnt = 7;
        bool enable_internal_pullup = true;
        bool allow_power_down = false;
        int intr_priority = 0;
        uint32_t trans_queue_depth = 0;
        uint8_t core = 0;
        i2c_master_bus_config_t bus_config = {};

        that->take();
        locked = true;

        auto cleanup_master_bus = [&]() {
            if(!that->devices.empty()) {
                for(auto &entry : that->devices) {
                    if(entry.second.dev_handle) {
                        i2c_master_bus_rm_device(entry.second.dev_handle);
                        entry.second.dev_handle = nullptr;
                    }
                }
                that->devices.clear();
            }
            if(that->bus_handle) {
                i2c_del_master_bus(that->bus_handle);
                that->bus_handle = nullptr;
            }
        };

        GET_GPIO_PROP(argv[0], "sda", that->_sda, {ret = JS_EXCEPTION; goto cleanup;})
        GET_GPIO_PROP(argv[0], "scl", that->_scl, {ret = JS_EXCEPTION; goto cleanup;})

        // Parse mode manually to avoid JSTHROW inside lock
        {
            JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "mode");
            if(!JS_IsUndefined(jsvar)) {
                if(!JS_IsNumber(jsvar)) {
                    JS_FreeValue(ctx, jsvar);
                    JS_ThrowReferenceError(ctx, "property mode is not a number");
                    ret = JS_EXCEPTION;
                    goto cleanup;
                }
                uint32_t tmp;
                JS_ToUint32(ctx, &tmp, jsvar);
                local_mode = (i2c_mode_t)tmp;
            }
            JS_FreeValue(ctx, jsvar);
        }

        if(local_mode == I2C_MODE_MASTER) {
            that->mode = local_mode;

            // clk_source
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "clk_source");
                if(!JS_IsUndefined(jsvar)) {
                    if(!JS_IsNumber(jsvar)) { JS_FreeValue(ctx, jsvar); JS_ThrowReferenceError(ctx, "property clk_source is not a number"); ret = JS_EXCEPTION; goto cleanup; }
                    JS_ToUint32(ctx, &clk_source, jsvar);
                }
                JS_FreeValue(ctx, jsvar);
            }
            // glitch_ignore_cnt
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "glitch_ignore_cnt");
                if(!JS_IsUndefined(jsvar)) {
                    if(!JS_IsNumber(jsvar)) { JS_FreeValue(ctx, jsvar); JS_ThrowReferenceError(ctx, "property glitch_ignore_cnt is not a number"); ret = JS_EXCEPTION; goto cleanup; }
                    JS_ToUint32(ctx, &glitch_ignore_cnt, jsvar);
                }
                JS_FreeValue(ctx, jsvar);
            }
            // internal_pullup
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "internal_pullup");
                if(!JS_IsUndefined(jsvar)) {
                    enable_internal_pullup = JS_ToBool(ctx, jsvar);
                }
                JS_FreeValue(ctx, jsvar);
            }
            // allow_pd
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "allow_pd");
                if(!JS_IsUndefined(jsvar)) {
                    allow_power_down = JS_ToBool(ctx, jsvar);
                }
                JS_FreeValue(ctx, jsvar);
            }
            // intr_priority
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "intr_priority");
                if(!JS_IsUndefined(jsvar)) {
                    if(!JS_IsNumber(jsvar)) { JS_FreeValue(ctx, jsvar); JS_ThrowReferenceError(ctx, "property intr_priority is not a number"); ret = JS_EXCEPTION; goto cleanup; }
                    int32_t tmp;
                    JS_ToInt32(ctx, &tmp, jsvar);
                    intr_priority = (int)tmp;
                }
                JS_FreeValue(ctx, jsvar);
            }
            // trans_queue_depth
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "trans_queue_depth");
                if(!JS_IsUndefined(jsvar)) {
                    if(!JS_IsNumber(jsvar)) { JS_FreeValue(ctx, jsvar); JS_ThrowReferenceError(ctx, "property trans_queue_depth is not a number"); ret = JS_EXCEPTION; goto cleanup; }
                    JS_ToUint32(ctx, &trans_queue_depth, jsvar);
                }
                JS_FreeValue(ctx, jsvar);
            }
            // core
            {
                JSValue jsvar = JS_GetPropertyStr(ctx, argv[0], "core");
                if(!JS_IsUndefined(jsvar)) {
                    if(!JS_IsNumber(jsvar)) { JS_FreeValue(ctx, jsvar); JS_ThrowReferenceError(ctx, "property core is not a number"); ret = JS_EXCEPTION; goto cleanup; }
                    uint32_t tmp;
                    JS_ToUint32(ctx, &tmp, jsvar);
                    core = (uint8_t)tmp;
                }
                JS_FreeValue(ctx, jsvar);
            }

            bus_config.i2c_port = that->busnum;
            bus_config.sda_io_num = that->_sda;
            bus_config.scl_io_num = that->_scl;
            bus_config.clk_source = static_cast<i2c_clock_source_t>(clk_source);
            bus_config.glitch_ignore_cnt = static_cast<uint8_t>(glitch_ignore_cnt);
            bus_config.intr_priority = intr_priority;
            bus_config.trans_queue_depth = trans_queue_depth;
            bus_config.flags.enable_internal_pullup = enable_internal_pullup;
            bus_config.flags.allow_pd = allow_power_down;

            run_wait_on_core([&]() {
                cleanup_master_bus();
                res = i2c_new_master_bus(&bus_config, &that->bus_handle);
            }, core);

            if(res != ESP_OK) {
                JS_ThrowReferenceError(ctx, "failed to setup i2c master bus: %d", res);
                ret = JS_EXCEPTION;
                goto cleanup;
            }

            JSValue dev = JS_GetPropertyStr(ctx, that->jsobj, "dev");
            if(JS_IsObject(dev)) {
                uint32_t len = 0;
                JSValue len_val = JS_GetPropertyStr(ctx, dev, "length");
                JS_ToUint32(ctx, &len, len_val);
                JS_FreeValue(ctx, len_val);

                for(uint32_t i = 0; i < len; i++) {
                    JSValue dev_obj = JS_GetPropertyUint32(ctx, dev, i);
                    if(!JS_IsObject(dev_obj)) {
                        JS_FreeValue(ctx, dev_obj);
                        continue;
                    }

                    if(!that->addDevice(ctx, dev_obj)) {
                        JS_FreeValue(ctx, dev);
                        JS_FreeValue(ctx, dev_obj);
                        ret = JS_EXCEPTION;
                        goto cleanup;
                    }

                    JS_FreeValue(ctx, dev_obj);
                }
            }
            JS_FreeValue(ctx, dev);

            ret = JS_UNDEFINED;
            goto cleanup;
        }

        #if SOC_I2C_SUPPORT_SLAVE
        if(local_mode == I2C_MODE_SLAVE) {
            cleanup_master_bus();
            JS_ThrowReferenceError(ctx, "I2C slave mode is not supported by NG driver yet");
            ret = JS_EXCEPTION;
            goto cleanup;
        }
        #endif

        JS_ThrowReferenceError(ctx, "invalid mode");
        ret = JS_EXCEPTION;
        goto cleanup;

    cleanup:
        if(locked) {
            that->give();
        }
        return ret;
    }

    /**
     * 向 I2C 总线添加一个设备。
     * 
     * 将设备配置添加到总线，用于后续的读写操作。
     * 如果设备已存在，则更新其配置信息。
     * 
     * options 参数说明：
     * ```js
     * {
     *     addr: number,               // 设备地址（必需）
     *     addrBit10: bool = false,    // 是否使用 10 位地址（默认 7 位）
     *     regAddrBits: number = 8,    // 寄存器地址位数（8/16/32，默认 8）
     *     regBits: number = 8,        // 寄存器值位数（8/16/32，默认与 regAddrBits 相同）
     *     freq: number = 100000,      // 时钟频率（Hz，默认 100kHz）
     *     timeout: number = 1000,     // 超时时间（微秒，默认 1000）
     *     ackCheck: bool = true       // 是否启用 ACK 检查（默认启用）
     * }
     * ```
     * 
     * @module serial
     * @class I2C
     * @function addDevice
     * @param options:object 设备配置对象
     * @return undefined
     * @throws I2C 未在主机模式下初始化
     * @throws 缺少设备地址
     * @throws 寄存器位数无效（必须是 8/16/32）
     * @throws 添加设备失败
     */
    JSValue I2C::addDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            JSTHROW("I2C is not in %s mode", "master");
        }
        if(!that->addDevice(ctx, argv[0])) {
            that->give();
            return JS_EXCEPTION;
        }
        that->give();
        return JS_UNDEFINED ;
    }

    /**
     * 从 I2C 总线移除一个设备。
     * 
     * 删除指定地址的设备配置，释放相关资源。
     * 
     * @module serial
     * @class I2C
     * @function removeDevice
     * @param address:number 设备地址
     * @param bit10:bool=false 是否使用 10 位地址（默认 false，即 7 位地址）
     * @return undefined
     * @throws I2C 未在主机模式下初始化
     * @throws 设备不存在
     * @throws 移除设备失败
     */
    JSValue I2C::removeDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT16(0, addr)
        bool bit10 = addr > 0x7F;
        if(argc > 1) {
            bit10 = JS_ToBool(ctx, argv[1]);
        }

        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            JSTHROW("I2C is not in %s mode", "master");
        }

        uint16_t addr_key = bit10 ? (addr | (30<<10)) : addr ;
        if(that->devices.count(addr_key) == 0) {
            that->give();
            JSTHROW("i2c device 0x%02X not found", addr) ;
        }
        inner::i2c_device_memo_t & dev = that->devices[addr_key] ;
        if(dev.dev_handle) {
            esp_err_t res = i2c_master_bus_rm_device(dev.dev_handle) ;
            if(res != ESP_OK) {
                that->give();
                JSTHROW("failed to remove i2c device 0x%02X: %d", addr, res) ;
            }
            dev.dev_handle = nullptr ;
        }
        that->devices.erase(addr_key) ;

        that->give();
        return JS_UNDEFINED ;
    }

    /**
     * 关闭并释放 I2C 总线资源。
     * 
     * 移除所有已添加的设备，删除总线句柄，释放相关资源。
     * 调用后 I2C 总线恢复为未初始化状态。
     * 
     * @module serial
     * @class I2C
     * @function unsetup
     * @return bool 成功返回 true，失败返回 false
     */
    JSValue I2C::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        that->take();
        #if SOC_I2C_SUPPORT_SLAVE
        // if(that->slaverRegisters) {
        //     free(that->slaverRegisters) ;
        //     that->slaverRegisters = nullptr ;
        //     that->slaverRegisterLength = 0 ;
        // }

        // if(that->slaveTask) {
        //     vTaskDelete(that->slaveTask);
        //     that->slaveTask = nullptr ;
        // }
        #endif

        esp_err_t res = ESP_OK ;
        if(!that->devices.empty()) {
            for(auto &entry : that->devices) {
                if(entry.second.dev_handle) {
                    esp_err_t rm_res = i2c_master_bus_rm_device(entry.second.dev_handle) ;
                    if(rm_res != ESP_OK) {
                        res = rm_res ;
                    }
                    entry.second.dev_handle = nullptr ;
                }
            }
            that->devices.clear() ;
        }

        if(that->bus_handle) {
            esp_err_t del_res = i2c_del_master_bus(that->bus_handle) ;
            if(del_res != ESP_OK && res == ESP_OK) {
                res = del_res ;
            }
            that->bus_handle = nullptr ;
        }

        that->mode = I2C_MODE_MASTER ;
        that->give();
        return res==ESP_OK? JS_TRUE: JS_FALSE ;
    }
    /**
     * 检查 I2C 总线是否已初始化。
     * 
     * @module serial
     * @class I2C
     * @function isInstalled
     * @return bool 已初始化返回 true，否则返回 false
     */
    JSValue I2C::isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        that->take();
        bool installed = that->isInstalled();
        that->give();
        return installed ? JS_TRUE : JS_FALSE ;
    }

    bool I2C::isInstalled() const {
        i2c_master_bus_handle_t handle = nullptr;
        esp_err_t err = i2c_master_get_bus_handle(busnum, &handle);
        return err == ESP_OK && handle != nullptr;
    }

    inner::i2c_device_memo_t * I2C::devMemo(uint16_t addr, bool bit10) {

        uint16_t addr_key = bit10 ? (addr | (30<<10)) : addr ;
        if(devices.count(addr_key)>0) {
            return & devices[addr_key] ;
        }

        if( !isInstalled() || mode != I2C_MODE_MASTER ) {
            return nullptr ;
        }
        if(!bus_handle) {
            esp_err_t res = i2c_master_get_bus_handle(busnum, &bus_handle);
            if (res != ESP_OK) {
                return nullptr;
            }
        }
        
        // Create device handle with default configuration
        i2c_device_config_t dev_config = {
            .dev_addr_length = bit10 ? I2C_ADDR_BIT_LEN_10 : I2C_ADDR_BIT_LEN_7,
            .device_address = addr ,
            .scl_speed_hz = 100000,
            .scl_wait_us = 1000,
            .flags = {
                .disable_ack_check = true,
            }
        };
        i2c_master_dev_handle_t dev_handle = nullptr;
        esp_err_t res = i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);
        if (res != ESP_OK) {
            return nullptr;
        }

        inner::i2c_device_memo_t device_info = {
            .addr = addr ,
            .dev_handle = dev_handle,
            .addr_len = dev_config.dev_addr_length ,
            .reg_addr_bits = 8 ,
            .reg_bits = 8 ,
        } ;
        devices[addr_key] = device_info ;

        return & devices[addr_key] ;
    }
    uint8_t I2C::devRegBit(uint16_t addr, bool bit10) {
        inner::i2c_device_memo_t * memo = devMemo(addr, bit10);
        if(!memo) {
            return 8 ;
        }
        return memo->reg_addr_bits ;
    }
    i2c_master_dev_handle_t I2C::devHandle(uint16_t addr, bool bit10) {
        inner::i2c_device_memo_t * memo = devMemo(addr, bit10);
        return memo ? memo->dev_handle : nullptr;
    }

    bool I2C::ping(uint16_t addr, uint32_t timeout_ms) {
        if(mode!=I2C_MODE_MASTER) {
            return false ;
        }
        int probe_timeout = (timeout_ms > static_cast<uint32_t>(INT_MAX)) ? INT_MAX : static_cast<int>(timeout_ms) ;
        
        esp_log_level_t loglevel = esp_log_level_get("i2c.master");
        esp_log_level_set("i2c.master", ESP_LOG_WARN);

        esp_err_t ret = i2c_master_probe(bus_handle, addr, probe_timeout) ;

        esp_log_level_set("i2c.master", loglevel);
        
        return ret==ESP_OK ;
    }

    void I2C::scan(uint16_t from, uint16_t to, uint32_t timeout_ms) {
        if(mode!=I2C_MODE_MASTER) {
            return ;
        }
        for(uint8_t addr=from; addr<=to; addr++) {
            if( ping(addr, timeout_ms) ){
                printf("found device: 0x%02x\n", addr) ;
            }
        }
    }

    bool I2C::send(uint16_t addr, uint8_t * data, size_t data_len) {
        if(mode != I2C_MODE_MASTER) {
            return false;
        }

        bool bit10 = addr > 0x7F;
        const uint16_t ten_bit_key = static_cast<uint16_t>(addr | (30 << 10));
        if(!bit10 && devices.count(ten_bit_key) > 0) {
            bit10 = true;
        }

        auto handle = devHandle(addr, bit10);
        if(!handle && bit10) {
            handle = devHandle(addr, false);
        }
        if(!handle) {
            return false;
        }

        constexpr int default_timeout_ms = 50;

        if(data_len == 0) {
            i2c_master_transmit_multi_buffer_info_t buffer_info = {};
            buffer_info.write_buffer = nullptr;
            buffer_info.buffer_size = 0;
            return i2c_master_multi_buffer_transmit(handle, &buffer_info, 1, default_timeout_ms) == ESP_OK;
        }

        if(!data) {
            return false;
        }

        return i2c_master_transmit(handle, data, data_len, default_timeout_ms) == ESP_OK;
    }

    bool I2C::recv(uint16_t addr, uint8_t * buff, size_t buffsize) {
        if(mode != I2C_MODE_MASTER || !buff || buffsize == 0) {
            return false;
        }

        bool bit10 = addr > 0x7F;
        const uint16_t ten_bit_key = static_cast<uint16_t>(addr | (30 << 10));
        if(!bit10 && devices.count(ten_bit_key) > 0) {
            bit10 = true;
        }

        auto handle = devHandle(addr, bit10);
        if(!handle && bit10) {
            handle = devHandle(addr, false);
        }
        if(!handle) {
            return false;
        }

        constexpr int default_timeout_ms = 50;
        return i2c_master_receive(handle, buff, buffsize, default_timeout_ms) == ESP_OK;
    }


    /**
     * 检测指定地址的设备是否存在。
     * 
     * 向指定 I2C 地址发送探测信号，检查设备是否响应。
     * 
     * @module serial
     * @class I2C
     * @function ping
     * @param address:number 设备 I2C 地址（7 位或 10 位）
     * @return bool 设备存在返回 true，否则返回 false
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     */
    JSValue I2C::ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT16(0, addr)
        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            JSTHROW("I2C is not in %s mode", "master");
        }
        bool result = that->ping(addr);
        that->give();
        return result ? JS_TRUE : JS_FALSE ;
    }
    /**
     * 扫描 I2C 总线上的所有设备。
     * 
     * 在指定地址范围内扫描，打印发现的设备地址到控制台。
     * 
     * @module serial
     * @class I2C
     * @function scan
     * @param from:number=1 起始扫描地址（默认 1）
     * @param to:number=127 结束扫描地址（默认 127）
     * @param timeout_ms:number=20 探测超时时间（毫秒，默认 20）
     * @return undefined
     * @throws I2C 未在主机模式下初始化
     */
    JSValue I2C::scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ARGV_TO_UINT8_OPT(0, from, 1)
        ARGV_TO_UINT8_OPT(1, to, 127)
        ARGV_TO_UINT32_OPT(2, timeout_ms, 20)
        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            JSTHROW("I2C is not in %s mode", "master");
        }
        that->scan(from, to, timeout_ms);
        that->give();
        return JS_UNDEFINED ;
    }
    /**
     * 向 I2C 设备发送数据。
     * 
     * 向指定地址的设备发送原始数据（不指定寄存器）。
     * 
     * @module serial
     * @class I2C
     * @function send
     * @param address:number 目标设备地址
     * @param data:uint8[] 要发送的数据字节数组
     * @return bool 发送成功返回 true，失败返回 false
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 数据参数不是数组
     */
    JSValue I2C::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT16(0, addr)
        if(!JS_IsArray(ctx, argv[1])) {
            JSTHROW("arg must be a array")
        }
        int len ;
        uint8_t * data = JS_ArrayToBufferUint8(ctx, argv[1], &len) ;
        if(!data) {
            return JS_FALSE ;
        }

        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            free(data);
            JSTHROW("I2C is not in %s mode", "master");
        }
        bool res = that->send(addr, data, len);
        that->give();
        free(data);
        return res ? JS_TRUE : JS_FALSE ;
    }

    #define I2C_WRITE(type, ARGV_CONVERT)       \
        ASSERT_ARGC(3)                          \
        ARGV_TO_UINT16(0, addr)                 \
        ARGV_TO_UINT8(1, reg)                   \
        ARGV_CONVERT(2, byte)                   \
        THIS_NCLASS(I2C, that)                  \
        that->take();                           \
        if(that->mode != I2C_MODE_MASTER) {     \
            that->give();                       \
            JSTHROW("I2C is not in %s mode", "master"); \
        }                                       \
        if(that->devices.count(addr)==0 && !that->devHandle(addr)) {   \
            that->give();                       \
            JSTHROW("device 0x%02x is not installed", addr);          \
        }                                       \
        bool __wr_ok = that->write<type>(addr, reg, byte); \
        that->give();                           \
        return __wr_ok ? JS_TRUE : JS_FALSE;

    /**
     * 向 I2C 设备寄存器写入 8 位数据。
     * 
     * @module serial
     * @class I2C
     * @function write8
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @param value:number 要写入的 8 位值（0-255）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     */
    JSValue I2C::write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint8_t,ARGV_TO_UINT8)
    }
    /**
     * 向 I2C 设备寄存器写入 16 位数据。
     * 
     * @module serial
     * @class I2C
     * @function write16
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @param value:number 要写入的 16 位值（0-65535）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     */
    JSValue I2C::write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint16_t,ARGV_TO_UINT16)
    }
    /**
     * 向 I2C 设备寄存器写入 32 位数据。
     * 
     * @module serial
     * @class I2C
     * @function write32
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @param value:number 要写入的 32 位值
     * @return bool 写入成功返回 true，失败返回 false
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     */
    JSValue I2C::write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint32_t,ARGV_TO_UINT32)
    }

    /**
     * 从 I2C 设备接收数据。
     * 
     * 从指定地址的设备读取指定长度的原始数据（不指定寄存器）。
     * 
     * @module serial
     * @class I2C
     * @function recv
     * @param address:number 设备地址
     * @param length:number 要接收的字节数
     * @return ArrayBuffer|null 接收到的数据，失败返回 null
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 接收长度无效
     * @throws 内存分配失败
     */
    JSValue I2C::recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT16(0, addr)
        ARGV_TO_UINT8(1, len)
        if(len < 1) {
            JSTHROW("invalid recv length")
        }
        uint8_t * buffer = (uint8_t*)malloc(len) ;
        if(!buffer) {
            JSTHROW("out of memory?") ;
        }

        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            free(buffer);
            JSTHROW("I2C is not in %s mode", "master");
        }
        bool ok = that->recv(addr, buffer, len);
        that->give();

        if(!ok) {
            free(buffer);
            return JS_NULL;
        }
        return JS_NewArrayBuffer(ctx, buffer, len, freeArrayBuffer, NULL, false) ;
    }

    /**
     * 从 I2C 设备接收 1 个字节数据。
     * 
     * 从指定地址的设备读取单个字节数据。
     * 
     * @module serial
     * @class I2C
     * @function recvU8
     * @param address:number 设备地址
     * @return number 接收到的 8 位无符号数值（0-255）
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 接收失败
     */
    JSValue I2C::recvU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT16(0, addr)
        THIS_NCLASS(I2C, that)
        that->take();
        if(that->mode != I2C_MODE_MASTER) {
            that->give();
            JSTHROW("I2C is not in %s mode", "master");
        }
        uint8_t byte ;
        if(!that->recv(addr, &byte, 1)) {
            that->give();
            JSTHROW("i2c recv failed");
        }
        that->give();
        return JS_NewUint32(ctx, byte) ;
    }

    /**
     * 从 I2C 设备寄存器读取有符号 8 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readI8
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 8 位有符号数值（-128 到 127）
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readI8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<int8_t>(ctx, this_val, argc, argv, true) ;
    }
    /**
     * 从 I2C 设备寄存器读取有符号 16 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readI16
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 16 位有符号数值（-32768 到 32767）
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readI16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<int16_t>(ctx, this_val, argc, argv, true) ;
    }
    /**
     * 从 I2C 设备寄存器读取有符号 32 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readI32
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 32 位有符号数值
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readI32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<int32_t>(ctx, this_val, argc, argv, true) ;
    }
    /**
     * 从 I2C 设备寄存器读取无符号 8 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readU8
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 8 位无符号数值（0-255）
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<uint8_t>(ctx, this_val, argc, argv, true) ;
    }
    /**
     * 从 I2C 设备寄存器读取无符号 16 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readU16
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 16 位无符号数值（0-65535）
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<uint16_t>(ctx, this_val, argc, argv, true) ;
    }
    /**
     * 从 I2C 设备寄存器读取无符号 32 位数据。
     * 
     * @module serial
     * @class I2C
     * @function readU32
     * @param address:number 设备地址
     * @param reg:number 寄存器地址
     * @return number 读取到的 32 位无符号数值
     * @throws 参数数量不足
     * @throws I2C 未在主机模式下初始化
     * @throws 设备未添加
     * @throws 读取失败
     */
    JSValue I2C::readU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return readRegInt<uint32_t>(ctx, this_val, argc, argv, true) ;
    }

    // -------------------
    // arduino like api

    gpio_num_t I2C::sda() const {
        return _sda ;
    }
    gpio_num_t I2C::scl() const {
        return _scl ;
    }
    i2c_port_t I2C::number() const {
        return busnum ;
    }

    void I2C::begin(uint32_t freq) {
        take();
        if (isInstalled()) {
            give();
            return ;
        }
        if (_sda == GPIO_NUM_NC || _scl == GPIO_NUM_NC) {
            give();
            return ;
        }
        i2c_master_bus_config_t bus_config = {} ;
        bus_config.i2c_port = busnum ;
        bus_config.sda_io_num = _sda ;
        bus_config.scl_io_num = _scl ;
        bus_config.clk_source = I2C_CLK_SRC_DEFAULT ;
        bus_config.glitch_ignore_cnt = 7 ;
        bus_config.flags.enable_internal_pullup = true ;
        i2c_new_master_bus(&bus_config, &bus_handle) ;
        give();
    }

    void I2C::end() {
        take();
        _tx_buffer.clear() ;
        _rx_buffer.clear() ;
        _rx_read_index = 0 ;
        give();
    }

    void I2C::beginTransmission(uint8_t addr) {
        take();
        _arduino_addr = addr ;
        _tx_buffer.clear() ;
        if (mode == I2C_MODE_MASTER) {
            devHandle(addr) ;
        }
        give();
    }

    size_t I2C::write(uint8_t data) {
        take();
        _tx_buffer.push_back(data) ;
        size_t sz = _tx_buffer.size() ;
        give();
        return sz ;
    }

    int I2C::endTransmission(bool stop) {
        take();
        if (_tx_buffer.empty() || mode != I2C_MODE_MASTER) {
            give();
            return -1 ;
        }
        i2c_master_dev_handle_t handle = devHandle(_arduino_addr) ;
        if (!handle) {
            give();
            return -1 ;
        }
        esp_err_t res = i2c_master_transmit(handle, _tx_buffer.data(), _tx_buffer.size(), 10) ;
        _tx_buffer.clear() ;
        give();
        return (int)res ;
    }

    uint8_t I2C::requestFrom(uint8_t addr, size_t len, uint8_t stop) {
        take();
        if (mode != I2C_MODE_MASTER || len == 0) {
            give();
            return 0 ;
        }
        i2c_master_dev_handle_t handle = devHandle(addr) ;
        if (!handle) {
            give();
            return 0 ;
        }
        _rx_buffer.resize(len) ;
        _rx_read_index = 0 ;
        esp_err_t res = i2c_master_receive(handle, _rx_buffer.data(), len, 10) ;
        if (res != ESP_OK) {
            _rx_buffer.clear() ;
            give();
            return 0 ;
        }
        give();
        return len ;
    }

    int I2C::read() {
        take();
        if (_rx_read_index >= (int)_rx_buffer.size()) {
            give();
            return -1 ;
        }
        int val = _rx_buffer[_rx_read_index++] ;
        give();
        return val ;
    }

    int I2C::available() {
        take();
        int avail = (int)_rx_buffer.size() - _rx_read_index ;
        give();
        return avail ;
    }
}

#endif

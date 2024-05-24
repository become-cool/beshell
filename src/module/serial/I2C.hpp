#pragma once

#include <NativeClass.hpp>
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

namespace be {

    #define I2C_BEGIN(addr, act)                                        \
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();                   \
        i2c_master_start(cmd);                                          \
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_##act, 0x1);
        
    #define I2C_BEGIN_READ(addr)    I2C_BEGIN(addr, READ)
    #define I2C_BEGIN_WRITE(addr)   I2C_BEGIN(addr, WRITE)

    #define I2C_COMMIT(bus)                                                     \
        i2c_master_stop(cmd);                                                   \
        esp_err_t res=i2c_master_cmd_begin(bus, cmd, 10/portTICK_PERIOD_MS) ;   \
        i2c_cmd_link_delete(cmd);

    /**
     * 该类的构造函数没有绑定给 JS , 无法从 JS 创建实例。
     * 
     * 模块 [serial](overview.md) 创建了和硬件对应的 I2C 实例，`import serial` 即可。
     * 
     * @module [serial](../serial)
     * @class I2C
     */
    class I2C: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        i2c_port_t busnum ;
        gpio_num_t _sda = GPIO_NUM_NC ;
        gpio_num_t _scl = GPIO_NUM_NC ;
        SemaphoreHandle_t sema ;

        i2c_mode_t mode = I2C_MODE_MASTER ;

        #if SOC_I2C_SUPPORT_SLAVE
        TaskHandle_t slaveTask = nullptr ;
        JSValue slaveListener = JS_NULL ;
        QueueHandle_t data_queue = nullptr ;
        static void loop(JSContext * ctx, void * opaque) ;
        static void task_i2c_slave(void *arg) ;
        #endif

        static I2C * i2c0 ;
        #if SOC_I2C_NUM > 1
        static I2C * i2c1 ;
        #endif
        #if SOC_LP_I2C_NUM > 0
        static I2C * i2clp0 ;
        #endif

        std::map<int, std::pair<int, void *>> slaveRegisters ;

        uint8_t * slaverRegisters = nullptr ;
        size_t slaverRegisterLength = 0 ;
        size_t slaverRegisterSize = 1 ;
        size_t slaverRegisterAddrSize = 1 ;

    public:
        I2C(JSContext * ctx, i2c_port_t busnum) ;
        ~I2C() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static I2C * flyweight(JSContext *, i2c_port_t) ;

        gpio_num_t sda() ;
        gpio_num_t scl() ;

        inline void take() ;
        inline void give() ;
        bool ping(uint8_t addr) ;
        void scan(uint8_t from=0, uint8_t to=127) ;
        bool send(uint8_t addr, uint8_t * data, size_t data_len) ;

        template <typename TR, typename TV>
        bool write(uint8_t addr, TR reg, TV value) {
            uint8_t _reg[sizeof(TR)] ;
            for(int i=0;i<sizeof(TR);i++) {
                _reg[i] = ( reg >> ((sizeof(TR)-i-1)*8) ) & 0xFF ;
            }
            I2C_BEGIN_WRITE(addr)
            i2c_master_write(cmd, _reg, sizeof(TR), true) ;
            i2c_master_write(cmd, (const uint8_t *)&value, sizeof(TV), true) ;
            I2C_COMMIT(busnum) ;
            return res == ESP_OK ;
        }

        bool recv(uint8_t addr, uint8_t * buff, size_t buffsize) ;
        
        template <typename TR>
        bool read(uint8_t addr, TR reg, uint8_t * buff, size_t buff_size) {
            uint8_t _reg[sizeof(TR)] ;
            for(int i=0;i<sizeof(TR);i++) {
                _reg[i] = ( reg >> ((sizeof(TR)-i-1)*8) ) & 0xFF ;
            }

            I2C_BEGIN_WRITE(addr)
            i2c_master_write(cmd, (uint8_t*)&_reg, sizeof(TR), 0x1) ;

            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, 0x1);
            i2c_master_read(cmd, buff, buff_size, I2C_MASTER_LAST_NACK);
            I2C_COMMIT(busnum)
            return ESP_OK==res ;
        }
        
        template <typename TR, typename TV>
        bool read(uint8_t addr, TR reg, TV & out) {
            return read<TR>(addr,reg,(uint8_t *)&out,sizeof(TV)) ;
        }

        /**
         * 设置 i2c 外设, 若遇到错误则抛出异常
         * 
         * ```typescript
         * options: {
         *   sda: number,
         *   scl: number,
         *   mode: number = 0
         *   rx_buffer_len: number = 0
         *   tx_buffer_len: number = 0
         *   freq: number = 400000
         *   addr_10bit_en: number = 0
         *   slave_addr: number|undefined = undefined
         *   timeout: number = 1000
         * }
         * ```
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method setup
         * 
         * @param options:object
         * @return undefined
         * 
         */
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        // for master mode ---------------
            
        /**
         * 测试总线上的设备是否存在
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method ping
         * 
         * @param addr:number 设备地址(0-127)
         * @return bool true 表示该地址上设备存在，false 表示不存在
         */
        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        /**
         * 在总线上扫描设备给定地址范围的所有设备，在控制台上输出存在的地址
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method scan
         * 
         * @param from:number=0 设备地址(0-127)
         * @param to:number=127 设备地址(0-127)
         * @return undefined
         */
        static JSValue scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        /**
         * 在总线上向指定设备发送数据
         * 
         * 参数 `data` 可以是 ArrayBuffer 对象, 也可以是字节数值的数值, 例如：[0x1, 0x2, 0x3]
         * 
         * * 发送数据后，接收到设备的 ack 回应则返回 true
         * * 否则返回 false (总线上设备可能不存在)
         * * 其他错误抛出异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method send
         * 
         * @param addr:number 设备地址(0-127)
         * @param data:number[]|ArrayBuffer 字节数据
         * @return bool
         */
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        
        /**
         * 在总线上向指定设备的8位寄存器地址写入数据
         * 
         * * 发送数据后，接收到设备的 ack 回应则返回 true
         * * 否则返回 false (总线上设备可能不存在)
         * * 其他错误抛出异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method write8
         * 
         * @param devAddr:number 设备地址(0-127)
         * @param regAddr:number 寄存器地址(0-255)
         * @param value:number 16位数值(0-255)
         * @return bool
         */
        static JSValue write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        /**
         * 在总线上向指定设备的16位寄存器地址写入数据
         * 
         * * 发送数据后，接收到设备的 ack 回应则返回 true
         * * 否则返回 false (总线上设备可能不存在)
         * * 其他错误抛出异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method write16
         * 
         * @param devAddr:number 设备地址(0-127)
         * @param regAddr:number 寄存器地址(0-255)
         * @param value:number 16位数值(0-65535)
         * @return bool
         */
        static JSValue write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        /**
         * 在总线上向指定设备的32位寄存器地址写入数据
         * 
         * * 发送数据后，接收到设备的 ack 回应则返回 true
         * * 否则返回 false (总线上设备可能不存在)
         * * 其他错误抛出异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method write32
         * 
         * @param devAddr:number 设备地址(0-127)
         * @param regAddr:number 寄存器地址(0-255)
         * @param value:number 32位数值(0-4294967295)
         * @return bool
         */
        static JSValue write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        
        /**
         * 在总线上从指定地址(参数`addr`)的设备, 接收指定长度(参数`length`)的数据
         * 
         * * 接收到的数据以 ArrayBuffer 对象返回
         * * 如果总线上设备不存在则返回 null
         * * 其他错误抛出异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method recv
         * 
         * @param addr:number 设备地址(0-127)
         * @param length:number 接收数据的长度
         * @return ArrayBuffer|null
         */
        static JSValue recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        
        /**
         * 在总线上从指定地址(参数`addr`)的设备, 接收一个无符号8位数据
         * 
         * * 抛出错误异常
         * 
         * @module [serial](../serial)
         * @class I2C
         * @method recvUint8
         * 
         * @param addr:number 设备地址(0-127)
         * @return number
         */
        static JSValue recvUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue readR8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        

        

        static JSValue readR8I8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR8U8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16I8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16U8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue readR8I16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR8U16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16I16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16U16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue readR8I32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR8U32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16I32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16U32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        // for slave mode ---------------
        #if SOC_I2C_SUPPORT_SLAVE
        static JSValue listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue slaveWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue slaveWriteReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue slaveWriteBuff(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue slaveReadBuff(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        #endif

        // arduino like api
    public:
        void begin(uint32_t freq=100000) ;
        void end() ;
        void beginTransmission(uint8_t addr);
        size_t write(uint8_t data);
        int endTransmission(bool stop=true);
        uint8_t requestFrom(uint8_t addr, size_t len, uint8_t stop=true)  ;
        int read() ;
        int available() ;
    private:
        i2c_cmd_handle_t cmd = nullptr ;
        size_t tx_wrote_bytes = 0 ;
    } ;
}

class TwoWire : public be::I2C {} ;

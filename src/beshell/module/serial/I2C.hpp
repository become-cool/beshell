#pragma once

#include <vector>
#include "../../NativeClass.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2c_master.h"
#include "../../debug.h"
#if SOC_I2C_SUPPORT_SLAVE
#include "driver/i2c_slave.h"
#endif


    
namespace be {

    namespace inner {
        typedef struct {
            uint16_t addr ;
            i2c_master_dev_handle_t dev_handle ;
            i2c_addr_bit_len_t addr_len ;
            uint8_t reg_addr_bits ;
            uint8_t reg_bits ;
        } i2c_device_memo_t ;
    }

    /**
     * 该类的构造函数没有绑定给 JS , 无法从 JS 创建实例。
     * 
     * 模块 [serial](overview.md) 创建了和硬件对应的 I2C 实例，`import serial` 即可。
     * 
     * @module serial
     * @class I2C
     */
    class I2C: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        I2C(JSContext * ctx, i2c_port_t busnum) ;
        ~I2C() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static I2C * flyweight(JSContext *, i2c_port_t) ;

        gpio_num_t sda() const ;
        gpio_num_t scl() const ;
        i2c_port_t number() const ;

        bool ping(uint16_t addr, uint32_t timeout_ms=20) ;
        void scan(uint16_t from=0, uint16_t to=127, uint32_t timeout_ms=20) ;
        bool send(uint16_t addr, uint8_t * data, size_t data_len) ;
        bool recv(uint16_t addr, uint8_t * buff, size_t buffsize) ;

        template <typename TV>
        bool write(uint16_t addr, uint32_t reg, TV value) {
            if(mode != I2C_MODE_MASTER) {
                return false;
            }

            inner::i2c_device_memo_t * memo = devMemo(addr, (addr > 0x7F));
            if(!memo) {
                return false;
            }
            uint8_t _reg[4] ;
            int addrBytes = memo->reg_addr_bits / 8 ;
            for(int i=0;i<addrBytes;i++) {
                _reg[i] = ( reg >> ((addrBytes-i-1)*8) ) & 0xFF ;
            }
            uint8_t _value[sizeof(TV)] ;
            for(int i=0;i<sizeof(TV);i++) {
                _value[i] = ( value >> ((sizeof(TV)-i-1)*8) ) & 0xFF ;
            }

            constexpr int default_timeout_ms = 10;

            i2c_master_transmit_multi_buffer_info_t buffer_info[2] = {};
            buffer_info[0].write_buffer = _reg;
            buffer_info[0].buffer_size = addrBytes;
            buffer_info[1].write_buffer = _value;
            buffer_info[1].buffer_size = sizeof(TV);

            return i2c_master_multi_buffer_transmit(memo->dev_handle, buffer_info, 2, default_timeout_ms) == ESP_OK;
        }
        
        template <typename TA>
        bool read(TA addr, uint32_t reg, uint8_t * buff, size_t buff_size) {
            if(mode != I2C_MODE_MASTER || !buff || buff_size == 0) {
                return false;
            }
            
            assert(sizeof(TA)==1 || sizeof(TA)==2);
            bool bit10 = sizeof(TA) == 2 ;
            
            inner::i2c_device_memo_t * memo = devMemo(addr, bit10);
            if(!memo) {
                return false;
            }

            int bytes = memo->reg_addr_bits / 8 ;
            uint8_t reg_buf[4];
            for(int i = 0; i < bytes; i++) {
                reg_buf[i] = (reg >> ((memo->reg_addr_bits - (i + 1) * 8))) & 0xFF;
            }

            constexpr int default_timeout_ms = 10;
            return i2c_master_transmit_receive(memo->dev_handle, reg_buf, bytes, buff, buff_size, default_timeout_ms) == ESP_OK;
        }
        
        template <typename TA, typename TV>
        bool read(uint16_t addr, uint32_t reg, TV & out) {
            assert(sizeof(TA)==1 || sizeof(TA)==2);
            bool bit10 = sizeof(TA) == 2 ;
            inner::i2c_device_memo_t * memo = devMemo(addr, bit10);
            if(!memo) {
                return false;
            }
            return read<TA>(addr,reg,(uint8_t *)&out,sizeof(TV)) ;
        }

        template <typename TV>
        static JSValue readRegInt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, bool isSigned=true) {
            THIS_NCLASS(I2C, that)
            if(that->mode!=I2C_MODE_MASTER) {
                JSTHROW("I2C is not in %s mode", "master") ;
            }
            ASSERT_ARGC(2)
            ARGV_TO_UINT16(0, addr)
            bool bit10 = addr > 0x7F;
            
            inner::i2c_device_memo_t * memo = that->devMemo(addr, bit10);
            if(!memo) {
                JSTHROW("i2c device 0x%02X not found", addr) ;
            }

            uint32_t reg32 = 0 ;
            if( JS_ToUint32(ctx, &reg32, argv[1])!=0 ) {
                JSTHROW("Invalid register address")
            }

            ARGV_TO_UINT32_OPT(2, len, 1)
            TV * buffer = (TV*) malloc( len * sizeof(TV) ) ;
            if(!buffer) {
                JSTHROW("out of memory?") ;
            }
            memset(buffer,128,len*sizeof(TV)) ;

            if(bit10) {
                if(!that->read<uint16_t>(addr,reg32,(uint8_t*)buffer,len)){
                    free(buffer) ;
                    JSTHROW("i2c read failed") ;
                }
            }
            else {
                if(!that->read<uint8_t>(addr,reg32,(uint8_t*)buffer,len)){
                    free(buffer) ;
                    JSTHROW("i2c read failed") ;
                }
            }

            JSValue arr ;
            if(isSigned) {
                JS_NewArrayWithInt(arr, buffer, len) ;
            } else {
                JS_NewArrayWithUint(arr, buffer, len) ;
            }
            free(buffer) ;
            return arr ;
        }
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        bool isInstalled() const ;

        // for master mode ---------------
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recvU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue readI8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readI16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readI32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
            
         
        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        // for slave mode ---------------
        #if SOC_I2C_SUPPORT_SLAVE
        // todo ...
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

        bool addDevice(i2c_device_config_t & config, uint8_t regAddrBits=8, uint8_t regBits=8) ;

    private:

        bool addDevice(JSContext *ctx, JSValue devOption) ;
        
        i2c_port_t busnum ;
        gpio_num_t _sda = GPIO_NUM_NC ;
        gpio_num_t _scl = GPIO_NUM_NC ;
        i2c_mode_t mode = I2C_MODE_MASTER ;

        static I2C * i2c0 ;
        #if SOC_I2C_NUM > 1
        static I2C * i2c1 ;
        #endif
        #if SOC_LP_I2C_NUM > 0
        static I2C * i2clp0 ;
        #endif

        #if SOC_I2C_SUPPORT_SLAVE
        // ...
        #endif

        uint8_t * slaverRegisters = nullptr ;
        size_t slaverRegisterLength = 0 ;
        size_t slaverRegisterSize = 1 ;
        size_t slaverRegisterAddrSize = 1 ;

        i2c_master_bus_handle_t bus_handle = nullptr ; 
        std::map<uint16_t, inner::i2c_device_memo_t> devices ;
        i2c_master_dev_handle_t devHandle(uint16_t addr, bool bit10=false) ;
        uint8_t devRegBit(uint16_t addr, bool bit10=false) ;
        inner::i2c_device_memo_t * devMemo(uint16_t addr, bool bit10=false) ;

        // Arduino-like API internal state
        uint8_t _arduino_addr = 0 ;
        std::vector<uint8_t> _tx_buffer ;
        std::vector<uint8_t> _rx_buffer ;
        int _rx_read_index = 0 ;
    } ;
}

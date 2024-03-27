#pragma once

#include <NativeClass.hpp>
#include "driver/i2c.h"
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


    class I2C: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        i2c_port_t busnum ;
        SemaphoreHandle_t sema ;

        i2c_mode_t mode = I2C_MODE_MASTER ;

        TaskHandle_t slaveTask = nullptr ;
        JSValue slaveListener = JS_NULL ;
        QueueHandle_t data_queue = nullptr ;
        static void loop(JSContext * ctx, void * opaque) ;
        static void task_i2c_slave(void *arg) ;

        static I2C * i2c0 ;
        static I2C * i2c1 ;


    public:
        I2C(JSContext * ctx, i2c_port_t busnum) ;
        ~I2C() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static I2C * flyweight(JSContext *, i2c_port_t) ;

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

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recvUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readR32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue slaveWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
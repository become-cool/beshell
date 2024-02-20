#pragma once

#include <NativeClass.hpp>
#include "driver/i2c.h"

namespace be {

    #define I2C_BEGIN(addr, act)                                        \
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();                   \
        i2c_master_start(cmd);                                          \
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_##act, true);
        
    #define I2C_BEGIN_READ(addr)    I2C_BEGIN(addr, READ)
    #define I2C_BEGIN_WRITE(addr)   I2C_BEGIN(addr, WRITE)

    #define I2C_RECV(buffer, len)                                       \
        if(len>1) {                                                     \
            i2c_master_read(cmd, buffer, len-1, I2C_MASTER_ACK);        \
            i2c_master_read(cmd, (buffer)+len-1, 1, I2C_MASTER_NACK);   \
        }                                                               \
        else {                                                          \
            i2c_master_read(cmd, buffer, 1, I2C_MASTER_NACK);           \
        }

    #define I2C_COMMIT(bus)                                                     \
        i2c_master_stop(cmd);                                                   \
        xSemaphoreTake(sema, portMAX_DELAY) ;                                   \
        esp_err_t res=i2c_master_cmd_begin(bus, cmd, 10/portTICK_PERIOD_MS) ;   \
        xSemaphoreGive(sema) ;                                                  \
        i2c_cmd_link_delete(cmd);

    #define I2C_READ_INT(var, type, size)                                       \
        CHECK_ARGC(2)                                                           \
        ARGV_I2C_BUSNUM(0, busnum)                                              \
        if(!I2C_IS_SETUP(busnum))                                               \
            return JS_NULL ;                                                    \
        ARGV_TO_UINT8(1, addr)                                                  \
        type var = 0 ;                                                          \
        if(argc>2) {                                                            \
            ARGV_TO_UINT8(2, reg)                                               \
            if( i2c_read(busnum, addr, reg, (uint8_t*)&var, size)!=ESP_OK ) {   \
                return JS_NULL ;                                                \
            }                                                                   \
        } else {                                                                \
            I2C_BEGIN_READ(addr)                                                \
            I2C_RECV((uint8_t*)&var, size)                                      \
            I2C_COMMIT(busnum)                                                  \
        }

    class I2C: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        i2c_port_t busnum ;
        SemaphoreHandle_t sema ;

    public:
        I2C(JSContext * ctx, i2c_port_t busnum) ;
        ~I2C() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        bool ping(uint8_t addr) ;
        bool send(uint8_t addr, uint8_t * data, size_t data_len) ;

        template <typename TR, typename TV>
        bool write(uint8_t addr, TR reg, TV value) {
            I2C_BEGIN_WRITE(addr)
            i2c_master_write(cmd, (const uint8_t *)&reg, sizeof(TR), true) ;
            i2c_master_write(cmd, (const uint8_t *)&value, sizeof(TV), true) ;
            I2C_COMMIT(busnum) ;
            return res == ESP_OK ;
        }

        bool recv(uint8_t addr, uint8_t * buff, size_t buffsize) ;
        
        template <typename TV>
        bool recv(uint8_t addr, TV out) {
            return recv(addr, (uint8_t *)&out, sizeof(TV)) ;
        }
        
        template <typename TR, typename TV>
        bool read(uint8_t addr, TR reg, TV & out) {
            I2C_BEGIN_READ(addr)
            I2C_RECV(&reg,sizeof(TR))
            I2C_RECV(((uint8_t*)&out),sizeof(TV))
            I2C_COMMIT(busnum)
            return ESP_OK!=res ;
        }

        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue recv8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
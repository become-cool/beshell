#ifndef __H_MODULE_SERIAL_I2C__
#define __H_MODULE_SERIAL_I2C__

#include "quickjs-libc.h"
#include "driver/i2c.h"

#define I2C_IS_SETUP(busnum) (_i2c_bus_setup & (1<<(busnum)))

#define ARGV_I2C_BUSNUM(i, var)                     \
    ARGV_TO_UINT8(i, var)                           \
    if(var<0 || var>=I2C_NUM_MAX) {                 \
        THROW_EXCEPTION("Bus num must be 0-1")      \
    }

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
    i2c_take_semph(bus, portMAX_DELAY ) ;                                   \
	esp_err_t res=i2c_master_cmd_begin(bus, cmd, 10/portTICK_PERIOD_MS) ;   \
    i2c_give_semph(bus) ;                                                   \
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

#define FREE_BUS_I2C(busnum)    if(_i2c_bus_setup&(1<<(busnum))){ i2c_driver_delete(busnum); }

esp_err_t i2c_write(uint8_t bus, uint8_t addr, uint8_t reg, uint8_t * data, size_t len) ;
esp_err_t i2c_write_byte(uint8_t bus, uint8_t addr, uint8_t reg, uint8_t data) ;
esp_err_t i2c_read(uint8_t bus, uint8_t addr, uint8_t reg, uint8_t * data, uint8_t len) ;

bool i2c_ping(uint8_t bus, uint8_t addr) ;

bool i2c_take_semph(i2c_port_t busnum, TickType_t wait) ;
bool i2c_give_semph(i2c_port_t busnum) ;

void be_module_serial_i2c_init() ;
void be_module_serial_i2c_require(JSContext *ctx, JSValue pkg) ;
void be_module_serial_i2c_loop(JSContext *ctx) ;
void be_module_serial_i2c_reset(JSContext *ctx) ;

#endif
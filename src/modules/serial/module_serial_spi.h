#ifndef __H_MODULE_SERIAL_SPI__
#define __H_MODULE_SERIAL_SPI__

#include "quickjs-libc.h"
#include "driver/spi_master.h"

#define FREE_BUS_SPI(busnum)    if(_spi_bus_setup&(1<<(busnum))){ spi_bus_free(busnum); }

spi_device_handle_t spi_handle_with_id(uint8_t idx) ;
esp_err_t spi_trans_int(spi_device_handle_t handle, uint8_t * in_buff, uint8_t * out_buff, size_t bit_length) ;

// void be_module_serial_spi_init() ;
void be_module_serial_spi_require(JSContext *ctx, JSValue pkg) ;
// void be_module_serial_spi_loop(JSContext *ctx) ;
void be_module_serial_spi_reset(JSContext *ctx) ;

#endif
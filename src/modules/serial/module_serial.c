#include "utils.h"
#include "module_serial.h"


void be_module_serial_require(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    be_module_serial_uart_require(ctx, beapi) ;
    be_module_serial_spi_require(ctx, beapi) ;
    be_module_serial_i2c_require(ctx, beapi) ;
    be_module_serial_i2s_require(ctx, beapi) ;

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

void be_module_serial_reset(JSContext *ctx) {

    be_module_serial_uart_reset(ctx) ;
    be_module_serial_spi_reset(ctx) ;
    be_module_serial_i2c_reset(ctx) ;
    be_module_serial_i2s_reset(ctx) ;
}
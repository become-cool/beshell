#include "driver_rmt.h"
#include "utils.h"
#include "driver/rmt.h"
#include <string.h>
#include "module_utils.h"
#include "esp_system.h"


static JSValue js_driver_rmt_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)

    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, channel)
    ARGV_TO_UINT16(2, clk_div)

    if(channel>=8) {
        THROW_EXCEPTION("arg chanel must equal to 0-7")
    }

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(pin, channel);
    // set counter clock to 40MHz
    config.clk_div = 2;

    CALL_IDF_API(rmt_config(&config))

    return JS_UNDEFINED ;
}

static JSValue js_driver_rmt_install(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    ARGV_TO_UINT8(0, channel)
    ARGV_TO_UINT32_OPT(1, rxBufSize, 0)
    ARGV_TO_UINT32_OPT(2, intrAllocFlags, 0)

    CALL_IDF_API(rmt_driver_install(channel, rxBufSize, intrAllocFlags))

    return JS_UNDEFINED ;

}

void be_module_driver_rmt_init() {
}

void be_module_driver_rmt_require(JSContext *ctx,JSValue driver) {
    
    JSValue rmt = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, driver, "rmt", rmt);

    JS_SetPropertyStr(ctx, rmt, "config", JS_NewCFunction(ctx, js_driver_rmt_config, "config", 1));
    JS_SetPropertyStr(ctx, rmt, "install", JS_NewCFunction(ctx, js_driver_rmt_config, "install", 1));
}

void be_module_driver_rmt_loop(JSContext *ctx) {
}

void be_module_driver_rmt_reset(JSContext *ctx) {
}


#include "module_serial_i2s.h"
#include "utils.h"
#include "driver/i2s.h"
#include <string.h>


static uint8_t _i2s_bus_setup = 0 ;

static JSValue js_i2s_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // return JS_UNDEFINED ;
    CHECK_ARGC(1)

    ARGV_TO_UINT8(0, i2s)
    if(i2s!=0 && i2s!=1){
        THROW_EXCEPTION("arg %s must be 0 or 1", "i2s")
    }


    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = 8000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
        .dma_buf_count = 2,
        .dma_buf_len = 512,
        .use_apll = true,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };


    i2s_pin_config_t pin_config = {
        .ws_io_num = 33,
        .data_out_num = 4,
        .bck_io_num = 26,
        .data_in_num = -1
    };

    if(argc>1) {

        if(!JS_IsObject(argv[1])) {
            THROW_EXCEPTION("arg %s must be an object", "opts");
        }

        ASSIGN_INT_PROP_DEFAULT(argv[1], "mode", i2s_config.mode, (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX))
        ASSIGN_UINT_PROP_DEFAULT(argv[1], "sample_rate", i2s_config.sample_rate, 8000)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "bits_per_sample", i2s_config.bits_per_sample, I2S_BITS_PER_SAMPLE_16BIT)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "channel_format", i2s_config.channel_format, I2S_CHANNEL_FMT_RIGHT_LEFT)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "communication_format", i2s_config.communication_format, I2S_COMM_FORMAT_STAND_I2S)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "intr_alloc_flags", i2s_config.intr_alloc_flags, ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "dma_buf_count", i2s_config.dma_buf_count, 2)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "dma_buf_len", i2s_config.dma_buf_len, 256)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "use_apll", i2s_config.use_apll, 1)
        
        ASSIGN_UINT_PROP(argv[1], "lrclk", pin_config.ws_io_num)
        ASSIGN_UINT_PROP(argv[1], "sclk", pin_config.bck_io_num)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "sout", pin_config.data_out_num, -1)
        ASSIGN_INT_PROP_DEFAULT(argv[1], "sin", pin_config.data_in_num, -1)
    }

    i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT ;
    dn(i2s_config.channel_format)
    dn(i2s_config.sample_rate)
    dn(i2s_config.bits_per_sample)

    if(i2s_driver_install(i2s, &i2s_config, 0, NULL)==ESP_OK){
        _i2s_bus_setup|= 1<<i2s ;
    }
    i2s_set_pin(i2s, &pin_config);

    return JS_UNDEFINED ;
}


bool i2s_has_setup(uint8_t busnum) {
    return (1<<busnum) & _i2s_bus_setup ;
}

void i2s_stop_play(uint8_t busnum) {
    i2s_zero_dma_buffer(busnum);
    i2s_stop(busnum);
    i2s_start(busnum);
}


static JSValue js_i2s_has_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, busnum)
    if(busnum>1){
        THROW_EXCEPTION("arg %s must be 0 or 1", "i2s")
    }
    return i2s_has_setup(busnum)? JS_TRUE : JS_FALSE;
}

static JSValue js_i2s_unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    ARGV_TO_UINT8(0, busnum)
    if(busnum>1){
        THROW_EXCEPTION("arg %s must be 0 or 1", "i2s")
    }
    if(i2s_driver_uninstall(busnum)==ESP_OK){
        _i2s_bus_setup &= ~(1<<busnum) ;
        return JS_TRUE ;
    }
    else {
        return JS_FALSE ;
    }
}


void be_module_serial_i2s_init() {
}

void be_module_serial_i2s_require(JSContext *ctx, JSValue pkg) {

    JSValue i2s = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, pkg, "i2s", i2s);
    JS_SetPropertyStr(ctx, i2s, "setup", JS_NewCFunction(ctx, js_i2s_setup, "setup", 1));
    JS_SetPropertyStr(ctx, i2s, "unsetup", JS_NewCFunction(ctx, js_i2s_unsetup, "unsetup", 1));
    JS_SetPropertyStr(ctx, i2s, "hasSetup", JS_NewCFunction(ctx, js_i2s_has_setup, "hasSetup", 1));
}

void be_module_serial_i2s_loop(JSContext *ctx) {
}

void be_module_serial_i2s_reset(JSContext *ctx) {
    FREE_BUS_I2S(0)
    FREE_BUS_I2S(1)
    _i2s_bus_setup = 0 ;
}


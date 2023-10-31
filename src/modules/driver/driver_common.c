#include "driver_common.h"
#include "utils.h"
#include "driver/gpio.h"
#include <rom/ets_sys.h>


/**
 * @beapi.driver.common.shift_read
 * 
 * 从移位寄存器芯片设备(74HC165)中读入数据
 * 
 * 所有 gpio 需要在调用前设置好 output 或 input 模式
 * 
 * @param data_pin:number 用于读入数据的 gpio
 * @param clk_pin:number 用于时钟的 gpio
 * @param pl_pin?:number|null=null 用于平行读取控制的 gpio
 * @param ce_pin?:number|null=null clk使能控制 gpio
 * @param bitnum:number=8 读入的位数
 * @param clk_interval_us:number=5 时钟间隔。该参数用于所有 gpio 的电平反转等待，包括 clk_pin,pl_pin,ce_pin
 * @return number
 */
static JSValue js_driver_shift_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, data_pin)
    ARGV_TO_UINT8(1, clk_pin)
    ARGV_TO_INT8_OPT(2, pl_pin, -1)
    ARGV_TO_INT8_OPT(3, ce_pin, -1)
    ARGV_TO_UINT8_OPT(4, bitnum, 8)
    ARGV_TO_UINT8_OPT(5, clk_interval_us, 5)

    #define SET_LEVEL(pin,lv)                   \
        if(gpio_set_level(pin, lv)!=ESP_OK) {   \
            THROW_EXCEPTION("gpio_set_level(%d,%d) faided",pin,lv)      \
        }

    if(ce_pin>-1) {
        SET_LEVEL(ce_pin, 0) ;
        ets_delay_us(clk_interval_us);
    }

    if(pl_pin>-1) {
        SET_LEVEL(pl_pin, 0) ;
        ets_delay_us(clk_interval_us);
        SET_LEVEL(pl_pin, 1) ;
        ets_delay_us(clk_interval_us);
    }

    JSValue data = JS_NewArray(ctx) ;
    uint8_t byte ;

    #define READ_BITS(bit_cnt,byteidx)                  \
        byte = 0 ;                                      \
        for(int b=0;b<bit_cnt;b++) {                    \
            SET_LEVEL(clk_pin, 0) ;                     \
            ets_delay_us(clk_interval_us);              \
            byte|= (gpio_get_level(data_pin)?1:0) << b ;\
            SET_LEVEL(clk_pin, 1) ;                     \
            ets_delay_us(clk_interval_us);              \
        }                                               \
        JS_SetPropertyUint32(ctx, data, byteidx, JS_NewUint32(ctx, byte)) ;

    size_t bytenum = bitnum/8 ;
    for(int i=0;i<bytenum;i++) {
        READ_BITS(8, i)
    }

    size_t remainder = bitnum%8 ;
    if(remainder) {
        READ_BITS(remainder, bytenum)
    }

    if(ce_pin>-1) {
        SET_LEVEL(ce_pin, 1) ;
        ets_delay_us(clk_interval_us);
    }

    return data ;
}


void be_module_driver_common_init() {
}

void be_module_driver_common_require(JSContext *ctx, JSValue pkg) {

    JSValue driver_common = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, pkg, "common", driver_common);

    JS_SetPropertyStr(ctx, driver_common, "shift_read", JS_NewCFunction(ctx, js_driver_shift_read, "shift_read", 1));


}

void be_module_driver_common_loop(JSContext *ctx) {
}

void be_module_driver_common_reset(JSContext *ctx) {
}


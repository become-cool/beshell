#include "WS2812B.hpp"
#include "hal/gpio_types.h"
#include "led_strip.h"
#include "led_strip_rmt.h"
#include <cstdint>

using namespace std ;

namespace be::driver::light{
    DEFINE_NCLASS_META(WS2812B, NativeClass)
    std::vector<JSCFunctionListEntry> WS2812B::methods = {
        JS_CFUNC_DEF("setup", 1, WS2812B::setup),
        JS_CFUNC_DEF("setPixel", 1, WS2812B::setPixel),
        JS_CFUNC_DEF("clear", 1, WS2812B::clear),
        JS_CFUNC_DEF("flush", 1, WS2812B::flush),
    } ;

    WS2812B::WS2812B(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }
    WS2812B::~WS2812B() {
        if(led_strip) {
            led_strip_del(led_strip) ;
            led_strip = nullptr ;
        }
    }

    JSValue WS2812B::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new WS2812B(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue WS2812B::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(WS2812B, that)
        CHECK_ARGC(1)

        // 配置RMT
        GET_UINT32_PROP(argv[0], "length", that->length, )
        if(that->length<1 && that->length>1024) {
            JSTHROW("length must be between 1 and 1024")
        }
        gpio_num_t GET_GPIO_PROP(argv[0], "pin", pin,  )

        // LED strip general initialization, according to your led board design
        led_strip_config_t strip_config = {
            .strip_gpio_num = pin,   // The GPIO that connected to the LED strip's data line
            .max_leds = (uint32_t)that->length,        // The number of LEDs in the strip,
            .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
            .led_model = LED_MODEL_WS2812,            // LED strip model
            // .flags.invert_out = false,                // whether to invert the output signal
        };
        strip_config.flags.invert_out = false ;

        // LED strip backend configuration: RMT
        led_strip_rmt_config_t rmt_config = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            .rmt_channel = 0,
#else
            .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
            .resolution_hz = 10 * 1000 * 1000,     // RMT counter clock frequency
#endif
        };
        // DMA feature is available on ESP target like ESP32-S3
        rmt_config.flags.with_dma = false ;

        // LED Strip object handle
        if(led_strip_new_rmt_device(&strip_config, &rmt_config, &that->led_strip)!= ESP_OK) {
            JSTHROW("Failed to initialize LED strip")
        }

        return JS_UNDEFINED ;
    }

    
    JSValue WS2812B::setPixel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(WS2812B, that)
        CHECK_ARGC(4)
        if(!that->led_strip) {
            JSTHROW("Please call setup() first")
        }

        int32_t idx, red, green, blue;
        if (JS_ToInt32(ctx, &idx, argv[0]) || idx < 0 || idx >= that->length) {
            JSTHROW("Invalid pixel index (must be between 0 and length-1)")
        }
        if (JS_ToInt32(ctx, &red, argv[1]) || red < 0 || red > 255) {
            JSTHROW("Invalid red value (must be between 0 and 255)")
        }
        if (JS_ToInt32(ctx, &green, argv[2]) || green < 0 || green > 255) {
            JSTHROW("Invalid green value (must be between 0 and 255)")
        }
        if (JS_ToInt32(ctx, &blue, argv[3]) || blue < 0 || blue > 255) {
            JSTHROW("Invalid blue value (must be between 0 and 255)")
        }
dn4(idx, red, green, blue)
        led_strip_set_pixel(that->led_strip, idx, red, green, blue);

        return JS_UNDEFINED ;
    }
    
    JSValue WS2812B::clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(WS2812B, that)
        if(!that->led_strip) {
            JSTHROW("Please call setup() first")
        }
        led_strip_clear(that->led_strip) ;
        return JS_UNDEFINED ;
    }
    
    JSValue WS2812B::flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(WS2812B, that)
        if(!that->led_strip) {
            JSTHROW("Please call setup() first")
        }
        led_strip_refresh(that->led_strip) ;
        return JS_UNDEFINED ;
    }
}
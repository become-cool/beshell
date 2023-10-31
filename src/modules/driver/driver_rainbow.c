#include "driver_rainbow.h"
#include "driver/rmt.h"
#include "led_strip.h"
#include "utils.h"
#include <string.h>
#include "module_utils.h"
#include "esp_system.h"


// WS2812 --------------------------------


#define THIS_WS2812(var)                                                        \
    led_strip_t * var = JS_GetOpaque(this_val, js_driver_ws2812_class_id) ;     \
    if(!var) {                                                                  \
        THROW_EXCEPTION("function must be called as method of driver.WS2812") ; \
    }

static JSClassID js_driver_ws2812_class_id ;
static JSValue js_driver_ws2812_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    CHECK_ARGC(3)
    
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, channel)
    if(channel>=8) {
        THROW_EXCEPTION("arg chanel must equal to 0-7")
    }
    ARGV_TO_UINT16(2, led_num)

    rmt_config_t config ;// RMT_DEFAULT_CONFIG_TX(pin, channel);
    memset(&config, 0, sizeof(rmt_config_t));

    config.channel = channel;
    config.rmt_mode = RMT_MODE_TX;
    config.gpio_num = pin;
    config.mem_block_num = 2;
    config.clk_div = 2;
    config.tx_config.loop_en = false;
    config.tx_config.carrier_level = RMT_CARRIER_LEVEL_LOW;
    config.tx_config.carrier_en = false;
    config.tx_config.carrier_duty_percent = 0;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.idle_output_en = true;

    CALL_IDF_API(rmt_config(&config))
    CALL_IDF_API(rmt_driver_install(channel, 0, 0))

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(led_num, (led_strip_dev_t)config.channel);
    led_strip_t * pStrip = led_strip_new_rmt_ws2812(&strip_config);

    JSValue jsobj = JS_NewObjectClass(ctx, js_driver_ws2812_class_id) ;

    JS_SetOpaque(jsobj,pStrip) ;
    
    led_strip_timeline_init(pStrip) ;

    return jsobj ;
}
static void js_driver_ws2812_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_driver_ws2812_finalizer()\n") ;
    
    led_strip_t * thisobj = JS_GetOpaque(this_val, js_driver_ws2812_class_id) ;
    if(thisobj) {
        led_strip_timeline_delete(&thisobj->timeline) ;
        led_strip_denit(thisobj) ;
        thisobj = NULL ;

        JS_SetOpaque(this_val, NULL) ;
    }
}
static JSClassDef js_driver_ws2812_class = {
    "js_full_name",
    .finalizer = js_driver_ws2812_finalizer,
} ;

static JSValue js_driver_ws2812_set_pixel_hsv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, idx)
    ARGV_TO_UINT16(1, h)
    ARGV_TO_UINT8(2, s)
    ARGV_TO_UINT8(3, v)

    uint32_t r,g,b ;
    hsv2rgb(h,s,v,&r,&g,&b) ;

    r = gamma8_correct(r) ;
    g = gamma8_correct(g) ;
    b = gamma8_correct(b) ;
    
    CALL_IDF_API(thisobj->set_pixel(thisobj, idx,r,g,b)) ;

    return JS_UNDEFINED ;
}
static JSValue js_driver_ws2812_set_pixel_rgb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, idx)
    ARGV_TO_UINT8(1, r)
    ARGV_TO_UINT8(2, g)
    ARGV_TO_UINT8(3, b)

    r = gamma8_correct(r) ;
    g = gamma8_correct(g) ;
    b = gamma8_correct(b) ;

    CALL_IDF_API(thisobj->set_pixel(thisobj, idx,r,g,b)) ;

    return JS_UNDEFINED ;
}

static JSValue js_driver_ws2812_fill(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CHECK_ARGC(5)
    ARGV_TO_UINT8(0, x)
    ARGV_TO_UINT8(1, width)
    ARGV_TO_UINT16(2, h)
    ARGV_TO_UINT8(3, s)
    ARGV_TO_UINT8(4, v)
    ARGV_TO_INT16_OPT(5,dh,0)
    ARGV_TO_INT16_OPT(6,ds,0)
    ARGV_TO_INT16_OPT(7,dv,0)

    CALL_IDF_API(thisobj->fill(thisobj, x, width, h,s,v, dh,ds,dv)) ;

    return JS_UNDEFINED ;
}

static JSValue js_driver_ws2812_buffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    return JS_NewArrayBuffer(ctx, thisobj->buffer, thisobj->strip_len * 3, nofreeArrayBuffer, NULL, false) ;
}

static JSValue js_driver_ws2812_refresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CALL_IDF_API(thisobj->refresh(thisobj, 0)) ;
    return JS_UNDEFINED ;
}
static JSValue js_driver_ws2812_clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CALL_IDF_API(thisobj->clear(thisobj, true, 0)) ;
    return JS_UNDEFINED ;
}


static JSValue js_driver_ws2812_timeline_clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    led_strip_timeline_clear(&thisobj->timeline) ;
    return JS_UNDEFINED ;
}
static JSValue js_driver_ws2812_timeline_print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    led_strip_timeline_print(&thisobj->timeline) ;
    return JS_UNDEFINED ;
}
static JSValue js_driver_ws2812_timeline_load(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CHECK_ARGC(1)

    if(thisobj->timeline.playing) {
        THROW_EXCEPTION("timeline is playing, stop first")
    }

    // 清空时间轴
    if(!led_strip_timeline_clear(&thisobj->timeline)) {
        THROW_EXCEPTION("clear timeline failed, maybe it's playing")
    }

    ASSIGN_UINT_PROP_DEFAULT(argv[0],"fps", thisobj->timeline.fps, 60)
    ASSIGN_UINT_PROP_DEFAULT(argv[0],"repeat", thisobj->timeline.repeat, 0)
    ASSIGN_UINT_PROP_DEFAULT(argv[0],"repeat_from", thisobj->timeline.repeat_from, 0)

    JSValue jstracks = JS_GetPropertyStr(ctx, argv[0], "tracks") ;

    char * easeName = NULL ;

    strip_keyframe_t empty_kf ;
    memset(&empty_kf, 0, sizeof(strip_keyframe_t)) ;

    strip_keyframe_t * prev_kf ;

    JS_ForeachArray(jstracks, jstr, {

        strip_track_t * track = led_strip_timeline_create_track(& thisobj->timeline) ;
        
        JSValue jskeyframes = JS_GetPropertyStr(ctx, jstr, "keyframes") ;

        prev_kf = &empty_kf ;

        JS_ForeachArray(jskeyframes, jskf, {

            strip_keyframe_t * kf = led_strip_track_create_keyframe(track) ;

            JSValue jsfrm = JS_GetPropertyStr(ctx, jskf, "frm") ;
            if( JS_IsNumber(jsfrm) ) {
                JS_ToUint32(ctx, &kf->frm, jsfrm) ;
            }
            else {
                size_t dur = 1 ;
                JSValue jsdur = JS_GetPropertyStr(ctx, jskf, "dur") ;
                if( JS_IsNumber(jsdur) ) {
                    JS_ToUint32(ctx, &dur, jsdur) ;
                }                
                kf->frm = prev_kf->frm + dur ;
                JS_FreeValue(ctx,jsdur) ;
            }
            JS_FreeValue(ctx,jsfrm) ;

            ASSIGN_INT_PROP_DEFAULT(jskf,"x", kf->x, prev_kf->x)
            ASSIGN_INT_PROP_DEFAULT(jskf,"width", kf->width, prev_kf->width)
            ASSIGN_UINT_PROP_DEFAULT(jskf,"h", kf->h, prev_kf->h)
            ASSIGN_UINT_PROP_DEFAULT(jskf,"s", kf->s, prev_kf->s)
            ASSIGN_UINT_PROP_DEFAULT(jskf,"v", kf->v, prev_kf->v)
            ASSIGN_INT_PROP_DEFAULT(jskf,"dh", kf->dh, prev_kf->dh)
            ASSIGN_INT_PROP_DEFAULT(jskf,"ds", kf->ds, prev_kf->ds)
            ASSIGN_INT_PROP_DEFAULT(jskf,"dv", kf->dv, prev_kf->dv)

#define ASSIGN_EASE_FUNC(name)                  \
            if(strcmp(easeName, #name)==0) {    \
                kf->ease = name ;               \
            }

            ASSIGN_STR_PROP(jskf,"ease",easeName)
            if(!easeName) {
                kf->ease = Linear ;
            }
            else ASSIGN_EASE_FUNC(Linear)
            else ASSIGN_EASE_FUNC(EaseInSine)
            else ASSIGN_EASE_FUNC(EaseOutSine)
            else ASSIGN_EASE_FUNC(EaseInOutSine)

            else ASSIGN_EASE_FUNC(EaseInQuad)
            else ASSIGN_EASE_FUNC(EaseOutQuad)
            else ASSIGN_EASE_FUNC(EaseInOutQuad)

            else ASSIGN_EASE_FUNC(EaseInCubic)
            else ASSIGN_EASE_FUNC(EaseOutCubic)
            else ASSIGN_EASE_FUNC(EaseInOutCubic)

            else ASSIGN_EASE_FUNC(EaseInQuart)
            else ASSIGN_EASE_FUNC(EaseOutQuart)
            else ASSIGN_EASE_FUNC(EaseInOutQuart)

            else ASSIGN_EASE_FUNC(EaseInQuint)
            else ASSIGN_EASE_FUNC(EaseOutQuint)
            else ASSIGN_EASE_FUNC(EaseInOutQuint)

            else ASSIGN_EASE_FUNC(EaseInExpo)
            else ASSIGN_EASE_FUNC(EaseOutExpo)
            else ASSIGN_EASE_FUNC(EaseInOutExpo)

            else ASSIGN_EASE_FUNC(EaseInCirc)
            else ASSIGN_EASE_FUNC(EaseOutCirc)
            else ASSIGN_EASE_FUNC(EaseInOutCirc)

            else ASSIGN_EASE_FUNC(EaseInBack)
            else ASSIGN_EASE_FUNC(EaseOutBack)
            else ASSIGN_EASE_FUNC(EaseInOutBack)

            else ASSIGN_EASE_FUNC(EaseInElastic)
            else ASSIGN_EASE_FUNC(EaseOutElastic)
            else ASSIGN_EASE_FUNC(EaseInOutElastic)

            else ASSIGN_EASE_FUNC(EaseInBounce)
            else ASSIGN_EASE_FUNC(EaseOutBounce)
            else ASSIGN_EASE_FUNC(EaseInOutBounce)

            else {
                printf("unknow ease function: %s\n", easeName) ;
            }

            if(easeName) {
                JS_FreeCString(ctx, easeName) ;
                easeName = NULL ;
            }

            // ease back and elastic
            if(kf->ease>=EaseInBack) {
                JSValue jsease_s = JS_GetPropertyStr(ctx, jskf, "ease_s") ;
                if( JS_IsNumber(jsease_s) ) {
                    kf->ease_s = 0.0 ;
                    JS_ToFloat64(ctx, &kf->ease_s, jsease_s) ;
                }
                else {
                    kf->ease_s = 1.70158 ;
                }
                JS_FreeValue(ctx, jsease_s) ;
            }


            // printf("hsv=%d,%d,%d, ease:%d\n", kf->h,kf->s,kf->v,kf->ease) ;

            prev_kf = kf ;
            
        })

        JS_FreeValue(ctx,jskeyframes) ;

    })
    JS_FreeValue(ctx,jstracks) ;

    return JS_UNDEFINED ;
}

static JSValue js_driver_ws2812_timeline_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    if(thisobj->timeline.playing) {
        THROW_EXCEPTION("timeline is playing")
    }
    return led_strip_timeline_start(&thisobj->timeline)? JS_TRUE : JS_FALSE;
}
static JSValue js_driver_ws2812_timeline_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    led_strip_timeline_stop(&thisobj->timeline) ;
    return JS_UNDEFINED ;
}
static JSValue js_driver_ws2812_timeline_is_playing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    return JS_ToBool(ctx, thisobj->timeline.playing) ;
}
static JSValue js_driver_ws2812_timeline_draw_frame(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_WS2812(thisobj)
    CHECK_ARGC(1)
    ARGV_TO_UINT32(0, frameIdx) ;
    return JS_ToBool(ctx, led_strip_timeline_render_frame(thisobj, frameIdx)) ;
}

static const JSCFunctionListEntry js_driver_ws2812_proto_funcs[] = {
    JS_CFUNC_DEF("fill", 0, js_driver_ws2812_fill),
    JS_CFUNC_DEF("setPixel", 0, js_driver_ws2812_set_pixel_hsv),
    JS_CFUNC_DEF("setPixelRGB", 0, js_driver_ws2812_set_pixel_rgb),
    JS_CFUNC_DEF("refresh", 0, js_driver_ws2812_refresh),
    JS_CFUNC_DEF("clear", 0, js_driver_ws2812_clear),
    JS_CFUNC_DEF("buffer", 0, js_driver_ws2812_buffer),

    JS_CFUNC_DEF("timelineLoad", 0, js_driver_ws2812_timeline_load),
    JS_CFUNC_DEF("timelinePrint", 0, js_driver_ws2812_timeline_print),
    JS_CFUNC_DEF("timelineStart", 0, js_driver_ws2812_timeline_start),
    JS_CFUNC_DEF("timelineStop", 0, js_driver_ws2812_timeline_stop),
    JS_CFUNC_DEF("timelineClear", 0, js_driver_ws2812_timeline_clear),
    JS_CFUNC_DEF("timelineIsPlaying", 0, js_driver_ws2812_timeline_is_playing),
    JS_CFUNC_DEF("timelineDrawFrame", 0, js_driver_ws2812_timeline_draw_frame),
} ;



void be_module_driver_rainbow_init() {
    JS_NewClassID(&js_driver_ws2812_class_id);
}

void be_module_driver_rainbow_require(JSContext *ctx, JSValue driver) {

    // JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    // JSValue driver_rainbow = JS_NewObject(ctx);
    // JS_SetPropertyStr(ctx, beapi, "driver_rainbow", driver_rainbow);

    // JS_SetPropertyStr(ctx, driver_rainbow, "xxxOoo", JS_NewCFunction(ctx, js_xxx_ooo, "xxxOoo", 1));


    // JS_FreeValue(ctx, beapi);

    
    QJS_DEF_CLASS(driver_ws2812, "WS2812", "driver.WS2812", JS_UNDEFINED, driver)

}

void be_module_driver_rainbow_loop(JSContext *ctx) {
}

void be_module_driver_rainbow_reset(JSContext *ctx) {
}


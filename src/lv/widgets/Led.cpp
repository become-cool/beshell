#include "Led.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Led, Obj)
    std::vector<JSCFunctionListEntry> Led::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("color",be::lv::Obj::invalidGetter,Led::setColor) ,
        JS_CGETSET_DEF("brightness",Led::getBrightness,Led::setBrightness) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("on", 0, Led::jsOn),
        JS_CFUNC_DEF("off", 0, Led::jsOff),
        JS_CFUNC_DEF("toggle", 0, Led::jsToggle),

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Led::Led(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Led::Led(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_led_create(parent))
    {}
        
    JSValue Led::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Led * widget = new Led(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Led::setColor(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Led,thisobj)
        lv_color_t color ;
        if(JS_ToUint32(ctx, (uint32_t *) &color, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","color","Led","setColor","number")
        }
        lv_led_set_color(thisobj->lvobj(), color) ;
        return JS_UNDEFINED ;
    }
    JSValue Led::getBrightness(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Led,thisobj)
        uint8_t value = lv_led_get_brightness(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Led::setBrightness(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Led,thisobj)
        uint8_t brightness ;
        if(JS_ToUint32(ctx, (uint32_t *) &brightness, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","brightness","Led","setBrightness","number")
        }
        lv_led_set_brightness(thisobj->lvobj(), brightness) ;
        return JS_UNDEFINED ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue Led::jsOn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_led_on( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Led::jsOff(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_led_off( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Led::jsToggle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_led_toggle( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

// AUTO GENERATE CODE END [METHODS] --------

}

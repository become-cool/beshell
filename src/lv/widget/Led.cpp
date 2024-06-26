#include "Led.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Led, Obj)
    std::vector<JSCFunctionListEntry> Led::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("color",be::lv::Obj::invalidGetter,Led::setColor) ,
        JS_CGETSET_DEF("brightness",Led::getBrightness,Led::setBrightness) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("on", 0, Led::on),
        JS_CFUNC_DEF("off", 0, Led::off),
        JS_CFUNC_DEF("toggle", 0, Led::toggle),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Led::Led(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Led::build(ctx,jsobj), lvobj)
    {}

    Led::Led(JSContext * ctx, lv_obj_t * parent)
        : Led(ctx, JS_NULL, lv_led_create(parent))
    {}

    JSValue Led::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Led * widget = new Led(ctx, obj, lv_led_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_led_set_color` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Led
     * @property color:number
     *
     */
    #ifndef SETTER_Led_Color
    JSValue Led::setColor(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Led,thisobj)
        lv_color_t color ;
        if(JS_ToUint32(ctx, (uint32_t *) &color, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","color","Led","setColor","number")
        }
        lv_led_set_color(thisobj->lvobj(), color) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_led_get_brightness`, `lv_led_set_brightness` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Led
     * @property brightness:number
     *
     */
    #ifndef GETTER_Led_Brightness
    JSValue Led::getBrightness(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Led,thisobj)
        uint8_t value = lv_led_get_brightness(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Led_Brightness
    JSValue Led::setBrightness(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Led,thisobj)
        uint8_t brightness ;
        if(JS_ToUint32(ctx, (uint32_t *) &brightness, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","brightness","Led","setBrightness","number")
        }
        lv_led_set_brightness(thisobj->lvobj(), brightness) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Led_on
    /**
     * > 该方法是对 LVGL C API `lv_led_on` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Led
     * @method on
     *
     */
    JSValue Led::on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_led_on( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Led_off
    /**
     * > 该方法是对 LVGL C API `lv_led_off` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Led
     * @method off
     *
     */
    JSValue Led::off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_led_off( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Led_toggle
    /**
     * > 该方法是对 LVGL C API `lv_led_toggle` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Led
     * @method toggle
     *
     */
    JSValue Led::toggle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_led_toggle( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}

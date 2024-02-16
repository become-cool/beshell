#include "Bar.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Bar, Obj)
    std::vector<JSCFunctionListEntry> Bar::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("mode",Bar::getMode,Bar::setMode) ,
        JS_CGETSET_DEF("value",Bar::getValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("startValue",Bar::getStartValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("minValue",Bar::getMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxValue",Bar::getMaxValue,be::lv::Obj::invalidSetter) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("isSymmetrical", 0, Bar::jsIsSymmetrical),

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Bar::Bar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Bar::Bar(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_bar_create(parent))
    {}
        
    JSValue Bar::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Bar * widget = new Bar(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Bar::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        lv_bar_mode_t value = lv_bar_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Bar::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Bar,thisobj)
        uint8_t mode ;
        if(JS_ToUint32(ctx, (uint32_t *) &mode, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","mode","Bar","setMode","number")
        }
        lv_bar_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    JSValue Bar::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Bar::getStartValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_start_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Bar::getMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Bar::getMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue Bar::jsIsSymmetrical(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            bool retval = lv_bar_is_symmetrical( thisobj->lvobj() ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

// AUTO GENERATE CODE END [METHODS] --------

}

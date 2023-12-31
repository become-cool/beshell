#include "indev.h"
#include "display.h"
#include "widgets_gen.h"
#include "utils.h"
#include "cutils.h"
#include "indev_pointer.h"
#include "indev_i2c.h"




uint8_t _indev_id = 0 ;

indev_driver_spec_t * find_indev_spec_by_id(uint8_t id) {
    lv_indev_t * indev=NULL ;
    while((indev=lv_indev_get_next(indev))) {
        if( !indev->driver || !indev->driver->user_data ) {
            continue;
        }
        if( ((indev_driver_spec_t *)indev->driver->user_data)->id == id ){
            return indev->driver->user_data ;
        }
    }
    return NULL ;
}

static JSValue js_find_indev_by_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    ARGV_TO_UINT8(0,id)

    indev_driver_spec_t * spec = find_indev_spec_by_id(id) ;

    if(!spec || /*!spec->jsobj ||*/ JS_IsUndefined(spec->jsobj) || JS_IsNull(spec->jsobj)) {
        return JS_NULL ;
    }
    return JS_DupValue(ctx,JS_MKPTR(JS_TAG_OBJECT,JS_VALUE_GET_PTR(spec->jsobj))) ;

}

static JSValue js_all_indev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    JSValue ret = JS_NewArray(ctx) ;
    int idx = 0 ;

    lv_indev_t * indev=NULL ;
    while((indev=lv_indev_get_next(indev))) {
        if( !indev->driver || !indev->driver->user_data ) {
            continue;
        }
        JSValue jsobj = ((indev_driver_spec_t *)indev->driver->user_data)->jsobj ;
        if(/*!jsobj ||*/ JS_IsUndefined(jsobj) || JS_IsNull(jsobj)) {
            continue;
        }
#ifdef PLATFORM_ESP32
        JS_SetPropertyUint32(ctx, ret, idx++, JS_DupValue(ctx,JS_MKPTR(JS_TAG_OBJECT,jsobj))) ;
#else
        JS_SetPropertyUint32(ctx, ret, idx++, JS_DupValue(ctx,jsobj)) ;
#endif
    }

    return ret ;
}

inline void indev_emit_js_event(indev_driver_spec_t * indev_spec, const char * event_name, const char * key) {
    
    JSValueConst * argv ;
    int argc = 0 ;
    if(key) {
        argc = 2 ;
        MALLOC_ARGV2(argv
            , JS_NewString(indev_spec->ctx, event_name)
            , JS_NewString(indev_spec->ctx, key)
        )
    }
    else {
        argc = 1 ;
        MALLOC_ARGV1(argv
            , JS_NewString(indev_spec->ctx, event_name)
        )
    }

    // 在 lvgl 屏幕对象上触发事件
    if ( indev_spec->lv_indev
            && indev_spec->lv_indev->driver
            && indev_spec->lv_indev->driver->disp
            && indev_spec->lv_indev->driver->disp->driver
            && indev_spec->lv_indev->driver->disp->driver->user_data
    ){
        disp_drv_spec_t * disp_spec = (disp_drv_spec_t *)indev_spec->lv_indev->driver->disp->driver->user_data ;

        if(disp_spec->enable_input_event) {
            JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT, disp_spec->jsobj) ;
            JSValue emit = JS_GetPropertyStr(disp_spec->ctx,jsobj,"emit") ;
            if( JS_IsFunction(indev_spec->ctx, emit) ) {
                JSValue ret = JS_Call(indev_spec->ctx, emit, jsobj, argc, argv ) ;
                if (JS_IsException(ret)) {
                    js_std_dump_error(indev_spec->ctx);
                }
            }
            JS_FreeValue(indev_spec->ctx, emit);
        }
        
    }

    // 在 indev 对象上触发事件
    if( indev_spec->enable_jsobj_event && !JS_IsUndefined(indev_spec->jsobj) && !JS_IsNull(indev_spec->jsobj) ){
#ifdef PLATFORM_ESP32
        JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT, indev_spec->jsobj) ;
#else
        JSValue jsobj = indev_spec->jsobj ;
#endif
        JSValue emit = JS_GetPropertyStr(indev_spec->ctx,jsobj,"emit") ;
        if( JS_IsFunction(indev_spec->ctx, emit) ) {
            JSValue ret = JS_Call(indev_spec->ctx, emit, jsobj, argc, argv ) ;
            if (JS_IsException(ret)) {
                js_std_dump_error(indev_spec->ctx);
            }
        }
        JS_FreeValue(indev_spec->ctx, emit);
    }

    free(argv) ;
}

JSContext * js_indev_global_cb_ctx = NULL ;
JSValue js_indev_global_cb_pressed ;
JSValue js_indev_global_cb_released ;
JSValue js_indev_global_cb_pressing ;

static JSValue js_set_indev_global_cb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_STRING(0, event) ;
    if( !JS_IsFunction(ctx, argv[1]) ) {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("arg callback must be a function")
    }
    if( strcmp("pressed", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_pressed = JS_DupValue(ctx, argv[1]) ;
    }
    else if( strcmp("pressing", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_pressing = JS_DupValue(ctx, argv[1]) ;
    }
    else if( strcmp("released", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_released = JS_DupValue(ctx, argv[1]) ;
    }
    else {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("unknow event")
    }

    JS_FreeCString(ctx, event) ;

    return JS_UNDEFINED ;
}

static JSValue js_clear_indev_global_cb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, event) ;
    if( strcmp("pressed", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_pressed) ;
        js_indev_global_cb_pressed = JS_UNDEFINED ;
    }
    else if( strcmp("pressing", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_pressing) ;
        js_indev_global_cb_pressing = JS_UNDEFINED ;
    }
    else if( strcmp("released", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_released) ;
        js_indev_global_cb_released = JS_UNDEFINED ;
    }
    else {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("unknow event")
    }

    JS_FreeCString(ctx, event) ;
    return JS_UNDEFINED ;
}


bool indev_last_pressed = false ;
void indev_global_cb_proc(lv_indev_data_t *data) {
    if( indev_last_pressed==false ) {
        // pressed
        if(data->state==LV_INDEV_STATE_PRESSED) {
            if( !JS_IsUndefined(js_indev_global_cb_pressed) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_pressed, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        }
    } else {
        if(data->state == LV_INDEV_STATE_PRESSED) {
            if( !JS_IsUndefined(js_indev_global_cb_pressing) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_pressing, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        } else {
            if( !JS_IsUndefined(js_indev_global_cb_released) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_released, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        }
    }
    indev_last_pressed = (data->state==LV_INDEV_STATE_PRESSED) ;
}




// ----------------------------------------------------------------
// IndevBase

static JSClassID js_indev_base_class_id ;

static JSValue js_indev_base_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_indev_base_class_id) ;
    return jsobj ;
}
static void js_indev_base_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_indev_base_finalizer()\n") ;
}
static JSClassDef js_indev_base_class = {
    "lv.IndevBase",
    .finalizer = js_indev_base_finalizer,
} ;


#ifdef PLATFORM_ESP32
static JSValue js_lv_indev_tick(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_INDEV(thisobj)
    lv_indev_read_timer_cb(thisobj->driver->read_timer) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_indev_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_INDEV(thisobj)
    return JS_NewUint32(ctx, ((indev_driver_spec_t*)thisobj->driver->user_data)->id) ;
}

static JSValue js_lv_indev_set_group(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_group_t * group = JS_GetOpaque(argv[0], lv_group_js_class_id()) ;
    if(!group) {
        THROW_EXCEPTION("invalid arg group")
    }
    THIS_INDEV(thisobj)
    lv_indev_set_group(thisobj, group) ;
    return JS_UNDEFINED ;
}
#endif

static JSValue js_lv_indev_unregister(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THROW_EXCEPTION("not implements yet") ;
}




static const JSCFunctionListEntry js_indev_base_proto_funcs[] = {
#ifdef PLATFORM_ESP32
    JS_CFUNC_DEF("tick", 0, js_lv_indev_tick),
    JS_CFUNC_DEF("id", 0, js_lv_indev_id),
    JS_CFUNC_DEF("setGroup", 0, js_lv_indev_set_group),
#endif
    JS_CFUNC_DEF("unregister", 0, js_lv_indev_unregister),
} ;




void be_lv_indev_reset(JSContext * ctx) {

    js_indev_global_cb_ctx = NULL ;

    JS_FreeValue(ctx, js_indev_global_cb_pressed) ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_released) ;
    js_indev_global_cb_released = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_pressing) ;
    js_indev_global_cb_pressing = JS_UNDEFINED;

}


void be_lv_indev_init() {
    JS_NewClassID(&js_indev_base_class_id);
    // JS_NewClassID(&js_indev_fake_key_class_id);

    be_indev_pointer_init() ;

#ifdef PLATFORM_ESP32
    be_indev_i2c_init() ;
#endif
}


void be_lv_indev_require(JSContext *ctx, JSValue lvgl) {
    
    js_indev_global_cb_ctx = NULL ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    js_indev_global_cb_released = JS_UNDEFINED;
    js_indev_global_cb_pressing = JS_UNDEFINED;

    JS_SetPropertyStr(ctx, lvgl, "setIndevCallback", JS_NewCFunction(ctx, js_set_indev_global_cb, "setIndevCallback", 1));
    JS_SetPropertyStr(ctx, lvgl, "clearIndevCallback", JS_NewCFunction(ctx, js_clear_indev_global_cb, "clearIndevCallback", 1));
    JS_SetPropertyStr(ctx, lvgl, "allIndev", JS_NewCFunction(ctx, js_all_indev, "allIndev", 1));
    JS_SetPropertyStr(ctx, lvgl, "findIndevById", JS_NewCFunction(ctx, js_find_indev_by_id, "findIndevById", 1));
    
    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    JSValue baseProto = QJS_DEF_CLASS(indev_base, "IndevBase", "lv.IndevBase", EventEmitterProto, lvgl) ;
    JS_FreeValue(ctx, EventEmitterProto);

    // QJS_DEF_CLASS(indev_fake_key, "IndevFakeKey", "lv.IndevFakeKey", baseProto, lvgl)

    be_indev_pointer_require(ctx, lvgl, baseProto) ;
#ifdef PLATFORM_ESP32
    be_indev_i2c_require(ctx, lvgl, baseProto) ;
#endif
}

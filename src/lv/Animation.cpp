#include "Animation.hpp"

using namespace std ;

namespace be::lv {
    DEFINE_NCLASS_META(Animation, NativeClass)
    std::vector<JSCFunctionListEntry> Animation::methods = {
        JS_CFUNC_DEF("setTarget", 1, Animation::setTarget),
        JS_CFUNC_DEF("setTime", 1, Animation::setTime),
        JS_CFUNC_DEF("setValues", 0, Animation::setValues),
        JS_CFUNC_DEF("setRepeatCount", 0, Animation::setRepeatCount),
        JS_CFUNC_DEF("setPlaybackDelay", 0, Animation::setPlaybackDelay),
        JS_CFUNC_DEF("setPlaybackTime", 0, Animation::setPlaybackTime),
        JS_CFUNC_DEF("setPath", 0, Animation::setPath),
        JS_CFUNC_DEF("setProp", 0, Animation::setProp),
        JS_CFUNC_DEF("start", 0, Animation::start),
    } ;

    Animation::Animation(JSContext * ctx, Obj * target)
        : NativeClass(ctx,build(ctx))
    {
        lv_anim_init(&lvanim) ;
        if(target) {
            lv_anim_set_var(&lvanim, target) ;
        }
    }

    JSValue Animation::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        JSVALUE_TO_NCLASS(Obj,argv[0],target)
        auto obj = new Animation(ctx,target) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue Animation::setTarget(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Animation,that)
        JSVALUE_TO_NCLASS(Obj,argv[0],target)
        lv_anim_set_var(&that->lvanim, target->lvobj()) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Animation::setTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Animation,that)
        ARGV_TO_UINT32(0,time)
        lv_anim_set_time(&that->lvanim, time) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Animation::setValues(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        THIS_NCLASS(Animation,that)
        ARGV_TO_INT32(0,startValue)
        ARGV_TO_INT32(1,endValue)
        lv_anim_set_values(&that->lvanim, startValue, endValue) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Animation::setRepeatCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Animation,that)
        ARGV_TO_UINT32(0,repeat)
        lv_anim_set_repeat_count(&that->lvanim, repeat) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Animation::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Animation,that)
        ARGV_TO_UINT32(0,repeat)
        lv_anim_start(&that->lvanim) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Animation::setPlaybackDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation,that)
        ARGV_TO_UINT32(0,delay)
        lv_anim_set_playback_delay(&that->lvanim, delay) ;
        return JS_UNDEFINED ;
    }
    JSValue Animation::setPlaybackTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation,that)
        ARGV_TO_UINT32(0,delay)
        lv_anim_set_playback_delay(&that->lvanim, delay) ;
        return JS_UNDEFINED ;
    }

    
    JSValue Animation::setPath(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation,that)
        ARGV_TO_CSTRING(0, pathName)

        if( strcmp(pathName, "linear") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_linear);
        }
        else if( strcmp(pathName, "ease-in") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_ease_in);
        }
        else if( strcmp(pathName, "ease-out") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_ease_out);
        }
        else if( strcmp(pathName, "ease-in-out") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_ease_in_out);
        }
        else if( strcmp(pathName, "overshoot") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_overshoot);
        }
        else if( strcmp(pathName, "bounce") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_bounce);
        }
        else if( strcmp(pathName, "step") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_step);
        }
        else if( strcmp(pathName, "bezier3") == 0 ) {
            lv_anim_set_path_cb(&that->lvanim, lv_anim_path_custom_bezier3);
        }

        JS_FreeCString(ctx, pathName) ;
        return JS_UNDEFINED ;
    }

    JSValue Animation::setProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation,that)
        ARGV_TO_CSTRING(0, propName)
        
        if( strcmp(propName, "x") == 0 ) {
            lv_anim_set_exec_cb(&that->lvanim, (lv_anim_exec_xcb_t)lv_obj_set_x);
        }
        else if( strcmp(propName, "y") == 0 ) {
            lv_anim_set_exec_cb(&that->lvanim, (lv_anim_exec_xcb_t)lv_obj_set_y);
        }
        else if( strcmp(propName, "width") == 0 ) {
            lv_anim_set_exec_cb(&that->lvanim, (lv_anim_exec_xcb_t)lv_obj_set_width);
        }
        else if( strcmp(propName, "height") == 0 ) {
            lv_anim_set_exec_cb(&that->lvanim, (lv_anim_exec_xcb_t)lv_obj_set_height);
        }

        JS_FreeCString(ctx, propName) ;
        return JS_UNDEFINED ;
    }
}
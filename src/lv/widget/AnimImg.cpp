#include "AnimImg.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(AnimImg, Obj)
    std::vector<JSCFunctionListEntry> AnimImg::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("duration",AnimImg::getDuration,AnimImg::setDuration) ,
        JS_CGETSET_DEF("repeatCount",AnimImg::getRepeatCount,AnimImg::setRepeatCount) ,
        JS_CGETSET_DEF("srcCount",AnimImg::getSrcCount,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("start", 0, AnimImg::start),
        // Unsupported arg type:
        // void lv_animimg_set_src(lv_obj_t * img, const void * dsc[], size_t num)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    AnimImg::AnimImg(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, AnimImg::build(ctx,jsobj), lvobj)
    {}

    AnimImg::AnimImg(JSContext * ctx, lv_obj_t * parent)
        : AnimImg(ctx, JS_NULL, lv_animimg_create(parent))
    {}

    JSValue AnimImg::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        AnimImg * widget = new AnimImg(ctx, obj, lv_animimg_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_animimg_get_duration`, `lv_animimg_set_duration` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class AnimImg
     * @property duration:number
     *
     */
    #ifndef GETTER_AnimImg_Duration
    JSValue AnimImg::getDuration(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(AnimImg,thisobj)
        uint32_t value = lv_animimg_get_duration(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_AnimImg_Duration
    JSValue AnimImg::setDuration(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(AnimImg,thisobj)
        uint32_t duration ;
        if(JS_ToUint32(ctx, (uint32_t *) &duration, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","duration","AnimImg","setDuration","number")
        }
        lv_animimg_set_duration(thisobj->lvobj(), duration) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_animimg_get_repeat_count`, `lv_animimg_set_repeat_count` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class AnimImg
     * @property repeatCount:number
     *
     */
    #ifndef GETTER_AnimImg_RepeatCount
    JSValue AnimImg::getRepeatCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(AnimImg,thisobj)
        uint32_t value = lv_animimg_get_repeat_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_AnimImg_RepeatCount
    JSValue AnimImg::setRepeatCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(AnimImg,thisobj)
        uint32_t repeatCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &repeatCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","repeatCount","AnimImg","setRepeatCount","number")
        }
        lv_animimg_set_repeat_count(thisobj->lvobj(), repeatCount) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const void **
    // JSValue AnimImg::getSrc(JSContext *ctx, JSValueConst this_val){}
    // const void ** lv_animimg_get_src(lv_obj_t * img)
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_animimg_get_src_count` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class AnimImg
     * @property srcCount:number
     *
     */
    #ifndef GETTER_AnimImg_SrcCount
    JSValue AnimImg::getSrcCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(AnimImg,thisobj)
        uint8_t value = lv_animimg_get_src_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: const void **
    // void lv_animimg_set_src(lv_obj_t * img, const void * dsc[], size_t num)

    #ifndef METHOD_AnimImg_start
    /**
     * > 该方法是对 LVGL C API `lv_animimg_start` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class AnimImg
     * @method start
     *
     */
    JSValue AnimImg::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_animimg_start( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}

#include <BeShell.hpp>
#include "Gif.hpp"
// #include "lv_gif.h"

using namespace std ;

namespace be::lv {
    DEFINE_NCLASS_META(Gif, Img)
    std::vector<JSCFunctionListEntry> Gif::methods = {
        JS_CGETSET_DEF("src",Img::getSrc,Gif::setSrc) ,
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("restart", 0, Gif::restart),
        JS_CFUNC_DEF("pause", 0, Gif::pause),
        JS_CFUNC_DEF("resume", 0, Gif::resume),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Gif::Gif(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Img(ctx, Gif::build(ctx,jsobj), lvobj)
    {}

    Gif::Gif(JSContext * ctx, lv_obj_t * parent)
        : Gif(ctx, JS_NULL, lv_gif_create(parent))
    {}

    JSValue Gif::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Gif * widget = new Gif(ctx, obj, lv_gif_create(lvparent)) ;
        return widget->jsobj ;
    }

    JSValue Gif::setSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value){
        THIS_NCLASS(Img,thisobj)
        const char * src = (char *)JS_ToCString(ctx, value) ;

        if(!FS::exist(src)){
            JSTHROW("path not exists")
        }

        string path = string("C:") + FS::toVFSPath(src) ;
        JS_FreeCString(ctx, src) ;

        lv_gif_set_src(thisobj->lvobj(), path.c_str()) ;
        return JS_UNDEFINED ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const void *
    // JSValue Gif::setSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_gif_set_src(lv_obj_t * obj, const void * src)
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Gif_restart
    /**
     * > 该方法是对 LVGL C API `lv_gif_restart` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Gif
     * @method restart
     *
     */
    JSValue Gif::restart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_gif_restart( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Gif_pause
    /**
     * > 该方法是对 LVGL C API `lv_gif_pause` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Gif
     * @method pause
     *
     */
    JSValue Gif::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_gif_pause( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Gif_resume
    /**
     * > 该方法是对 LVGL C API `lv_gif_resume` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Gif
     * @method resume
     *
     */
    JSValue Gif::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_gif_resume( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}

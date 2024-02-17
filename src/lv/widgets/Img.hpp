#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Img: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Img(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Img(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getOffsetX(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setOffsetX(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getOffsetY(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setOffsetY(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getRotation(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScale(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScale(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScaleX(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScaleX(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScaleY(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScaleY(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getBlendMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setBlendMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getAntialias(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setAntialias(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getAlign(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsSrcGetType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBufSetPalette(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBufFree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderGetInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderOpen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderGetArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderClose(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderDelete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderGetNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderSetInfoCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderSetOpenCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderSetGetAreaCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderSetCloseCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderSetCacheFreeCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderAddToCache(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDecoderPostProcess(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCacheDrop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}

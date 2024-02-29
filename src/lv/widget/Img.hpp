#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Img: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    protected:
        Img(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Img(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue getSrc(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

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
        static JSValue srcGetType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bufSetPalette(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bufFree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderGetInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderOpen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderGetArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderClose(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderDelete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderGetNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderSetInfoCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderSetOpenCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderSetGetAreaCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderSetCloseCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderSetCacheFreeCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderAddToCache(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decoderPostProcess(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue cacheDrop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getPivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    friend class Obj ;
    } ;

}

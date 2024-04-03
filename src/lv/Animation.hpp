#pragma once

#include <NativeClass.hpp>
#include "widget/Obj.hpp"

namespace be::lv{
    class Animation: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        lv_anim_t lvanim ;

    public:
        Animation(JSContext * ctx, Obj * target=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        static JSValue setTarget(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setValues(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setRepeatCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPlaybackDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPlaybackTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPath(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}
#pragma once

#include <NativeClass.hpp>

namespace be::media {
    class MP3Player: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        MP3Player(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue playPCM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue playMP3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isPaused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue printStats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}
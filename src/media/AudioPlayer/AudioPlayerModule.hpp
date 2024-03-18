#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::media{
    class AudioPlayerModule: public be::NativeModule {
    public:
        AudioPlayerModule(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<AudioPlayerModule>("AudioPlayer") ;
        }

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

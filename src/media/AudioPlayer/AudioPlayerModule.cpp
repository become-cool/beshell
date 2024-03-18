#include "AudioPlayerModule.hpp"

namespace be::media{
    AudioPlayerModule::AudioPlayerModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("playPCM",playPCM,0) ;
        exportFunction("playMP3",playMP3,0) ;
        exportFunction("pause",pause,0) ;
        exportFunction("resume",resume,0) ;
        exportFunction("stop",stop,0) ;
        exportFunction("isRunning",isRunning,0) ;
        exportFunction("isPaused",isPaused,0) ;
        exportFunction("detach",detach,0) ;
        exportFunction("printStats",printStats,0) ;
    }

    JSValue AudioPlayerModule::playPCM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::playMP3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::isPaused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayerModule::printStats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}
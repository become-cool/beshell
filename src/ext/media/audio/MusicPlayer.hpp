#pragma once

#include <NativeClass.hpp>
#include "stream/audio_stream.h"

namespace be::media {
    class MusicPlayer: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        audio_pipe_t pipe ;

        audio_el_src_t * src = nullptr ;
        audio_el_mp3_t * mp3 = nullptr ;
        audio_el_i2s_t * playback = nullptr ;

        void build_el_mp3(int core) ;
        void build_el_src(int core) ;
        void build_el_i2s(int core) ;

    public:
        MusicPlayer(JSContext * ctx) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        ~MusicPlayer() ;

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
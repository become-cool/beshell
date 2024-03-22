#include "MusicPlayer.hpp"
#include <fs/FS.hpp>

using namespace std ;

namespace be::media {
    DEFINE_NCLASS_META(MusicPlayer, NativeClass)
    std::vector<JSCFunctionListEntry> MusicPlayer::methods = {
        JS_CFUNC_DEF("playPCM", 0, MusicPlayer::playPCM),
        JS_CFUNC_DEF("playMP3", 0, MusicPlayer::playMP3),
        JS_CFUNC_DEF("pause", 0, MusicPlayer::pause),
        JS_CFUNC_DEF("resume", 0, MusicPlayer::resume),
        JS_CFUNC_DEF("stop", 0, MusicPlayer::stop),
        JS_CFUNC_DEF("isRunning", 0, MusicPlayer::isRunning),
        JS_CFUNC_DEF("isPaused", 0, MusicPlayer::isPaused),
        JS_CFUNC_DEF("detach", 0, MusicPlayer::detach),
        JS_CFUNC_DEF("printStats", 0, MusicPlayer::printStats),
    } ;

    MusicPlayer::MusicPlayer(JSContext * ctx)
        : NativeClass(ctx,build(ctx))
    {
        memset((void*)&pipe, 0, sizeof(audio_pipe_t)) ;

        pipe.jsobj = JS_DupValue(ctx, jsobj) ;
        pipe.ctx = ctx ;
    }
    JSValue MusicPlayer::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new MusicPlayer(ctx) ;
        obj->self = std::shared_ptr<MusicPlayer> (obj) ;
        return obj->jsobj ;
    }
    MusicPlayer::~MusicPlayer() {
        if(src) {
            audio_el_src_delete(src) ;
        }
        if(mp3) {
            audio_el_mp3_delete(mp3) ;
        }
        if(playback) {
            audio_el_i2s_delete(playback) ;
        }

        JS_FreeValue(pipe.ctx, pipe.jsobj) ;
        pipe.jsobj = JS_NULL ;
    }

    
    void MusicPlayer::build_el_src(int core) {
        if(!src) {
            src = audio_el_src_create(&pipe, core) ;
        }
    }
    void MusicPlayer::build_el_mp3(int core) {
        if(!mp3) {
            mp3 = audio_el_mp3_create(&pipe,core) ;
            mp3->i2s = 0 ;
        }
    }
    void MusicPlayer::build_el_i2s(int core) {
        if(!playback) {
            playback = audio_el_i2s_create(&pipe, 0, core) ;
        }
    }
    
    JSValue MusicPlayer::playMP3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MusicPlayer, player)
        if(player->pipe.running) {
            JSTHROW("player is running")
        }
        CHECK_ARGC(1)
        string path = be::FS::toVFSPath(ctx, argv[0]) ;
        player->build_el_src(1) ;
        player->build_el_mp3(1) ;
        player->build_el_i2s(0) ;

        bool sync = false ;
        if(argc>1) {
            sync = JS_ToBool(ctx, argv[1]);
        }

        if(path.length()>=sizeof(player->src->src_path)) {
            JSTHROW("path is too long")
        }
        strcpy(player->src->src_path, path.c_str()) ;

        if(!audio_el_src_strip_mp3(player->src)) {
            JSTHROW("file not exists") ;
        }

        // 重置 hexli 状态
        if(xEventGroupGetBits(player->mp3->base.stats) & STAT_RUNNING) {
            JSTHROW("decoder not close yet")
        }
        audio_el_mp3_reset(player->mp3) ;

        // 清空管道
        audio_pipe_clear(&player->pipe) ;

        audio_pipe_link( &player->pipe, 3, player->src, player->mp3, player->playback ) ;

        player->pipe.paused = false ;
        player->pipe.running = true ;
        player->pipe.finished = false ;
        player->pipe.error = 0 ;
        audio_pipe_emit_js(&player->pipe, "play", JS_UNDEFINED) ;

        audio_pipe_set_stats(&player->pipe, STAT_RUNNING) ;

        if(sync) {
            xEventGroupWaitBits(player->playback->base.stats, STAT_STOPPED, false, false, portMAX_DELAY);
        }

        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::playPCM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MusicPlayer, player)
        return player->pipe.running? JS_TRUE : JS_FALSE ;
    }
    JSValue MusicPlayer::isPaused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MusicPlayer, player)
        return player->pipe.paused? JS_TRUE : JS_FALSE ;
    }
    JSValue MusicPlayer::detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MusicPlayer::printStats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}
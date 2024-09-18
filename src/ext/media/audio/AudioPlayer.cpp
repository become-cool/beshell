#include "AudioPlayer.hpp"
#include <fs/FS.hpp>

using namespace std ;

namespace be::media {
    DEFINE_NCLASS_META(AudioPlayer, EventEmitter)
    std::vector<JSCFunctionListEntry> AudioPlayer::methods = {
        JS_CFUNC_DEF("playPCM", 0, AudioPlayer::playPCM),
        JS_CFUNC_DEF("playMP3", 0, AudioPlayer::playMP3),
        JS_CFUNC_DEF("pause", 0, AudioPlayer::pause),
        JS_CFUNC_DEF("resume", 0, AudioPlayer::resume),
        JS_CFUNC_DEF("stop", 0, AudioPlayer::stop),
        JS_CFUNC_DEF("isRunning", 0, AudioPlayer::isRunning),
        JS_CFUNC_DEF("isPaused", 0, AudioPlayer::isPaused),
        JS_CFUNC_DEF("printStats", 0, AudioPlayer::printStats),
    } ;

    AudioPlayer::AudioPlayer(JSContext * ctx)
        : EventEmitter(ctx,build(ctx))
    {
        memset((void*)&pipe, 0, sizeof(audio_pipe_t)) ;

        pipe.callback = (audio_pipe_event_callback_t) pipeCallback ;

        // pipe.jsobj = JS_DupValue(ctx, jsobj) ;
        // pipe.ctx = ctx ;
    }
    JSValue AudioPlayer::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new AudioPlayer(ctx) ;
        obj->self = std::shared_ptr<AudioPlayer> (obj) ;
        return obj->jsobj ;
    }
    AudioPlayer::~AudioPlayer() {
        if(src) {
            audio_el_src_delete(src) ;
        }
        if(mp3) {
            audio_el_mp3_delete(mp3) ;
        }
        if(playback) {
            audio_el_i2s_delete(playback) ;
        }

        // JS_FreeValue(pipe.ctx, pipe.jsobj) ;
        // pipe.jsobj = JS_NULL ;
    }

    void AudioPlayer::pipeCallback(const char * event, int param, AudioPlayer * player) {

    }
    
    void AudioPlayer::build_el_src(int core) {
        if(!src) {
            src = audio_el_src_create(&pipe, core) ;
        }
    }
    void AudioPlayer::build_el_mp3(int core) {
        if(!mp3) {
            mp3 = audio_el_mp3_create(&pipe,core) ;
            mp3->i2s = 0 ;
        }
    }
    void AudioPlayer::build_el_i2s(int core) {
        if(!playback) {
            playback = audio_el_i2s_create(&pipe, 0, core) ;
        }
    }
    
    JSValue AudioPlayer::playMP3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AudioPlayer, player)
        if(player->pipe.running) {
            JSTHROW("player is running")
        }
        ASSERT_ARGC(1)
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
        // audio_pipe_emit_js(&player->pipe, "play", JS_UNDEFINED) ;

        audio_pipe_set_stats(&player->pipe, STAT_RUNNING) ;

        if(sync) {
            xEventGroupWaitBits(player->playback->base.stats, STAT_STOPPED, false, false, portMAX_DELAY);
        }

        return JS_UNDEFINED ;
    }
    JSValue AudioPlayer::playPCM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayer::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        THIS_NCLASS(AudioPlayer, player)

        audio_pipe_clear_stats(&player->pipe, STAT_RUNNING) ;
        audio_pipe_set_stats(&player->pipe, STAT_PAUSING) ;
        player->pipe.paused = true ;
        // audio_pipe_emit_js(&player->pipe, "pause", JS_UNDEFINED) ;
        
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayer::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AudioPlayer, player)
        if(!player->pipe.paused) {
            return JS_UNDEFINED ;
        }
        audio_pipe_clear_stats(&player->pipe, STAT_PAUSED) ;
        audio_pipe_clear_stats(&player->pipe, STAT_PAUSING) ;
        audio_pipe_set_stats(&player->pipe, STAT_RUNNING) ;
        player->pipe.paused = false ;
        // audio_pipe_emit_js(player, "resume", JS_UNDEFINED) ;
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayer::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AudioPlayer, player)
        if(!player->pipe.running) {
            return JS_UNDEFINED ;
        }
    
        audio_pipe_set_stats(&player->pipe, STAT_RUNNING) ;
        audio_el_set_stat(player->pipe.first, STAT_STOPPING) ;
        vTaskDelay(0) ;
        
        return JS_UNDEFINED ;
    }
    JSValue AudioPlayer::isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AudioPlayer, player)
        return player->pipe.running? JS_TRUE : JS_FALSE ;
    }
    JSValue AudioPlayer::isPaused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AudioPlayer, player)
        return player->pipe.paused? JS_TRUE : JS_FALSE ;
    }

    JSValue AudioPlayer::printStats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        THIS_NCLASS(AudioPlayer, player)

        for( audio_el_t * el = player->pipe.first; el; el=el->downstream) {
            printf("\n[%s]\n", el->name? el->name: "unknow el") ;
            audio_el_print_stats(el) ;
        }

        return JS_UNDEFINED ;
    }
}
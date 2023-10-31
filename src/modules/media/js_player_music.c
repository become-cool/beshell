#include "js_player_music.h"

#include <stdio.h>
#include <string.h>
#include "module_fs.h"
// #include "widgets_gen.h"
#include "cutils.h"
#include "utils.h"
#include "eventloop.h"

#include "audio_stream.h"

#ifdef PLATFORM_ESP32

#else

// fake for pc simulation 
#define configTICK_RATE_HZ 10
#define portTICK_PERIOD_MS 10

#define portMAX_DELAY 0
#define xQueueReceive(a,b,c)
#define xRingbufferSend(a,b,c,d) pdTRUE
#define xRingbufferPrintInfo(a)
#define xRingbufferReceiveUpTo(a,b,c,d) NULL
#define vRingbufferReturnItem(a,b)
#define vTaskDelay(a)
#define xRingbufferCreate(a,b) NULL ;
#define vRingbufferDelete(a) ;
#define xQueueCreate(a,b) NULL ;
#define vQueueDelete(a) ;
#define xTaskCreatePinnedToCore(a,b,c,d,e,f,g)
#define vTaskDelete(a) ;
#define xQueueSend(a,b,c) ;

#define echo_DMA(a) ;

#define i2s_set_clk(a,b,c,d)
#define i2s_zero_dma_buffer(a)
#define i2s_write_expand(a,b,c,d,e,f,g)

typedef void * QueueHandle_t;
typedef void * TaskHandle_t;
typedef void * RingbufHandle_t;

typedef int8_t BaseType_t;
typedef uint8_t UBaseType_t;

enum {
    RINGBUF_TYPE_BYTEBUF
} ;

typedef void (*TaskFunction_t)(void * d);

#define pdPASS 0
#define pdTRUE 1

#define MALLOC_CAP_DMA 0
#define heap_caps_malloc(s, a) malloc(s)

#endif

typedef struct {
    audio_pipe_t base ;

    audio_el_src_t * src ;
    audio_el_mp3_t * mp3 ;
    audio_el_i2s_t * playback ;

    audio_el_spi_keyboard_t * keyboard ;
    audio_el_midi_render_t * midi ;

} player_t ;

#define THIS_PLAYER(thisobj)                                                \
    player_t * thisobj = JS_GetOpaque(this_val, js_audio_player_class_id) ; \
    if(!thisobj) {                                                          \
        THROW_EXCEPTION("must be called as a Player method")                \
    }


static inline void build_el_src(player_t * player, int core) {
    if(!player->src) {
        player->src = audio_el_src_create(player, core) ;
    }
}
static inline void build_el_mp3(player_t * player, uint8_t core) {
    if(!player->mp3) {
        player->mp3 = audio_el_mp3_create(player,core) ;
        player->mp3->i2s = 0 ;
    }
}
static inline void build_el_i2s(player_t * player, uint8_t core) {
    if(!player->playback) {
        player->playback = audio_el_i2s_create(player, 0, core) ;
    }
}
// static inline void build_el_keyboard(player_t * player, uint8_t bit_num) {
//     if(!player->keyboard) {
//         audio_el_spi_keyboard(player, bit_num) ;
//     }
// }

static JSClassID js_audio_player_class_id ;
static JSValue js_audio_player_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    size_t mem = heap_caps_get_free_size(MALLOC_CAP_DMA) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_audio_player_class_id) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    player_t * player = heap_caps_malloc(sizeof(player_t), MALLOC_CAP_DMA) ;
    if(!player) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(player, 0, sizeof(player_t)) ;
    JS_SetOpaque(jsobj, player) ;

    player->base.ctx = ctx ;
    player->base.jsobj = jsobj ;

    // 避免 gc 销毁对象
    // 调用 detach() 方法释放这里的引用
    JS_DupValue(ctx, jsobj) ;

    printf("audio.Player DMA alloc: %d, free:%d\n", mem-heap_caps_get_free_size(MALLOC_CAP_DMA), heap_caps_get_free_size(MALLOC_CAP_DMA)) ;
    return jsobj ;
}
static void js_audio_player_finalizer(JSRuntime *rt, JSValue this_val){
    printf("deinit all player resource") ;
    
    player_t * player = JS_GetOpaque(this_val, js_audio_player_class_id) ;
    if(!player) {
        printf("?! js_audio_player_finalizer()") ;
        return ;
    }

    if(player->src) {
        audio_el_src_delete(player->src) ;
    }
    if(player->mp3) {
        audio_el_mp3_delete(player->mp3) ;
    }
    if(player->playback) {
        audio_el_i2s_delete(player->playback) ;
    }

    free(player) ;
    JS_SetOpaque(this_val, NULL) ;
}
static JSClassDef js_audio_player_class = {
    "audio.Player",
    .finalizer = js_audio_player_finalizer,
} ;

static JSValue js_audio_play_pcm(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(player->base.running) {
        THROW_EXCEPTION("player is running")
    }
    CHECK_ARGC(1)
    
    build_el_src(player,0) ;
    build_el_i2s(player,0) ;

    ARGV_TO_STRING(0, path) ;

    if(strlen(path) + strlen(vfs_path_prefix) + 1 > sizeof(player->src->src_path)) {
        free(path) ;
        THROW_EXCEPTION("path too long")
    }

    sprintf(player->src->src_path, "%s%s", vfs_path_prefix, path) ;
    JS_FreeCString(ctx, path) ;

    i2s_set_clk(0,44100,32,2);

    if(!audio_el_src_strip_pcm(player->src)) {
        THROW_EXCEPTION("file not exists") ;
    }

    // 清空管道
    audio_pipe_clear(player) ;

    // src -> playback
    audio_pipe_link( player, 2, player->src, player->playback ) ;

    audio_pipe_set_stats(player, STAT_RUNNING) ;

    player->base.paused = false ;
    player->base.running = true ;
    player->base.finished = false ;
    player->base.error = 0 ;
    audio_pipe_emit_js(player, "play", JS_UNDEFINED) ;

    return JS_UNDEFINED ;
}


static JSValue js_audio_play_mp3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(player->base.running) {
        THROW_EXCEPTION("player is running")
    }
    CHECK_ARGC(1)
    
    build_el_src(player,1) ;
    build_el_mp3(player,1) ;
    build_el_i2s(player,0) ;

    ARGV_TO_STRING(0, path) ;
    bool sync = false ;
    if(argc>1) {
        sync = JS_ToBool(ctx, argv[1]);
    }

    if(strlen(path) + strlen(vfs_path_prefix) + 1 > sizeof(player->src->src_path)) {
        free(path) ;
        THROW_EXCEPTION("path too long")
    }
    sprintf(player->src->src_path, "%s%s", vfs_path_prefix, path) ;
    JS_FreeCString(ctx, path) ;


#ifdef PLATFORM_ESP32
    if(!audio_el_src_strip_mp3(player->src)) {
        THROW_EXCEPTION("file not exists") ;
    }

    // 重置 hexli 状态
    if(xEventGroupGetBits(player->mp3->base.stats) & STAT_RUNNING) {
        THROW_EXCEPTION("decoder not close yet")
    }
    audio_el_mp3_reset(player->mp3) ;

    // 清空管道
    audio_pipe_clear(player) ;

    audio_pipe_link( player, 3, player->src, player->mp3, player->playback ) ;

    player->base.paused = false ;
    player->base.running = true ;
    player->base.finished = false ;
    player->base.error = 0 ;
    audio_pipe_emit_js(player, "play", JS_UNDEFINED) ;

    audio_pipe_set_stats(player, STAT_RUNNING) ;
    
    if(sync) {
        xEventGroupWaitBits(player->playback->base.stats, STAT_STOPPED, false, false, portMAX_DELAY);
        printf("stopped") ;
    }

#else
    player->mp3->fin = fopen("../filesystem/root/mnt/sd/music-16b-2c-44100hz.mp3", "rb") ;
    player->mp3->fout = fopen("../filesystem/root/mnt/sd/test2.pcm", "wb") ;
    task_mp3_decoder(NULL) ;
    fclose(player->mp3->fin) ;
    fclose(player->mp3->fout) ;
#endif
    return JS_UNDEFINED ;
}

static JSValue js_audio_play_with_keyboard(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // THIS_PLAYER(player)
    // if(player->base.running) {
    //     THROW_EXCEPTION("player is running")
    // }
    // CHECK_ARGC(1)
    // ARGV_TO_STRING(0, path) ;

    return JS_UNDEFINED ;
}

static JSValue js_audio_is_running(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return player->base.running? JS_TRUE : JS_FALSE ;
}
static JSValue js_audio_is_paused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return player->base.paused? JS_TRUE : JS_FALSE ;
}
static JSValue js_audio_pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    audio_pipe_clear_stats(player, STAT_RUNNING) ;
    audio_pipe_set_stats(player, STAT_PAUSING) ;
    player->base.paused = true ;
    audio_pipe_emit_js(player, "pause", JS_UNDEFINED) ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    audio_pipe_clear_stats(player, STAT_PAUSED) ;
    audio_pipe_clear_stats(player, STAT_PAUSING) ;
    audio_pipe_set_stats(player, STAT_RUNNING) ;
    player->base.paused = false ;
    audio_pipe_emit_js(player, "resume", JS_UNDEFINED) ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_track_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(!player->base.running) {
        return JS_UNDEFINED ;
    }
    
    audio_pipe_set_stats(player, STAT_RUNNING) ;
    audio_el_set_stat(player->base.first, STAT_STOPPING) ;
    vTaskDelay(0) ;

    return JS_UNDEFINED ;
    // EventBits_t bits = xEventGroupWaitBits(player->last->stats, STAT_STOPPED, false, false, 1000);
    // if(!(bits&STAT_STOPPED)) {
    //     printf("stop time out: %d\n", bits) ;
    // }

    // return (bits&STAT_STOPPED)? JS_TRUE : JS_FALSE ;
}

static JSValue js_audio_print_stats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)

    printf("\n[task_src]\n") ;
    audio_el_print_stats(player->src) ;
    
    printf("\n[task_mp3_decode]\n") ;
    audio_el_print_stats(player->mp3) ;
    
    printf("\n[task_i2s]\n") ;
    audio_el_print_stats(player->playback) ;

    return JS_UNDEFINED ;
}

static JSValue js_audio_play_detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    JS_FreeValue(ctx, player->base.jsobj) ;
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_audio_player_proto_funcs[] = {
    JS_CFUNC_DEF("playPCM", 0, js_audio_play_pcm),
    JS_CFUNC_DEF("playMP3", 0, js_audio_play_mp3),
    JS_CFUNC_DEF("pause", 0, js_audio_pause),
    JS_CFUNC_DEF("resume", 0, js_audio_resume),
    JS_CFUNC_DEF("stop", 0, js_audio_stop),
    JS_CFUNC_DEF("isRunning", 0, js_audio_is_running),
    JS_CFUNC_DEF("isPaused", 0, js_audio_is_paused),
    JS_CFUNC_DEF("detach", 0, js_audio_play_detach),
    JS_CFUNC_DEF("printStats", 0, js_audio_print_stats),
} ;

void be_module_music_init() {
    // dn(configTICK_RATE_HZ)
    JS_NewClassID(&js_audio_player_class_id);
}

void be_module_music_require(JSContext *ctx, JSValue audio) {
    
    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    QJS_DEF_CLASS(audio_player, "Player", "audio.Player", EventEmitterProto, audio)

    JS_FreeValue(ctx, EventEmitterProto);
}

void be_module_music_reset(JSContext *ctx) {
}
#include "js_player_midi.h"
#include "module_fs.h"
#include "utils.h"

#ifdef PLATFORM_ESP32
#include "driver/i2s.h"
#include "module_serial_spi.h"
#include "audio_stream.h"
#endif

#include "midi_keys.h"


#define THIS_PLAYER(thisobj) \
    audio_player_midi_t * thisobj = JS_GetOpaque(this_val, js_midi_player_class_id) ; \
    if(!thisobj) { \
        THROW_EXCEPTION("must be called as a MIDIPlayer method") \
    }

static JSClassID js_midi_player_class_id ;
static JSValue js_midi_player_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    CHECK_ARGC(5)

    ARGV_TO_UINT8(0, spiDevId)
    spi_device_handle_t spiDev = spi_handle_with_id(spiDevId) ;
    if(!spiDev) {
        THROW_EXCEPTION("Invalid spi dev id")
    }
    ARGV_TO_UINT8(1, pl)
    ARGV_TO_UINT8(2, i2s)

    JSValue jsarr = argv[3] ;
    if(!JS_IsArray(ctx, jsarr)) {
        THROW_EXCEPTION("arg keys must be a array")
    }
    size_t keys_cnt = 0 ;
    if(JS_ToUint32(ctx, &keys_cnt, JS_GetPropertyStr(ctx, jsarr, "length"))!=0) {
        THROW_EXCEPTION("arg keys must be a array")
    }
    if(keys_cnt<1 || keys_cnt%8!=0) {
        THROW_EXCEPTION("Keys must be a multiple of 8")
    }
    uint8_t * keys = malloc(keys_cnt) ;
    if(!keys) {
        THROW_EXCEPTION("out of memory?")
    }
    uint32_t k = 0 ;
    for(int i=0;i<keys_cnt;i++) {
        JS_ToUint32(ctx, &k, JS_GetPropertyUint32(ctx, jsarr, i)) ;
        keys[i] = k ;
        printf("%d : K%d\n", i, k) ;
    }

    audio_player_midi_conf_t config = {
        .sf2_path = "/fs/var/piano.sf2" ,
        .handle = spiDev ,
        .pl_pin = pl ,
        .keys = keys ,
        .keys_cnt = keys_cnt ,
        .i2s_num = i2s ,
    } ;

    audio_player_midi_t * player = audio_player_midi_create(&config) ;
    
    free(keys) ;

    if(!player) {
        THROW_EXCEPTION("out of memory?")
    }

    JSValue jsobj = JS_NewObjectClass(ctx, js_midi_player_class_id) ;

    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    JS_SetOpaque(jsobj, player) ;
    player->base.ctx = ctx ;
    player->base.jsobj = jsobj ;

    // 避免 gc 销毁对象
    // 调用 detach() 方法释放这里的引用
    JS_DupValue(ctx, jsobj) ;

    return jsobj ;
}
static void js_midi_player_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_midi_player_finalizer()\n") ;
    audio_player_midi_t * player = JS_GetOpaque(this_val, js_midi_player_class_id) ;
    if(!player) {
        printf("?! js_audio_player_finalizer()") ;
        return ;
    }
    audio_player_midi_delete(player) ;
    JS_SetOpaque(this_val, NULL) ;
}
static JSClassDef js_midi_player_class = {
    "lv.MIDIPlayer",
    .finalizer = js_midi_player_finalizer,
} ;

static JSValue js_midi_player_detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    JS_FreeValue(ctx, player->base.jsobj) ;
    return JS_UNDEFINED ;
}


static JSValue js_midi_player_note_press(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, note)
    THIS_PLAYER(player)

    dn(note) ;
    tsf_note_on(player->sf, player->sf_preset, note, 1.0f) ;

    return JS_UNDEFINED ;
}
static JSValue js_midi_player_note_release(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, note)
    THIS_PLAYER(player)

    tsf_note_off(player->sf, player->sf_preset, note) ;

    return JS_UNDEFINED ;
}

static JSValue js_midi_player_load(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_PLAYER(player)

    char * midpath = js_arg_to_vfspath(ctx, argv[0]) ;
    bool res = audio_el_midi_msg_load(player->midi_msg, midpath) ;

    free(midpath) ;

    return res? JS_TRUE : JS_FALSE ;
}

static JSValue js_midi_player_play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return audio_el_midi_msg_play(player->midi_msg)? JS_TRUE : JS_FALSE ;
}

static JSValue js_midi_player_pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    audio_el_midi_msg_pause(player->midi_msg) ;
    return JS_UNDEFINED ;
}


static JSValue js_midi_player_current_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return JS_NewInt32(ctx, player->midi_msg->played_notes) ;
}

static JSValue js_midi_player_seek(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_PLAYER(player)

    ARGV_TO_UINT32(0,pos)
    
    return audio_el_midi_msg_seek(player->midi_msg, pos)? JS_TRUE : JS_FALSE ;
}

static JSValue js_midi_player_note_count(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return JS_NewInt32(ctx, audio_el_midi_msg_note_cnt(player->midi_msg)) ;
}


static JSValue js_midi_player_set_playback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_PLAYER(player)
    
    player->midi_msg->playback = JS_ToBool(ctx, argv[0])? true: false ;

    return JS_UNDEFINED ;
}

static JSValue js_midi_player_set_speed(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    THIS_PLAYER(player)

    ARGV_TO_UINT8(0, speed)
    ARGV_TO_UINT8(1, speed_d)

    player->midi_msg->speed = speed ;
    player->midi_msg->speed_d = speed_d ;

    return JS_UNDEFINED ;

}


static JSValue js_midi_player_hint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_PLAYER(player)

    ARGV_TO_UINT8(0, key)
    bool press = true ;
    if(argc>1) {
        press = JS_ToBool(ctx, argv[1]) ;
    }
    
    audio_el_spi_keyboard_hint(player->keyboard, key, press) ;

    return JS_UNDEFINED ;
}

static JSValue js_midi_player_hint_with_bytes(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_PLAYER(player)

    ARGV_TO_UINT8(0, byte0)
    uint8_t byte1 = 0, byte2 = 0, byte3 = 0 ;
    if(argc>1) {
        JS_ToUint32(ctx, &byte1, argv[1]) ;
    }
    if(argc>2) {
        JS_ToUint32(ctx, &byte2, argv[2]) ;
    }
    if(argc>3) {
        JS_ToUint32(ctx, &byte3, argv[3]) ;
    }
    
    audio_el_spi_keyboard_hint_bytes(player->keyboard, byte0, byte1, byte2, byte3) ;

    return JS_UNDEFINED ;
}

static JSValue js_midi_note_count(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * midpath = js_arg_to_vfspath(ctx, argv[0]) ;

    tml_message* msg = tml_load_filename(midpath);
    if(!msg) {
        JS_ThrowReferenceError(ctx, "can not load midi file %s", midpath) ;
        free(midpath) ;
        return JS_EXCEPTION ;
    }
    free(midpath) ;

    int cnt = 0 ;
    for(;msg!=NULL;msg=msg->next) {
        if(msg->type!=TML_NOTE_OFF && msg->velocity!=0) {
            cnt ++ ;
        }
    }

    tml_free(msg) ;

    return JS_NewInt32(ctx, cnt) ;
}

static const JSCFunctionListEntry js_midi_player_proto_funcs[] = {
    JS_CFUNC_DEF("detach", 0, js_midi_player_detach),
    JS_CFUNC_DEF("notePress", 0, js_midi_player_note_press),
    JS_CFUNC_DEF("noteRelease", 0, js_midi_player_note_release),
    JS_CFUNC_DEF("load", 0, js_midi_player_load),
    JS_CFUNC_DEF("play", 0, js_midi_player_play),
    JS_CFUNC_DEF("pause", 0, js_midi_player_pause),
    JS_CFUNC_DEF("seek", 0, js_midi_player_seek),
    JS_CFUNC_DEF("currentPos", 0, js_midi_player_current_pos),
    JS_CFUNC_DEF("noteCount", 0, js_midi_player_note_count),
    JS_CFUNC_DEF("setSpeed", 0, js_midi_player_set_speed),
    JS_CFUNC_DEF("setPlayback", 0, js_midi_player_set_playback),
    JS_CFUNC_DEF("hint", 0, js_midi_player_hint),
    JS_CFUNC_DEF("hintWithBytes", 0, js_midi_player_hint_with_bytes),
} ;



void be_module_midi_init() {
    JS_NewClassID(&js_midi_player_class_id);
}

void be_module_midi_require(JSContext *ctx, JSValue pkg) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    QJS_DEF_CLASS(midi_player, "MIDIPlayer", "lv.MIDIPlayer", EventEmitterProto, pkg)

    JS_SetPropertyStr(ctx, pkg, "midiNoteCount", JS_NewCFunction(ctx, js_midi_note_count, "midiNoteCount", 1));

    JS_FreeValue(ctx, EventEmitterProto);

#ifdef PLATFORM_ESP32
#else
#endif
}

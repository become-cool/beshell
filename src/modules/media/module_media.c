
#include "module_media.h"
#include "utils.h"

#include "tml.h"

#ifdef PLATFORM_ESP32
#include "js_player_midi.h"
#include "js_player_music.h"
#endif

static JSValue js_audio_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    tml_message * midmsg = tml_load_filename("tmp/孤勇者.mid");
    if(!midmsg) {
        return JS_FALSE;
    }

    for (; midmsg; midmsg = midmsg->next) {
        switch (midmsg->type) {
            case TML_PROGRAM_CHANGE: //channel program (preset) change (special handling for 10th MIDI channel with drums)
                // tsf_channel_set_presetnumber(g_TinySoundFont, midmsg->channel, midmsg->program, (midmsg->channel == 9));
                printf("TML_PROGRAM_CHANGE\n") ;
                break;
            case TML_NOTE_ON: //play a note
            case TML_NOTE_OFF: //stop a note
                // tsf_channel_note_on(g_TinySoundFont, midmsg->channel, midmsg->key, midmsg->velocity / 127.0f);
                if(midmsg->type==TML_NOTE_OFF || midmsg->velocity==0) {
                    printf("[TML_NOTE_OFF] key:%d, dur:%d\n",midmsg->key, midmsg->time) ;
                }
                else {
                    printf("[TML_NOTE_ON] key:%d, vl:%d, dur:%d\n",midmsg->key, midmsg->velocity, midmsg->time) ;
                }
                break;

            case TML_PITCH_BEND: //pitch wheel modification
                // tsf_channel_set_pitchwheel(g_TinySoundFont, midmsg->channel, midmsg->pitch_bend);
                printf("TML_PITCH_BEND\n") ;
                break;
            case TML_CONTROL_CHANGE: //MIDI controller messages
                // tsf_channel_midi_control(g_TinySoundFont, midmsg->channel, midmsg->control, midmsg->control_value);
                printf("TML_CONTROL_CHANGE\n") ;
                break;
            default: 
                printf("unknow type:%d\n",midmsg->type) ;
        }
    }

    return JS_UNDEFINED ;
}


void be_module_media_init() {
#ifdef PLATFORM_ESP32
    be_module_midi_init() ;
    be_module_music_init() ;
#endif
}

void be_module_media_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue audio = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "audio", audio);

    // JS_SetPropertyStr(ctx, audio, "test", JS_NewCFunction(ctx, js_audio_test, "test", 1));

#ifdef PLATFORM_ESP32
    be_module_midi_require(ctx, audio) ;
    be_module_music_require(ctx, audio) ;
#endif

    JS_FreeValue(ctx, beapi);
}

void be_module_media_reset(JSContext *ctx) {
}
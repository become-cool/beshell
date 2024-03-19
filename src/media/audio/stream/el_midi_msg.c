#include "audio_stream.h"
#include "esp_timer.h"

#define IsPress(msg) (msg->type==TML_NOTE_ON && msg->velocity!=0)


// midi 事件任务线程
static void task_func_midi_msg(audio_el_midi_msg_t * el) {

    EventBits_t stat ;
    uint64_t time_ms = 0 ;
    uint8_t bit = 0 ;
    bool press = false ;

    while(1) {

        stat = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        // 请求停止线程
        if( (stat&STAT_STOPPING) == STAT_STOPPING ) {
            audio_el_set_stat(el, STAT_STOPPED) ;
            vTaskDelay(10) ;
            continue;
        }
        if( (stat&STAT_RUNNING)!=STAT_RUNNING || !el->msg ) {
            vTaskDelay(10) ;
            continue;
        }

        time_ms = ( esp_timer_get_time()/1000 - el->start_ms )  ;

        if(el->msg->time > time_ms) {
            // printf("%lld, delay:%lld\n", time_ms, el->msg->time-time_ms) ;
            vTaskDelay( (el->msg->time-time_ms)*el->speed/portTICK_PERIOD_MS/el->speed_d ) ;
        }

        switch (el->msg->type) {
            case TML_PROGRAM_CHANGE: //channel program (preset) change (special handling for 10th MIDI channel with drums)
                tsf_channel_set_presetnumber(el->sf, el->msg->channel, el->msg->program, (el->msg->channel == 9));
                // printf("TML_PROGRAM_CHANGE\n") ;
                break;

            case TML_NOTE_ON: //play a note
            case TML_NOTE_OFF: //stop a note

                // 琴键按下或释放
                press = IsPress(el->msg) ;

                // 键盘提示
                if(el->prompter) {
                    audio_el_spi_keyboard_hint(el->prompter, el->msg->key, press) ;
                }

                if(press) {
                    // printf("[%d ON] key:%d \n",el->msg->channel, el->msg->key) ;
                    if(el->playback && el->sf) {
                        tsf_channel_note_on(el->sf, el->msg->channel, el->msg->key, el->msg->velocity / 127.0f);
                        // tsf_note_on(el->sf, el->sf_preset, el->msg->key, 1.0f);
                    }

                    el->played_notes ++ ;
                    audio_pipe_emit_js(el->base.pipe, "press", JS_NewInt32(((audio_pipe_t*)el->base.pipe)->ctx,  el->msg->key)) ;

                }
                else {
                    // printf("[%d OFF] key:%d\n",el->msg->channel,el->msg->key) ;
                    if(el->playback && el->sf) {
					    tsf_channel_note_off(el->sf, el->msg->channel, el->msg->key);
                        // tsf_note_off(el->sf, el->sf_preset, el->msg->key);
                    }
                    
                    audio_pipe_emit_js(el->base.pipe, "release", JS_NewInt32(((audio_pipe_t*)el->base.pipe)->ctx,  el->msg->key)) ;
                }
                break;

            case TML_PITCH_BEND: //pitch wheel modification
                tsf_channel_set_pitchwheel(el->sf, el->msg->channel, el->msg->pitch_bend);
                printf("TML_PITCH_BEND\n") ;
                break;
            case TML_CONTROL_CHANGE: //MIDI controller messages
                tsf_channel_midi_control(el->sf, el->msg->channel, el->msg->control, el->msg->control_value);
                // printf("TML_CONTROL_CHANGE\n") ;
                break;
            default: 
                printf("unknow type:%d\n",el->msg->type) ;
        }

        el->msg = el->msg->next ;

        // midi 文件结束
        if(!el->msg) {
            audio_el_set_stat(el, STAT_STOPPED) ;

            audio_pipe_emit_js(el->base.pipe, "finish", JS_UNDEFINED) ;

            printf("time: %lld = %lld - %lld sec\n", (esp_timer_get_time()/1000-el->start_ms)/1000, esp_timer_get_time()/1000/1000, el->start_ms/1000) ;
        }
    }
}



// keys_cnt 必须是 32 的倍数 
audio_el_midi_msg_t * audio_el_midi_msg_create(audio_pipe_t * pipe, tsf* sf, audio_player_midi_conf_t * config) {  

    audio_el_midi_msg_t * el ;
    necho_alloc("audio_el_midi_msg_t",{
        ELEMENT_CREATE(pipe, audio_el_midi_msg_t, el, task_func_midi_msg, 1024*3, 5, 0, 0)
    })

    el->sf = sf ;
    el->sf_preset = 0 ;

    el->speed = 10 ;
    el->speed_d = 10 ;
    
    el->playback = true ;
    
    audio_el_set_stat(el, STAT_STOPPED) ;

    return el ;
}

void audio_el_midi_msg_delete(audio_el_midi_msg_t * el) {
    
    tml_message * msg = el->msg_header ;
    el->msg_header = NULL ;
    el->msg = NULL ;

    audio_el_delete(el) ;

    if(msg) {
        tml_free(msg) ;
    }
}

// 开始播放 midi 文件里的事件
bool audio_el_midi_msg_load(audio_el_midi_msg_t * el, const char * midPath) {

    audio_el_stop(el) ;

    if(el->msg_header) {
        tml_free(el->msg_header) ;
        el->msg_header = NULL ;
        el->msg = NULL ;
    }
    
    el->msg_header = tml_load_filename(midPath);
    if(!el->msg_header) {
        return false ;
    }

    el->msg = NULL ;
    el->played_notes = -1 ;

    return true ;
}


// 开始播放 midi 文件里的事件
bool audio_el_midi_msg_play(audio_el_midi_msg_t * el) {

    if(!el->msg_header) {
        return false ;
    }

    if(!el->msg) {
        el->msg = el->msg_header ;
        el->played_notes = 0 ;
    }

    // 已经结束
    if(!el->msg) {
        return audio_el_stop(el) ;
    }
    
    if( audio_el_is_running(el) ) {
        return true ;
    }
    
    // 释放所有按下的琴键
    tsf_note_off_all(el->sf) ;
    
    // 清空键盘提示
    if(el->prompter) {
        audio_el_spi_keyboard_hint_clear(el->prompter) ;
    }
    
    el->start_ms = esp_timer_get_time()/1000 - el->msg->time ;

    audio_el_set_stat(el, STAT_RUNNING) ;

    return true ;
}


int audio_el_midi_msg_note_cnt(audio_el_midi_msg_t * el) {

    int cnt = 0 ;
    
    if(!el->msg_header) {
        return 0 ;
    }
    
    for(tml_message * msg = el->msg_header;msg!=NULL;msg=msg->next) {
        if(IsPress(msg)) {
            cnt ++ ;
        }
    }

    return cnt ;
}

// 开始播放 midi 文件里的事件
bool audio_el_midi_msg_seek(audio_el_midi_msg_t * el, unsigned int pos) {
    if(!el->msg_header) {
        return false ;
    }

    int notes = 0 ;

    for( tml_message * msg = el->msg_header; msg!=NULL; msg=msg->next ) {
        if(IsPress(msg)) {
            if(notes==pos) {

                bool running = audio_el_is_running(el) ;
                if(running) {
                    if(!audio_el_stop(el)) {
                        printf("can not stop midi msg\n") ;
                        return false ;
                    }
                }

                // 释放所有按下的琴键
                tsf_note_off_all(el->sf) ;

                el->msg = msg ;
                el->played_notes = notes ;
                
                el->start_ms = esp_timer_get_time()/1000 - el->msg->time ;

                if(running) {
                    audio_el_set_stat(el, STAT_RUNNING) ;
                }

                return true ;
            }

            notes ++ ;
        }
    }

    return false ;
}

void audio_el_midi_msg_pause(audio_el_midi_msg_t * el) {
    audio_el_stop(el) ;
}

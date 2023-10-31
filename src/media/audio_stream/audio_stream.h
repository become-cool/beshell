#ifndef ONCE_H_AUDIO_STREAM
#define ONCE_H_AUDIO_STREAM

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "debug.h"
#include "quickjs-libc.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/ringbuf.h"
#include "driver/spi_master.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include "psram.h"
#include <sys/stat.h>

#include "mp3dec.h"

#include "tsf.h"
#include "tml.h"

#define STAT_RUNNING        1
#define STAT_STOPPING       2
#define STAT_STOPPED        4
#define STAT_PAUSING        8
#define STAT_PAUSED         16
#define STAT_ALL_PHASE      (STAT_RUNNING | STAT_STOPPING | STAT_STOPPED | STAT_PAUSING | STAT_PAUSED)
// 以上 bit 是互斥的

#define STAT_DRAIN          32

typedef struct _audio_el_t{

    TaskHandle_t task ;
    RingbufHandle_t ring ;
    size_t ringSz ;
    EventGroupHandle_t stats ;

    struct _audio_el_t * upstream;
    struct _audio_el_t * downstream;
    void * pipe ;

} audio_el_t ;

#define MAX_MIDI_KEY_CNT 128


#define ELEMENT_CREATE(_pipe, type, var, func,stack,prio,core,ringsize) \
    var = mallocDMA(sizeof(type)) ;                                     \
    memset(var, 0, sizeof(type)) ;                                      \
    var->base.pipe = _pipe ;                                            \
    audio_el_init(var, func, #func, stack, prio, core, ringsize ) ;



// 文件读取 element
typedef struct {

    audio_el_t base ;

    char src_path[256] ;
    FILE * file ;

} audio_el_src_t ;


// mp3 解码 element
typedef struct {

    audio_el_t base ;

    uint8_t * undecode_buff ;
    MP3DecInfo * decoder ;

    uint8_t i2s ;
    int samprate ;
    int channels ;

} audio_el_mp3_t ;

// i2s 播放 element
typedef struct {
    audio_el_t base ;
    uint8_t i2s ;
} audio_el_i2s_t ;



// spi midi 键盘输入(74HC165/74HC595) element
typedef struct {
    audio_el_t base ;

    spi_device_handle_t handle ;
    int8_t pl_pin ;
    uint8_t byte_num ;
    uint8_t * map_bits_keys ; // 用输入的键位查询 MIDI琴键值
    
    tsf* sf ;
    uint8_t sf_preset ;

    uint8_t hint_bytes[MAX_MIDI_KEY_CNT/8] ;

} audio_el_spi_keyboard_t ;

// midi 事件源(.mid文件) element
typedef struct {

    audio_el_t base ;
    
    tsf* sf ;
    uint8_t sf_preset ;
    
    tml_message * msg_header ;  // 事件列表的头部
    tml_message * msg ;         // 当前事件
    int played_notes ;          // 已播放音符位置, 用于表示进度

    uint64_t start_ms ;         // 开始播放的系统时间

    uint8_t speed ;         // 速率分子
    uint8_t speed_d ;       // 速率分母

    bool playback ;   // 直接合成音频

    audio_el_spi_keyboard_t * prompter ; //在 spi midi 键盘上演奏提示

} audio_el_midi_msg_t ;


// midi 音色合成 element
typedef struct {
    audio_el_t base ;

    tsf* sf ;
    uint8_t sf_preset ;
    
    short * sound_buff ;

} audio_el_midi_render_t ;



typedef struct _audio_pipe {

    bool running:1 ;
    bool paused:1 ;
    bool finished: 1 ;   // 正常播放完毕: finished=true， 中途停止: finished=false

    int8_t error ;

    int samplerate ;

    audio_el_t * first ;
    audio_el_t * last ;
    
    JSContext * ctx ;
    JSValue jsobj ;
    
} audio_pipe_t ;

// 用于设置midi player
typedef struct {
    char * sf2_path ;
    spi_device_handle_t handle ;
    uint8_t pl_pin ;
    uint8_t * keys ;
    uint8_t keys_cnt ;
    int8_t i2s_num ;
} audio_player_midi_conf_t ;


typedef struct _audio_player_midi {
    audio_pipe_t base ;

    tsf* sf ;
    uint8_t sf_preset ;
    audio_el_midi_msg_t * midi_msg ;
    audio_el_spi_keyboard_t * keyboard ;
    audio_el_midi_render_t * render ;
    audio_el_i2s_t * playback ;
    
} audio_player_midi_t ;



// base audio element
void audio_el_set_stat(audio_el_t * el, EventBits_t stat) ;
void audio_el_init(
        audio_el_t * el
        , TaskFunction_t pvTaskCode
        , const char *const pcName
        , const uint32_t usStackDepth
        , UBaseType_t uxPriority
        , const BaseType_t xCoreID
        , size_t ringSz
    ) ;
void audio_el_delete(audio_el_t * el) ;
bool audio_el_is_running(audio_el_t * el) ;
bool audio_el_stop(audio_el_t * el) ;
bool audio_el_is_drain(audio_el_t * el) ;
void audio_el_print_stats(audio_el_t * el) ;
void audio_el_clear_ring(audio_el_t * el) ;
void audio_el_stop_when_req(audio_el_t * el) ;


// source file reader
void audio_el_src_delete(audio_el_src_t * el) ;
audio_el_t *  audio_el_src_create(audio_pipe_t * pipe, uint8_t core) ;
void task_src(audio_el_src_t * el) ;
bool audio_el_src_strip_pcm(audio_el_src_t * el) ;
bool audio_el_src_strip_mp3(audio_el_src_t * el) ;

// mp3 decoder
audio_el_t * audio_el_mp3_create(audio_pipe_t * pipe, uint8_t core) ;
void audio_el_mp3_delete(audio_el_mp3_t * el) ;
void audio_el_mp3_reset(audio_el_mp3_t * el) ;

// i2s playback
audio_el_i2s_t * audio_el_i2s_create(audio_pipe_t * pipe, uint8_t i2s_num, uint8_t core) ;
void audio_el_i2s_delete(audio_el_i2s_t * el) ;

// midi message
audio_el_midi_msg_t * audio_el_midi_msg_create(audio_pipe_t * pipe, tsf* sf, audio_player_midi_conf_t * config) ;
bool audio_el_midi_msg_load(audio_el_midi_msg_t * el, const char * midPath) ;
int audio_el_midi_msg_note_cnt(audio_el_midi_msg_t * el) ;
bool audio_el_midi_msg_play(audio_el_midi_msg_t * el) ;
bool audio_el_midi_msg_seek(audio_el_midi_msg_t * el, unsigned int pos) ;
void audio_el_midi_msg_pause(audio_el_midi_msg_t * el) ;
void audio_el_midi_msg_delete(audio_el_midi_msg_t * el) ;

// spi midi keyboard
audio_el_spi_keyboard_t * audio_el_spi_keyboard_create(audio_pipe_t * pipe, tsf* sf, audio_player_midi_conf_t * config) ;
void audio_el_spi_keyboard_delete(audio_el_spi_keyboard_t * el) ;
void audio_el_spi_keyboard_hint_clear(audio_el_spi_keyboard_t * el) ;
void audio_el_spi_keyboard_hint(audio_el_spi_keyboard_t * el, uint8_t key, bool isOn) ;
void audio_el_spi_keyboard_hint_bytes(audio_el_spi_keyboard_t * el, uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3) ;

// midi render
audio_el_midi_render_t * audio_el_midi_render_create(audio_pipe_t * pipe, tsf * sf) ;
void audio_el_midi_render_delete(audio_el_midi_render_t * el) ;

// pipe
void audio_pipe_link(audio_pipe_t * pipe, int audio_el_cnt, ...) ;
void audio_pipe_set_stats(audio_pipe_t * pipe, int stats) ;
void audio_pipe_clear_stats(audio_pipe_t * pipe, int stats) ;
void audio_pipe_clear(audio_pipe_t * pipe) ;
void audio_pipe_emit_js(audio_pipe_t * pipe, const char * event, JSValue param) ;

// midi player 
audio_player_midi_t * audio_player_midi_create(audio_player_midi_conf_t * config) ;
void audio_player_midi_delete(audio_player_midi_t * player) ;



#endif
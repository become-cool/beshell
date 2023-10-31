#include "midi_keys.h"
#include "audio_stream.h"
#include "utils.h"

#ifdef PLATFORM_ESP32
#include "driver/i2s.h"
#include "module_serial_spi.h"
#endif

audio_player_midi_t * audio_player_midi_create(audio_player_midi_conf_t * config)  {
    audio_player_midi_t * player = mallocDMA(sizeof(audio_player_midi_t)) ;
    if(!player) {
        return NULL ;
    }
    memset(player, 0, sizeof(audio_player_midi_t)) ;

    player->sf = tsf_load_filename(config->sf2_path);
    if(!player->sf) {
        free(player) ;
        return NULL ;
    }
    tsf_set_output(player->sf, TSF_MONO, 44100, 0);

    // element: midi 事件
    player->midi_msg = audio_el_midi_msg_create(player, player->sf, config) ;
    if(!player->midi_msg) {
        audio_player_midi_delete(player) ;
        return NULL ;
    }

    // element: 键盘输入
    player->keyboard = audio_el_spi_keyboard_create(player, player->sf, config) ;
    if(!player->keyboard) {
        audio_player_midi_delete(player) ;
        return NULL ;
    }

    // element: midi 合成
    player->render = audio_el_midi_render_create(player, player->sf);
    if(!player->render) {
        audio_player_midi_delete(player) ;
        return NULL ;
    }
    
    // element: i2s 播放
    player->playback = audio_el_i2s_create(player, config->i2s_num, 0) ;
    if(!player->playback) {
        audio_player_midi_delete(player) ;
        return NULL ;
    }

    audio_pipe_clear(player) ;
    audio_pipe_link( player, 2, player->render, player->playback ) ;

    // 键盘提示
    player->midi_msg->prompter = player->keyboard ;

    audio_pipe_set_stats(player, STAT_RUNNING) ;
    
    return player ;
}

void audio_player_midi_delete(audio_player_midi_t * player) {

    if(player->keyboard) {
        audio_el_spi_keyboard_delete(player->keyboard) ;
        player->keyboard = NULL ;
    }
    if(player->render) {
        audio_el_midi_render_delete(player->render) ;
        player->render = NULL ;
    }
    if(player->playback) {
        audio_el_i2s_delete(player->playback) ;
        player->playback = NULL ;
    }

    if(player->sf) {
        tsf_close(player->sf) ;
        player->sf = NULL ;
    }

    free(player) ;
}



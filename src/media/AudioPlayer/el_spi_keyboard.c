#include "audio_stream.h"
#include "midi_keys.h"
#include "driver/i2s.h"
#include "driver/gpio.h"


// 由键位查询键值
// inline uint8_t audio_el_spi_keyboard_key_with_bit(audio_el_spi_keyboard_t * el, uint8_t bit) {
//     el->
// }


inline static void trans_keyboard(audio_el_spi_keyboard_t * el, uint8_t * keys, uint8_t * leds) {

    // 触发 74HC165 PL引脚，将 key 读入到移位寄存器
    // record_time(xx, {
        gpio_set_level(el->pl_pin, 0) ;
        vTaskDelay(0) ;
        gpio_set_level(el->pl_pin, 1) ;
        vTaskDelay(0) ;
    // })

    // record_time(yy, {
        spi_trans_int(el->handle, keys, leds, el->byte_num*8) ;
    // })
}

// 键盘读取任务线程
static void task_func_spi_keyboard(audio_el_spi_keyboard_t * el) {

    // 琴键状态(按位存储)
    uint8_t bit_keys[MAX_MIDI_KEY_CNT/8] ;
    memset(bit_keys, 0, sizeof(bit_keys));

    // 上一次的琴键状态(按位存储)
    uint8_t bit_keys_last[MAX_MIDI_KEY_CNT/8] ;
    memset(bit_keys_last, 0, sizeof(bit_keys_last));

    uint8_t bit_diff = 0 ;
    uint8_t i = 0, byte = 0 ;
    uint8_t key = 0 ;

    bool press = false ;
    int64_t t = gettime() ;

    trans_keyboard(el, bit_keys, el->hint_bytes) ;
    memcpy(bit_keys_last, bit_keys, byte<el->byte_num) ;
    // dn(bit_keys)

    while(1) {
        
        vTaskDelay(1) ;

        trans_keyboard(el, bit_keys, el->hint_bytes) ;

        // 逐字节对比
        for(byte=0; byte<el->byte_num; byte++) {
            if( bit_keys_last[byte] != bit_keys[byte] ) {

                printf("%d/%d: %d->%d\n", byte, el->byte_num, bit_keys_last[byte], bit_keys[byte]) ;

                bit_diff = bit_keys_last[byte] ^ bit_keys[byte] ;

                // 逐位对比
                for(i=0;i<8;i++) {
                    if(! (bit_diff & (1 << i)) ) {
                        continue ;
                    }

                    press = (bit_keys[byte] & (1 << i)) ==0 ;

                    key = el->map_bits_keys[ byte*8 + i ] ;

                    if(key) {
                        if(press) {
                            tsf_note_on(el->sf, el->sf_preset, key, 1.0f);
                            
                            audio_pipe_emit_js(el->base.pipe, "press", JS_NewInt32(((audio_pipe_t*)el->base.pipe)->ctx,  key)) ;

                        }
                        else {
                            tsf_note_off(el->sf, el->sf_preset, key);
                            
                            audio_pipe_emit_js(el->base.pipe, "release", JS_NewInt32(((audio_pipe_t*)el->base.pipe)->ctx,  key)) ;
                        }
                    }

                    // char s[40];//要转换成的字符数组
                    // itoa(bit_keys,s,2);//itoa转为二进制
                    // printf("bit:%d, note:%d %s (%s)\n", i, el->map_bits_keys[i], (press?"press":"release"), s ) ;
                }


                bit_keys_last[byte] = bit_keys[byte] ;
            }

        }

    }
}

audio_el_spi_keyboard_t * audio_el_spi_keyboard_create(audio_pipe_t * pipe, tsf* sf, audio_player_midi_conf_t * config) {  

    audio_el_spi_keyboard_t * el ;
    necho_alloc("audio_el_spi_keyboard_t",{
        ELEMENT_CREATE(pipe, audio_el_spi_keyboard_t, el, task_func_spi_keyboard, 1024*3, 5, 0, 0)
    })

    el->byte_num = config->keys_cnt/8 ;
    el->map_bits_keys = mallocDMA(config->keys_cnt) ;
    if(!el->map_bits_keys) {
        return NULL;
    }
    memcpy(el->map_bits_keys, config->keys, config->keys_cnt) ;

    el->handle = config->handle ;
    el->sf = sf ;
    el->sf_preset = 0 ;
    el->pl_pin = config->pl_pin ;

    return el ;
}

void audio_el_spi_keyboard_delete(audio_el_spi_keyboard_t * el) {

    free(el->map_bits_keys) ;
    el->map_bits_keys = NULL ;

    audio_el_delete(el) ;
}

void audio_el_spi_keyboard_hint_clear(audio_el_spi_keyboard_t * el) {
    memset(el->hint_bytes, 0, sizeof(el->hint_bytes) ) ;
}
void audio_el_spi_keyboard_hint(audio_el_spi_keyboard_t * el, uint8_t key, bool isPress) {

    uint8_t keys_cnt = el->byte_num*8 ;
    uint8_t byte, bit ;

    for(uint8_t k=0;k<keys_cnt;k++) {
        if(el->map_bits_keys[k] != key) {
            continue;
        }

        byte = el->byte_num - 1 - (k / 8);  // 串联 74HC595 芯片，末端先输出 
        bit = k % 8 ;

        if(isPress) {
            el->hint_bytes[byte]  |= 1<<bit ;
        }
        else {
            el->hint_bytes[byte]  &= ~(1<<bit) ;
        }

        // char s0[40];//要转换成的字符数组
        // char s1[40];//要转换成的字符数组
        // char s2[40];//要转换成的字符数组
        // char s3[40];//要转换成的字符数组
        // itoa(el->hint_bytes[0],s0,2);//itoa转为二进制    
        // itoa(el->hint_bytes[1],s1,2);//itoa转为二进制    
        // itoa(el->hint_bytes[2],s2,2);//itoa转为二进制    
        // itoa(el->hint_bytes[3],s3,2);//itoa转为二进制    
        // printf("hit %d(%d/%d) %s [%s,%s,%s,%s]\n", k, bit, byte, isPress?"on":"off", s0, s1, s2, s3) ;

        break ;
    }
}

void audio_el_spi_keyboard_hint_bytes(audio_el_spi_keyboard_t * el, uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    el->hint_bytes[0] = byte0 ;
    el->hint_bytes[1] = byte1 ;
    el->hint_bytes[2] = byte2 ;
    el->hint_bytes[3] = byte3 ;
}


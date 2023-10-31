#include "audio_stream.h"
#include "midi_keys.h"
#include "driver/i2s.h"
#include "module_serial_spi.h"


// 键盘读取任务线程
static void task_func_render(audio_el_midi_render_t * el) {

    i2s_set_clk(0, 44100, 16, 1);

	float outputSamples[TSF_RENDER_SHORTBUFFERBLOCK] ;
    short outputBuff[TSF_RENDER_SHORTBUFFERBLOCK] ;

    float * reader = outputSamples ;
    short * write = outputBuff ;
    
    int i = 0 ;

    while(1) {
        
        vTaskDelay(0) ;

		tsf_render_float(el->sf, outputSamples, TSF_RENDER_SHORTBUFFERBLOCK, 0);
        
        reader = outputSamples ;
        write = outputBuff ;
        for (i=0; i<TSF_RENDER_SHORTBUFFERBLOCK; i++) {
            float v = *(reader++);
            *(write++) = (v < -1.00004566f ? (short)-32768 : (v > 1.00001514f ? (short)32767 : (short)(v * 32767.5f)));
        }
        if(pdTRUE != xRingbufferSend(el->base.ring, outputBuff, sizeof(outputBuff), portMAX_DELAY)) {
            printf("task midi render xRingbufferSend() wrong ?????\n") ;
        }
        else {
            xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
        }
    }
}

// keys_cnt 必须是 32 的倍数 
audio_el_midi_render_t * audio_el_midi_render_create(audio_pipe_t * pipe, tsf * sf) {  

    audio_el_midi_render_t * el ;
    necho_alloc("audio_el_midi_render_t",{
        ELEMENT_CREATE(pipe, audio_el_midi_render_t, el, task_func_render, 1024*5, 5, 1, 1024*2)
    })

    el->sf = sf ;

    return el ;
}

void audio_el_midi_render_delete(audio_el_midi_render_t * el) {
    audio_el_delete(el) ;
}


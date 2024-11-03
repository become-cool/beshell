#include "audio_stream.h"
#include "qjs_utils.h"

void audio_pipe_set_stats(audio_pipe_t * pipe, int stats) {
    for(audio_el_t * el=pipe->last; el; el=el->upstream) {
        audio_el_set_stat(el, stats) ;
    }
}
void audio_pipe_clear_stats(audio_pipe_t * pipe, int stats) {
    for(audio_el_t * el=pipe->last; el; el=el->upstream) {
        xEventGroupClearBits(el->stats, stats) ;
    }
}


void audio_pipe_clear(audio_pipe_t * pipe) {
    if(!pipe->first) {
        return ;
    }
    for(audio_el_t * el=pipe->first; el; el=el->downstream) {
        audio_el_clear_ring(el);
    }
}


void audio_pipe_link(audio_pipe_t * pipe, int audio_el_cnt, ...) {

    audio_el_t * el ;
	va_list argptr;
	va_start(argptr, audio_el_cnt);

    pipe->first = pipe->last = NULL ;

	for (int i=0;i<audio_el_cnt; i++) {
		el = va_arg(argptr, audio_el_t *);
        if(i==0) {
            el->upstream = NULL ;
            pipe->first = pipe->last = el ;
        }
        else {
            el->upstream = pipe->last ;
            pipe->last->downstream = el ;
            pipe->last = el ;
        }

        el->downstream = NULL ;

	}
	va_end(argptr);
}

void audio_pipe_append(audio_pipe_t * pipe, audio_el_t * el) {
    if(!pipe->last) {
        pipe->last = pipe->first = el ;
    }
    else {
        pipe->last->downstream = el ;
        el->upstream = pipe->last ;
        el->downstream = NULL ;
        pipe->last = el ;
    }
}

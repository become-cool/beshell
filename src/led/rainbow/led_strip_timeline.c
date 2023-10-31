#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "esp_log.h"
#include "esp_attr.h"
#include "led_strip.h"
#include "be_list.h"
#include "esp_timer.h"
#include "utils.h"
#include "ease.h"


inline static void draw_keyframe(strip_keyframe_t * kf, led_strip_t * strip) {
    strip->fill(strip, kf->x, kf->width, kf->h, kf->s, kf->v, kf->dh, kf->ds, kf->dv) ;
}

#define MID_FRAME(prop)                                     \       
    if(prev->prop!=next->prop) {                            \
        if(next->ease>=EaseInBack) {                        \
            prop = (libEaseExFuncs[next->ease-EaseInBack]( t, prev->prop, next->prop-prev->prop, d, (float)next->ease_s) + 0.5f) ; \
        } else {                                            \
            prop = (libEaseFuncs[next->ease]( t, prev->prop, next->prop-prev->prop, d) + 0.5f) ; \
        }                                                   \
    }                                                       \
    else {                                                  \
        prop = prev->prop ;                                 \
    }

inline static void draw_midframe(strip_keyframe_t * prev, strip_keyframe_t * next, size_t frm, led_strip_t * strip) {
    
    int16_t x, width, dh, ds, dv ;
    uint16_t h,s,v ;

    float t = frm - prev->frm ;
    float d = next->frm - prev->frm ;

    MID_FRAME(x)
    MID_FRAME(width)
    MID_FRAME(h)
    MID_FRAME(s)
    MID_FRAME(v)
    MID_FRAME(dh)
    MID_FRAME(ds)
    MID_FRAME(dv)

    // printf("x:%d,width:%d,hsv(%d,%d,%d), (delta: %d,%d,%d)\n", x, width, h,s,v, dh, ds, dv) ;

    strip->fill(strip, x, width, h, s, v, dh, ds, dv) ;
}

bool led_strip_timeline_render_frame(led_strip_t * strip, size_t frm) {

    bool empty_frame = true ;

    #define CLEAR                               \
        if(empty_frame){                        \
            strip->clear(strip, false, 100) ;   \
            empty_frame = false ;               \
        }

    FOREACH_TYPE_LIST(&strip->timeline.tracks, strip_track_t, track, {
        FOREACH_TYPE_LIST(&track->keyframes, strip_keyframe_t, kf, {
            if(kf->frm==frm) {
                CLEAR
                draw_keyframe(kf, strip) ;
            }
            else {
                if( kf->frm < frm && kf->item.next && ((strip_keyframe_t*)kf->item.next)->frm > frm ) {
                    CLEAR
                    draw_midframe(kf, (strip_keyframe_t*)kf->item.next, frm, strip) ;
                }
            }
        })
    })

    return !empty_frame ;
}


static void timeline_timer_func(led_strip_t * strip) {

    if( led_strip_timeline_render_frame(strip, strip->timeline.current) ){
        strip->refresh(strip, 100) ;
    }

    strip->timeline.current ++ ;

    if(strip->timeline.current>=strip->timeline.frames) {

        // 不重复，结束时间轴
        if( strip->timeline.repeat==0 ) {
            led_strip_timeline_stop(&strip->timeline) ;
            
            strip->timeline.finish_time = gettime() ;

            dn2(strip->timeline.start_time, strip->timeline.finish_time)
            return ;
        }
        // 重新开始
        else {
            strip->timeline.start_time = gettime() ;
            strip->timeline.finish_time = 0 ;
            
            strip->timeline.current = strip->timeline.repeat_from ;
        }
    }
    
}


void led_strip_timeline_init(led_strip_t * strip) {
    be_list_init(&strip->timeline.tracks) ;
    esp_timer_create_args_t timer_args = {
        .callback = &timeline_timer_func,
        .arg = strip,
        .name = "timer-led-strip"
    };
    esp_timer_create(&timer_args, &strip->timeline.timer);
}

void led_strip_timeline_print(strip_timeline_t * timeline) {
    printf("\ntimeline\n") ;
    printf("======\n") ;
    printf("frames=%d\n",timeline->frames) ;
    printf("current=%d\n",timeline->current) ;
    printf("fps=%d\n",(int)timeline->fps) ;

    FOREACH_TYPE_LIST(&timeline->tracks, strip_track_t, track, {
        printf("[track]\n") ;

        FOREACH_TYPE_LIST(&track->keyframes, strip_keyframe_t, kf, {
            printf("<keyframe>\n") ;
            printf("frame@%d\n",kf->frm) ;
            printf("x=%d\n",kf->x) ;
            printf("width=%d\n",kf->width) ;
            printf("hsv=(%d,%d,%d)\n",kf->h,kf->s,kf->v) ;
            printf("delta hsv=(%d,%d,%d)\n",kf->dh,kf->ds,kf->dv) ;
            if(kf->ease>=EaseInBack) {
                printf("ease: %d, s=%lf\n",kf->ease,kf->ease_s) ;
            } else {
                printf("ease: %d\n",kf->ease) ;
            }
        })
         
    })
}

bool led_strip_timeline_clear(strip_timeline_t * timeline) {
    if(timeline->playing) {
        return false ;
    }
    FOREACH_LIST_POP(&timeline->tracks, strip_track_t, track, {
        FOREACH_LIST_POP(&track->keyframes, strip_keyframe_t, kf, {
            free(kf) ;
        })
        free(track) ;
    })
    return true ;
}

bool led_strip_timeline_start(strip_timeline_t * timeline) {
    if(timeline->playing) {
        return false ;
    }
    
    timeline->current = 0 ;

    // 统计 frames 
    timeline->frames = 0 ;
    FOREACH_TYPE_LIST(&timeline->tracks, strip_track_t, track, {
        FOREACH_TYPE_LIST(&track->keyframes, strip_keyframe_t, kf, {
            if(kf->frm>=timeline->frames) {
                timeline->frames = kf->frm + 1 ;
            }        
        })    
    })

    dn(timeline->frames)

    timeline->start_time = gettime() ;
    timeline->finish_time = 0 ;
    esp_timer_start_periodic(timeline->timer, 1000000/timeline->fps);

    timeline->playing = true ;

    return true ;
}
void led_strip_timeline_stop(strip_timeline_t * timeline) {
    if(!timeline->playing) {
        return ;
    }
    esp_timer_stop(timeline->timer) ;
    timeline->playing = false ;
}


void led_strip_timeline_delete(strip_timeline_t * timeline) {
    led_strip_timeline_stop(timeline) ;
    esp_timer_delete(timeline->timer) ;

    led_strip_timeline_clear(timeline) ;
}


strip_track_t * led_strip_timeline_create_track(strip_timeline_t * timeline) {

    strip_track_t * tr = malloc(sizeof(strip_track_t));
    memset(tr, 0, sizeof(strip_track_t));

    be_list_init(&tr->keyframes) ;

    be_list_append(&timeline->tracks, &tr->item) ;

    return tr ;
}


strip_keyframe_t * led_strip_track_create_keyframe(strip_track_t * track) {

    strip_keyframe_t * kf = malloc(sizeof(strip_keyframe_t));
    memset(kf, 0, sizeof(strip_keyframe_t));

    be_list_append(&track->keyframes, &kf->item) ;

    return kf ;
}


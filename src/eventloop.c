#include "eventloop.h"
#include <time.h>
#include <string.h>
#include "utils.h"

eventloop_callback_t * _callback_stack_top = NULL ;

// eventloop_callback_t * eventloop_header() {
//     return _callback_stack_top ;
// }

int eventloop_count() {
    int cnt = 0 ;
    for(
        eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
        current!=NULL ;
        prev=current, current=current->next
    ) {
        cnt ++ ;
    }
    return cnt ;
}

eventloop_callback_t * eventloop_push(JSContext *ctx, JSValue func, JSValue thisobj, int interval, bool repeat) {

    eventloop_callback_t * newcb = malloc(sizeof(eventloop_callback_t)) ;
    memset(newcb,0,sizeof(eventloop_callback_t)) ;
    newcb->func = JS_DupValue(ctx, func) ;
    newcb->thisobj = thisobj ;
    newcb->interval = interval ;
    newcb->repeat = repeat ;
    newcb->requestAnimationFrame = false ;
    newcb->next = _callback_stack_top ;
    newcb->deadline = gettime() + interval ;
    newcb->argc = 0 ;
    newcb->argv = NULL ;

    _callback_stack_top = newcb ;
    
    
    // printf("eventloop_push(%d)\n", eventloop_count()) ;
    return newcb ;
}



eventloop_callback_t * eventloop_push_with_argv(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) {

    eventloop_callback_t * newcb = malloc(sizeof(eventloop_callback_t)) ;
    newcb->func = JS_DupValue(ctx, func) ;
    newcb->thisobj = JS_DupValue(ctx, thisobj) ;
    newcb->interval = 0 ;
    newcb->repeat = 0 ;
    newcb->next = _callback_stack_top ;
    newcb->deadline = 0 ;
    newcb->argc = argc ;
    newcb->argv = argv ;

    for(int i=0;i<argc; i++) {
        if(JS_VALUE_HAS_REF_COUNT(argv[i])){
            JS_DupValue(ctx, argv[i]) ;
        }
    }

    _callback_stack_top = newcb ;

    // printf("eventloop_push_with_argv(%d)\n", eventloop_count()) ;
    return newcb ;
}

void eventloop_out(JSContext *ctx, eventloop_callback_t * prev, eventloop_callback_t * current) {

    // 回收 argv
    for(int i=0; i<current->argc; i++) {
        if(JS_VALUE_HAS_REF_COUNT(current->argv[i]))
            JS_FreeValue(ctx, current->argv[i]) ;
    }
    free(current->argv) ;
    current->argv = NULL ;

    // 回收 func
    JS_FreeValue(ctx, current->func) ;
    JS_FreeValue(ctx, current->thisobj) ;

    if(prev) {
        prev->next = current->next ;
    }
    else {
        _callback_stack_top = current->next ;
    }

    // printf("eventloop_out(%d)\n", eventloop_count()) ;
    free(current) ;
}

void eventloop_remove(JSContext *ctx, eventloop_callback_t * item) {
    int i = 0 ;
    for(
        eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
        current!=NULL ;
        prev=current, current=current->next, i++
    ) {
        if(current == item) {
            eventloop_out(ctx, prev, current) ;
            return ;
        }
    }
}

inline void be_module_eventloop_loop(JSContext *ctx) {
    if(!_callback_stack_top) {
        return ;
    }

    uint64_t now = gettime() ;
    
    eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
    while( current!=NULL ) {
        if(current->deadline > now) {
            goto next ;
        }

        JSValue ret = JS_Call(ctx, current->func, current->thisobj, current->argc, current->argv) ;
        if( JS_IsException(ret) ) {
            js_std_dump_error(ctx) ;
        }
        JS_FreeValue(ctx, ret) ;

        // 一次性任务
        if(!current->repeat) {
            eventloop_callback_t * _current = current;
            current = current->next ;
            eventloop_remove(ctx, _current) ;
            continue ;
        }

        // 连续重复任务
        if(current->interval==0) {
            current->deadline = now ;
        }
        // 间隔重复
        else {
            uint64_t dur = now - current->deadline ;
            current->deadline+= (dur/current->interval) * current->interval ;
            if( dur%current->interval>0 ){
                current->deadline+= current->interval ;
            }
        }
            
    next:
        prev=current ;
        current=current->next ;
    }
}

void be_module_eventloop_reset(JSContext *ctx){
    // 清空未决事件表
    for(
        eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
        current!=NULL ;
        prev=current, current=current->next
    ) {
        eventloop_out(ctx, prev, current) ;
    }
    _callback_stack_top = NULL ;
}
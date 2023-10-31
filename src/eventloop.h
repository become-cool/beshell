#ifndef _EVETNTLOOP_H
#define _EVETNTLOOP_H

#include <stdbool.h>
#include "quickjs-libc.h"

typedef struct _eventloop_callback_t {
    JSValue func ;
    JSValue thisobj ;
    uint64_t deadline ;
    uint32_t interval ;
    bool repeat:1 ;
    bool requestAnimationFrame:1 ;
    int argc ;
    JSValueConst * argv ;
    struct _eventloop_callback_t * next ;
} eventloop_callback_t ;

// inline eventloop_callback_t * eventloop_header() ;

eventloop_callback_t * eventloop_push(JSContext *ctx, JSValue func, JSValue thisobj, int interval, bool repeat) ;
eventloop_callback_t * eventloop_push_with_argv(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) ;

void be_module_eventloop_loop(JSContext *ctx) ;

void eventloop_remove(JSContext *ctx, eventloop_callback_t * item) ;

void be_module_eventloop_reset(JSContext *ctx) ;

#endif
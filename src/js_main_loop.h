#ifndef _TASK_JS_H
#define _TASK_JS_H

#include "quickjs-libc.h"

#ifdef __cplusplus
extern "C" {
#endif

void js_main_loop_tick() ;
void js_main_loop(const char * script) ;
JSContext * task_current_context() ;

uint8_t task_boot_level() ;
void task_reset(int level) ;

void be_main() ;

#ifdef __cplusplus
}
#endif

#endif
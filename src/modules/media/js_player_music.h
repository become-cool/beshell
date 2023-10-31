#ifndef ONCE_H_JS_PLAYER_MUSIC
#define ONCE_H_JS_PLAYER_MUSIC

#include "quickjs-libc.h"

void be_module_music_init() ;
void be_module_music_require(JSContext *ctx, JSValue pkg) ;
void be_module_music_loop(JSContext *ctx) ;
void be_module_music_reset(JSContext *ctx) ;

#endif
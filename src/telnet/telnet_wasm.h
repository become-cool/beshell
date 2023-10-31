#pragma once
#ifdef PLATFORM_WASM


#include "quickjs-libc.h"
#include <stdbool.h>

void be_telnet_wasm_init() ;
void be_telnet_wasm_loop(JSContext *ctx) ;

bool be_telnet_wasm_send(uint8_t * data, size_t datalen) ;

#endif
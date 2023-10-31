#ifndef _TELNET_STDIO_H
#define _TELNET_STDIO_H

#include "quickjs-libc.h"

void be_telnet_stdio_init() ;
void be_telnet_stdio_require(JSContext * ctx) ;
void be_telnet_stdio_loop(JSContext * ctx) ;
void be_telnet_stdio_reset(JSContext * ctx) ;

#endif
#if !defined(_TELNET_SERIAL_H)

#define _TELNET_SERIAL_H

#include "quickjs-libc.h"


void be_telnet_serial_init() ;
void be_telnet_serial_loop(JSContext *ctx) ;
void be_telnet_serial_reset(JSContext *ctx) ;


#endif
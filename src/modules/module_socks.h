#ifndef _MODULE_SOCKS_H
#define _MODULE_SOCKS_H

#include "quickjs-libc.h"

void be_module_socks_init() ;
void be_module_socks_udp_loop(JSContext *ctx) ;
void be_module_socks_reset(JSContext *ctx) ;

void be_module_socks_require(JSContext *ctx) ;

#endif
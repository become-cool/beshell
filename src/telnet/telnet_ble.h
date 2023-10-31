#ifndef __H_TELNET_BLE__
#define __H_TELNET_BLE__

#include "quickjs-libc.h"

void be_telnet_ble_init() ;
void be_telnet_ble_require(JSContext *ctx) ;
void be_telnet_ble_loop(JSContext *ctx) ;
void be_telnet_ble_reset(JSContext *ctx) ;

#endif
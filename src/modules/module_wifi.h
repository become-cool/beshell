#ifndef _MODULE_WIFI_H
#define _MODULE_WIFI_H

#include "quickjs-libc.h"
#include <esp_netif.h>

esp_netif_t * get_netif_ap() ;
esp_netif_t * get_netif_sta() ;

bool wifi_has_inited() ;

#define CHECK_WIFI_INITED                   \
    if(!wifi_has_inited()) {                \
        THROW_EXCEPTION("wifi not init")    \
    }

void be_module_wifi_init() ;
void be_module_wifi_require(JSContext *ctx) ;
void be_module_wifi_reset(JSContext *ctx) ;

#endif
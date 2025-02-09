#include "platform.hpp"

#include "esp_event.h"
#include "esp_netif.h"


namespace be::platform {
    
    bool init() {

        static bool inited = false ;
        if(inited) {
            return true ;
        }
        esp_err_t res = esp_event_loop_create_default() ;
        if(res!=ESP_OK) {
            printf("esp_event_loop_create_default() failed:%d\n", res) ;
            return false ;
        }

        inited = true ;
        return true ;
    }

    bool networkInit() {

        static bool networkInited = false ;
        if(networkInited) {
            return true ;
        }

        if(!init()) {
            return false;
        }

        esp_err_t res = esp_netif_init() ;
        if(res!=ESP_OK) {
            printf("esp_netif_init() failed:%d\n", res) ;
            return false ;
        }

        networkInited = true ;
        return true ;
    }

}
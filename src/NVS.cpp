#include "NVS.hpp"


#ifdef PLATFORM_ESP32

#include "esp_system.h"
#include "nvs_flash.h"


#define NVS_OPEN(h, failed)                                 \
    nvs_handle_t h;                                         \
    esp_err_t err = nvs_open("beshell", NVS_READWRITE, &h); \
    if(err!=ESP_OK) {                                       \
        failed                                              \
    }

#endif

namespace be {
    NVS::NVS() {
    }

    void NVS::readOneTime(const char * key, uint8_t * value) const {
    #ifdef PLATFORM_ESP32
        NVS_OPEN(handle, {
            return ;
        })
        if(nvs_get_u8(handle, key, value)==ESP_OK) {
            nvs_erase_key(handle, key) ;
        }
        nvs_close(handle) ;
    #else
    #endif
    }
}
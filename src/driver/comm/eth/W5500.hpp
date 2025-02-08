#pragma once

#include <EventEmitter.hpp>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_eth.h"
#include "esp_eth_driver.h"
#include "esp_netif.h"

namespace be::driver::comm {

    class W5500: public be::EventEmitter {
        DECLARE_NCLASS_META

    public:
        W5500(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    protected:
        virtual void onNativeEvent(JSContext *ctx, void * param) ;

    private:
        static std::vector<JSCFunctionListEntry> methods ;

        esp_netif_t * eth_netif = NULL ;
        esp_eth_handle_t eth_handle = NULL ;
        esp_eth_netif_glue_handle_t eth_netif_glue = NULL ;

        spi_host_device_t spi_num ;

        gpio_num_t gpio_cs ;
        gpio_num_t gpio_rst ;
        gpio_num_t gpio_intr ;

        int32_t clock_mhz = 16 ;
        uint32_t polling_ms;
        uint8_t phy_addr;
        uint8_t mac_addr[ETH_ADDR_LEN] ;

        esp_netif_ip_info_t ipinfo ;

        static void ethEventHandler(W5500 * that, esp_event_base_t event_base, int32_t event_id, void *event_data) ;
        static void gotIpEventHandler(W5500 * that, esp_event_base_t event_base, int32_t event_id, void * event_data) ;

    } ;
}
#include "W5500.hpp"
#include "Platform.hpp"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_eth_mac_spi.h"
#include "esp_event.h"
#include "module/GPIO.hpp"
#include "platform.hpp"


using namespace std ;

namespace be::driver::comm {
    
    DEFINE_NCLASS_META(W5500, EventEmitter)

    typedef struct {
        uint8_t type ; // 1: Ethernet events, 2: IP events
        esp_event_base_t event_base ;
        int32_t event_id ;
        // union {
        //     esp_netif_ip_info_t ip_info ;
        // } ;
    } w5500_event_t ;

    std::vector<JSCFunctionListEntry> W5500::methods = {
        JS_CFUNC_DEF("setup", 0, W5500::setup),
    } ;

    W5500::W5500(JSContext * ctx, JSValue _jsobj)
        : EventEmitter(ctx,build(ctx,_jsobj))
    {
        memset(&ipinfo,0,sizeof(esp_netif_ip_info_t)) ;

        enableNativeEvent(ctx, sizeof(w5500_event_t), 5) ;
    }
    
    W5500::~W5500() {
        // ds("~W5500")
        if(handlerEth) {
            esp_event_handler_instance_unregister(ETH_EVENT, ESP_EVENT_ANY_ID, handlerEth);
            handlerEth = NULL ;
        }
        if(handlerIp) {
            esp_event_handler_instance_unregister(ETH_EVENT, ESP_EVENT_ANY_ID, handlerIp);
            handlerIp = NULL ;
        }
    }

    JSValue W5500::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new W5500(ctx, this_val) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    

    /** Event handler for Ethernet events */
    void W5500::ethEventHandler(W5500 * that, esp_event_base_t event_base, int32_t event_id, void *event_data) {

        if(!that) {
            printf("W5500 instance is null\n") ;
            return ;
        }
        if( that->eth_handle != *(esp_eth_handle_t *)event_data ){
            printf("Event from unknown eth handle\n") ;
        }

        switch (event_id) {
        case ETHERNET_EVENT_CONNECTED:
            esp_eth_ioctl(that->eth_handle, ETH_CMD_G_MAC_ADDR, that->mac_addr);
            // printf("Ethernet Link Up\n");
            break;
        case ETHERNET_EVENT_DISCONNECTED:
            // printf("Ethernet Link Down\n");
            break;
        case ETHERNET_EVENT_START:
            // printf("Ethernet Started\n");
            break;
        case ETHERNET_EVENT_STOP:
            // printf("Ethernet Stopped\n");
            break;
        default:
            // printf("event_id=%d\n",(int) event_id) ;
            break;
        }
        
        w5500_event_t msg = {
            .type = 1,
            .event_base = event_base,
            .event_id = event_id,
        } ;
        that->emitNativeEvent(&msg) ;
    }

    /** Event handler for IP_EVENT_ETH_GOT_IP */
    void W5500::gotIpEventHandler(W5500 * that, esp_event_base_t event_base, int32_t event_id, void * event_data) {

        if(!that) {
            printf("W5500 instance is null\n") ;
            return ;
        }

        w5500_event_t msg = {
            .type = 2,
            .event_base = event_base,
            .event_id = event_id,
        } ;

        if(event_id==IP_EVENT_ETH_GOT_IP) {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
            memcpy(&that->ipinfo, &event->ip_info, sizeof(esp_netif_ip_info_t));
        }
        else if ( event_id == IP_EVENT_ETH_LOST_IP ) {
            memset(&that->ipinfo,0,sizeof(esp_netif_ip_info_t)) ;
        }

        that->emitNativeEvent(&msg) ;
    }

    
    void W5500::onNativeEvent(JSContext *ctx, void * param) {
        w5500_event_t * event_wrapper = (w5500_event_t *) param ;
        char * name = nullptr ;
        if(event_wrapper->type==1) {
            switch (event_wrapper->event_id) {
            case ETHERNET_EVENT_CONNECTED:
                name = (char*)"connected" ;
                break;
            case ETHERNET_EVENT_DISCONNECTED:
                name = (char*)"disconnected" ;
                break;
            case ETHERNET_EVENT_START:
                name = (char*)"start" ;
                break;
            case ETHERNET_EVENT_STOP:
                name = (char*)"stop" ;
                break;
            default:
                break;
            }
        }

        else if(event_wrapper->type==2) {
            switch (event_wrapper->event_id) {
            case IP_EVENT_ETH_GOT_IP: {
                char ip_str[16];
                JSValue jsipinfo = JS_NewObject(ctx) ;
                
                sprintf(ip_str, IPSTR, IP2STR(&ipinfo.ip));
                JS_SetPropertyStr(ctx, jsipinfo, "ip", JS_NewString(ctx, ip_str)) ;

                sprintf(ip_str, IPSTR, IP2STR(&ipinfo.netmask));
                JS_SetPropertyStr(ctx, jsipinfo, "mask", JS_NewString(ctx, ip_str)) ;

                sprintf(ip_str, IPSTR, IP2STR(&ipinfo.gw));
                JS_SetPropertyStr(ctx, jsipinfo, "gw", JS_NewString(ctx, ip_str)) ;

                emitSyncFree("ip.got", {jsipinfo}) ;
                return ;
            }
            case IP_EVENT_ETH_LOST_IP:
                name = "ip.lost" ;
                break;
            default:
                break;
            }
        }

        if(!name){
            printf("unknow event: %d\n", event_wrapper->event_id) ;
            return ;
        }
        
        emitSyncFree(name, {}) ;
    }

    /**
     * {
     *   spi 
     *   rst=-1
     *   intr=-1
     *   clock_mhz=16
     *   polling_ms=0
     *   phy_addr=1
     * } 
     */
    JSValue W5500::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#if CONFIG_ETH_SPI_ETHERNET_W5500
        ASSERT_ARGC(1)
        THIS_NCLASS(W5500, that)

        GET_INT_PROP(argv[0], "spi", that->spi_num, spi_host_device_t, )
        GET_GPIO_PROP_OPT(argv[0], "cs", that->gpio_cs, GPIO_NUM_NC)
        GET_GPIO_PROP_OPT(argv[0], "rst", that->gpio_rst, GPIO_NUM_NC)
        GET_GPIO_PROP_OPT(argv[0], "intr", that->gpio_intr, GPIO_NUM_NC)
        GET_INT32_PROP_OPT(argv[0], "clock_mhz", that->clock_mhz, 16)
        GET_UINT32_PROP_OPT(argv[0], "polling_ms", that->polling_ms, 0)
        GET_UINT8_PROP_OPT(argv[0], "phy_addr", that->phy_addr, 1)

        // Init common MAC and PHY configs to default
        eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
        eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();

        be::platform::networkInit() ;

        if(!GPIO::installISR(0)){
            JSTHROW("install gpio isr failed")
        }

        // Configure SPI interface for specific SPI module
        spi_device_interface_config_t spi_devcfg = {
            .mode = 0,
            .clock_speed_hz = that->clock_mhz * 1000 * 1000,
            .spics_io_num = that->gpio_cs,
            .queue_size = 20,
        };

        eth_w5500_config_t w5500_config = ETH_W5500_DEFAULT_CONFIG(that->spi_num, &spi_devcfg);
        w5500_config.int_gpio_num = that->gpio_intr;
        w5500_config.poll_period_ms = that->polling_ms;
        esp_eth_mac_t *mac = esp_eth_mac_new_w5500(&w5500_config, &mac_config);
        esp_eth_phy_t *phy = esp_eth_phy_new_w5500(&phy_config);
    
        // Init Ethernet driver to default and install it
        esp_eth_config_t eth_config_spi = ETH_DEFAULT_CONFIG(mac, phy);
        if(esp_eth_driver_install(&eth_config_spi, &that->eth_handle) != ESP_OK){
            JSTHROW("SPI Ethernet driver install failed")
        }

        // 设置 mac 地址
        uint8_t base_mac_addr[ETH_ADDR_LEN];
        esp_efuse_mac_get_default(base_mac_addr) ;
        esp_derive_local_mac(that->mac_addr, base_mac_addr);
        // printf("Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x\n", that->mac_addr[0], that->mac_addr[1], that->mac_addr[2], that->mac_addr[3], that->mac_addr[4], that->mac_addr[5]);
        if(esp_eth_ioctl(that->eth_handle, ETH_CMD_S_MAC_ADDR, that->mac_addr) != ESP_OK) {
            printf("SPI Ethernet driver set MAC address failed\n") ;
        }

        // 注册网络接口
        // Use ESP_NETIF_DEFAULT_ETH when just one Ethernet interface is used and you don't need to modify
        // default esp-netif configuration parameters.
        esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
        that->eth_netif = esp_netif_new(&cfg);
        that->eth_netif_glue = esp_eth_new_netif_glue(that->eth_handle);
        // Attach Ethernet driver to TCP/IP stack
        if(esp_netif_attach(that->eth_netif, that->eth_netif_glue)!=ESP_OK) {
            JSTHROW("SPI Ethernet driver attach to netif failed")
        }
    
        // Register user defined event handers
        esp_event_handler_instance_register(ETH_EVENT, ESP_EVENT_ANY_ID, (esp_event_handler_t)&ethEventHandler, (void*)that, &that->handlerEth);
        esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, (esp_event_handler_t)&gotIpEventHandler, (void*)that, &that->handlerIp);

        // 需要在这里增加一个引用表示 esp native 事件监听，否则 w5500 js 对象被释放以后，esp event handle 里会传入 that 的野指针
        // unsetup 以后在 free 这个引用， 同时 unregister esp handler
        JS_DupValue(ctx, that->jsobj) ;
    
        // Start Ethernet driver state machine
        if(esp_eth_start(that->eth_handle)!=ESP_OK){
            JSTHROW("SPI Ethernet driver start failed")
        }

        return JS_UNDEFINED ;
#else
        JSTHROW("SPI Ethernet driver not enabled, please set \"CONFIG_ETH_USE_SPI_ETHERNET=y\" in sdkconfig")
#endif
    }
}
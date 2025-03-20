#include "NimBLE.hpp"

#if CONFIG_BT_NIMBLE_ENABLED

#include "../../BeShell.hpp"
#include "JSEngine.hpp"
#include "ModuleLoader.hpp"
#include "NativeModule.hpp"
#include "debug.h"
#include "esp_err.h"
#include "host/ble_gap.h"
#include "module/nimble/NimBLE.hpp"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "qjs_utils.h"
#include "quickjs/quickjs.h"
#include "services/gap/ble_svc_gap.h"
#include <cassert>
#include <stdio.h>
#include "host/util/util.h"
#include "blecent.h"
#include "Peer.hpp"


#define BLE_GAP_EVENT_EX_INIT 100
#define BLE_GAP_EVENT_EX_RESET 101


// void ble_store_config_init(void);

namespace be::nimble {

    char const * const NimBLE::name = "nimble" ;

    static int mtu_def = 512;
    
    static NimBLE * singleton = nullptr ;

    NimBLE::NimBLE(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        EXPORT_FUNCTION(init)
        EXPORT_FUNCTION(setName)
        EXPORT_FUNCTION(startScan)
        EXPORT_FUNCTION(stopScan)
        EXPORT_FUNCTION(isScaning)

        EXPORT_FUNCTION(connect)

        enableNativeEvent(ctx, sizeof(struct ble_gap_event), 10) ;

        singleton = this ;
    }
    void NimBLE::use(be::BeShell * beshell) {
        beshell->use<NimBLE>() ;
    }

    // void NimBLE::import(JSContext *ctx) {
    // }



    static void task_nimble(void *param) {
        nimble_port_run();
        nimble_port_freertos_deinit();
    }

    static void nimble_cb_sync(void) {
        
        /* Make sure we have proper identity address set (public preferred) */
        int rc = ble_hs_util_ensure_addr(0);
        assert(rc == 0);

        assert(singleton) ;
        struct ble_gap_event event ;
        event.type = BLE_GAP_EVENT_EX_INIT ;
        singleton->emitNativeEvent(& event) ;
    }
    
    static void nimble_cb_reset(int reason) {
        assert(singleton) ;
        struct ble_gap_event event ;
        event.type = BLE_GAP_EVENT_EX_RESET ;
        singleton->emitNativeEvent(& event) ;
    }

    static int gap_event_handler(struct ble_gap_event *event, NimBLE * nm) {
        // 先复制 data 指向的内存，再进入队列, 避免后续扫描数据覆盖
        if(event->type==BLE_GAP_EVENT_DISC) {
            if(event->disc.length_data) {
                uint8_t * data = (uint8_t *)malloc(event->disc.length_data) ;
                if(!data) {
                    printf("out of memory? %d",event->disc.length_data) ;
                    return 0 ;
                }
                memcpy(data, event->disc.data, event->disc.length_data) ;
                event->disc.data = data ;
            }
        }
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
        else if(event->type==BLE_GAP_EVENT_EXT_DISC) {
            if(event->ext_disc.length_data) {
                uint8_t * data = (uint8_t *)malloc(event->ext_disc.length_data) ;
                if(!data) {
                    printf("out of memory? %d",event->ext_disc.length_data) ;
                    return 0 ;
                }
                memcpy(data, event->ext_disc.data, event->ext_disc.length_data) ;
                event->ext_disc.data = data ;
            }
        }
#endif
        if(!nm->emitNativeEvent(event)) {
            printf("ble scanning event queue full\n") ;
            
            if(event->type==BLE_GAP_EVENT_DISC) {
                if(event->disc.data) {
                    free((void *)event->disc.data) ;
                    event->disc.length_data = 0 ;
                }
            }
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
            else if(event->type==BLE_GAP_EVENT_EXT_DISC) {
                if(event->ext_disc.data) {
                    free((void *)event->ext_disc.data) ;
                    event->ext_disc.length_data = 0 ;
                }
            }
#endif
        }
        return 0 ;
    }

    template <typename T>
    JSValue makeScanParam(JSContext * ctx, T * disc) {

        JSValue param = JS_NewObject(ctx) ;

        JSValue addr = JS_NewArray(ctx) ;
        for(int i=0;i<sizeof(disc->addr.val);i++) {
            JS_SetPropertyUint32(ctx, addr, i, JS_NewUint32(ctx, disc->addr.val[i])) ;
        }
        JS_SetPropertyStr(ctx, param, "addr", addr) ;
        JS_SetPropertyStr(ctx, param, "rssi", JS_NewInt32(ctx, disc->rssi)) ;

        if(disc->data && disc->length_data) {
            JS_SetPropertyStr(ctx, param, "data_length", JS_NewInt32(ctx, disc->length_data)) ;
            JS_SetPropertyStr(ctx, param, "data", JS_NewArrayBufferCopy(ctx, disc->data, disc->length_data)) ;
        }

        // dn(disc->length_data)
        // print_block((uint8_t *)disc->data, disc->length_data, 8) ;
        
        struct ble_hs_adv_fields fields;
        int rc = ble_hs_adv_parse_fields(&fields, disc->data, disc->length_data);
        if (rc == 0) {
            if(fields.name_is_complete) {
                JS_SetPropertyStr(ctx, param, "name", JS_NewStringLen(ctx, (const char *)fields.name, fields.name[fields.name_len-1]==0? fields.name_len-1: fields.name_len )) ;
            }
            if(fields.mfg_data && fields.mfg_data_len) {
                JS_SetPropertyStr(ctx, param, "mfg", JS_NewArrayBufferCopy(ctx, fields.mfg_data, fields.mfg_data_len)) ;
            }
        }

        return param ;
    }

    void NimBLE::onNativeEvent(JSContext *ctx, void * param) {
        struct ble_gap_event * event = (struct ble_gap_event *) param ;

        // Nimble 定义的事件
        if(event->type<200) {
            switch (event->type) {
            case BLE_GAP_EVENT_DISC: {
                JSValue param = makeScanParam<struct ble_gap_disc_desc>(ctx, &event->disc) ;
                emitSyncFree("scaning", {param}) ;
                if(event->disc.data && event->disc.length_data) {
                    free((void*)event->disc.data) ;
                    event->disc.data = nullptr ;
                    event->disc.length_data = 0 ;
                }
                return ;
            }
    #ifdef CONFIG_BT_NIMBLE_EXT_ADV
            case BLE_GAP_EVENT_EXT_DISC: {
                // event->ext_disc ;
                // if(event->ext_disc.length_data==30) {
                //     print_block((uint8_t *)event->ext_disc.data, event->ext_disc.length_data, 8) ;
                // }
            
                // // dn(event->ext_disc.length_data)
                // printf("%d,%d,%d,%d,%d,%d\n",
                //     event->ext_disc.addr.val[0],
                //     event->ext_disc.addr.val[1],
                //     event->ext_disc.addr.val[2],
                //     event->ext_disc.addr.val[3],
                //     event->ext_disc.addr.val[4],
                //     event->ext_disc.addr.val[5]
                // ) ;

                JSValue param = makeScanParam<struct ble_gap_ext_disc_desc>(ctx, &event->ext_disc) ;
                emitSyncFree("scaning", {param}) ;

                if(event->ext_disc.data && event->ext_disc.length_data) {
                    free((void*)event->ext_disc.data) ;
                    event->ext_disc.data = nullptr ;
                    event->ext_disc.length_data = 0 ;
                }

                // printf("BLE_GAP_EVENT_EXT_DISC\n") ;
                return ;
            }
    #endif
            case BLE_GAP_EVENT_DISC_COMPLETE: {
                // dn(event->disc_complete.reason)
                emitSyncFree("scan-complete", {JS_NewInt32(ctx, event->disc_complete.reason)}) ;
                return ;
            }

            // case BLE_GAP_EVENT_CONNECT: {
            //     struct ble_gap_conn_desc desc;
            //     int rc = ble_gap_conn_find(event->connect.conn_handle, &desc);

            //     char addr[18] ;
            //     sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X"
            //         , desc.peer_id_addr.val[0]
            //         , desc.peer_id_addr.val[1]
            //         , desc.peer_id_addr.val[2]
            //         , desc.peer_id_addr.val[3]
            //         , desc.peer_id_addr.val[4]
            //         , desc.peer_id_addr.val[5]
            //     ) ;
            //     emitSyncFree("connect", {
            //         JS_NewInt32(ctx, event->connect.status),
            //         JS_NewInt32(ctx, event->connect.conn_handle),
            //         JS_NewString(ctx, addr)
            //     }) ;
            //     return ;
            // }
            case BLE_GAP_EVENT_DISCONNECT: {
                char addr[18] ;
                sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X"
                    , event->disconnect.conn.peer_id_addr.val[0]
                    , event->disconnect.conn.peer_id_addr.val[1]
                    , event->disconnect.conn.peer_id_addr.val[2]
                    , event->disconnect.conn.peer_id_addr.val[3]
                    , event->disconnect.conn.peer_id_addr.val[4]
                    , event->disconnect.conn.peer_id_addr.val[5]
                ) ;
                emitSyncFree("disconnect", {
                    JS_NewInt32(ctx, event->disconnect.reason) ,
                    JS_NewInt32(ctx, event->disconnect.conn.conn_handle),
                    JS_NewString(ctx, addr)
                }) ;
                return ;
            }
            case BLE_GAP_EVENT_NOTIFY_RX:
                /* Peer sent us a notification or indication. */
                // MODLOG_DFLT(INFO, "received %s; conn_handle=%d attr_handle=%d "
                //             "attr_len=%d\n",
                //             event->notify_rx.indication ?
                //             "indication" :
                //             "notification",
                //             event->notify_rx.conn_handle,
                //             event->notify_rx.attr_handle,
                //             OS_MBUF_PKTLEN(event->notify_rx.om));

                // /* Attribute data is contained in event->notify_rx.om. Use
                // * `os_mbuf_copydata` to copy the data received in notification mbuf */
                return ;

            case BLE_GAP_EVENT_MTU:
                printf("mtu update event; conn_handle=%d cid=%d mtu=%d\n",
                            event->mtu.conn_handle,
                            event->mtu.channel_id,
                            event->mtu.value);
                return ;

            case BLE_GAP_EVENT_EX_INIT:{
                emitSync("init", {}) ;
                return ;
            }
            case BLE_GAP_EVENT_EX_RESET:{
                emitSync("reset", {}) ;
                return ;
            }


            default:
                return ;
            }
        }

        // beshell 定义的事件
        else {
            struct js_nimble_event * jsevent = (struct js_nimble_event *) param ;
            switch (jsevent->type) {
            case JS_NIMBLE_EVENT_DISC_ALL: {
                if( jsevent->disc_all.status==0 ){
                    assert(jsevent->disc_all.peer) ;
                    Peer * npeer = new Peer(ctx, jsevent->disc_all.peer) ;
                    emitSyncFree("connect", {JS_NULL, npeer->jsobj}) ;
                }
                else {
                    emitSyncFree("connect", {JS_NewInt32(ctx, jsevent->disc_all.status)}) ;
                }
            }
            
            default:
                break;
            }
        }
    }

    JSValue NimBLE::init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        // esp_nimble_hci_and_controller_init();
        nimble_port_init();

        ble_hs_cfg.reset_cb = nimble_cb_reset;
        ble_hs_cfg.sync_cb = nimble_cb_sync;
        ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

        /* Initialize data structures to track connected peers. */
        int rc = peer_init(MYNEWT_VAL(BLE_MAX_CONNECTIONS), 64, 64, 64);
        assert(rc == 0);

        /* Set the default device name. */
        rc = ble_svc_gap_device_name_set("nimble-blecent");
        assert(rc == 0);

        /* XXX Need to have template for store */
        // ble_store_config_init();

        nimble_port_freertos_init(task_nimble);

        return JS_UNDEFINED ;
    }

    JSValue NimBLE::startScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        int dur = BLE_HS_FOREVER ;
        uint16_t itvl = 0 ;
        uint16_t window = 0 ;
        uint8_t filter_policy = 0 ;
        uint8_t limited = 0 ;
        uint8_t passive = 1 ;
        uint8_t filter_duplicates = 1 ;

        if(argc>0) {
            GET_INT32_PROP_OPT(argv[0], "dur", dur, BLE_HS_FOREVER)
            if(dur==0) {
                dur = BLE_HS_FOREVER ;
            }
            GET_UINT16_PROP_OPT(argv[0], "itvl", itvl, 0)
            GET_UINT16_PROP_OPT(argv[0], "window", window, 0)
            GET_UINT16_PROP_OPT(argv[0], "filter_policy", filter_policy, 0)
            GET_BOOL_PROP_OPT(argv[0], "limited", limited, 0)
            GET_BOOL_PROP_OPT(argv[0], "passive", passive, 1)
            GET_BOOL_PROP_OPT(argv[0], "filter_duplicates", filter_duplicates, 1)
        }

        uint8_t own_addr_type;
        struct ble_gap_disc_params disc_params;
        int rc;

        /* Figure out address to use while advertising (no privacy for now) */
        rc = ble_hs_id_infer_auto(0, &own_addr_type);
        if (rc != 0) {
            printf("error determining address type; rc=%d\n", rc);
            return JS_FALSE ;
        }


        /* Use defaults for the rest of the parameters. */
        disc_params.itvl = itvl;
        disc_params.window = window;
        disc_params.filter_policy = filter_policy;
        disc_params.filter_duplicates = filter_duplicates;
        disc_params.limited = limited;
        disc_params.passive = passive;

        // dn(disc_params.itvl)
        // dn(disc_params.window)
        // dn(disc_params.passive)
        // dn(disc_params.limited)
        // dn(disc_params.filter_policy)
        // dn(disc_params.filter_duplicates)

        NativeModule * nm = ModuleLoader::moduleByName(ctx, name) ;
        if(!nm) {
            JSTHROW("Can not found native module with name %s", name)
        }

        rc = ble_gap_disc(own_addr_type, dur, &disc_params, (ble_gap_event_fn *)gap_event_handler, nm);
        if (rc!=0) {
            printf("Error initiating GAP discovery procedure; rc=%d\n", rc);
        }

        return rc==0? JS_TRUE: JS_FALSE ;
    }

    JSValue NimBLE::stopScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ble_gap_disc_cancel() ;
        return JS_UNDEFINED ;
    }

    JSValue NimBLE::isScaning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return ble_gap_disc_active()? JS_TRUE: JS_FALSE ;
    }

    JSValue NimBLE::setName(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_CSTRING(0, name)
        int rc = ble_svc_gap_device_name_set(name);
        JS_FreeCString(ctx, name) ;
        return rc==0? JS_TRUE: JS_FALSE ;
    }

}

#endif
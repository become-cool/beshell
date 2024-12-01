#include "NimBLE.hpp"
#include "../../BeShell.hpp"
#include "JSEngine.hpp"
#include "ModuleLoader.hpp"
#include "NativeModule.hpp"
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


#define BLE_GAP_EVENT_EX_INIT 100
#define BLE_GAP_EVENT_EX_RESET 101

namespace be {

    const char * const NimBLE::name = "nimble" ;

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
        nm->emitNativeEvent(event) ;
        return 0 ;
    }


    static JSValue ble_hs_adv_fields_to_js(JSContext *ctx, const struct ble_hs_adv_fields *fields) {
        JSValue obj = JS_NewObject(ctx);

        // 添加简单字段
        JS_SetPropertyStr(ctx, obj, "flags", JS_NewUint32(ctx, fields->flags));
        JS_SetPropertyStr(ctx, obj, "tx_pwr_lvl", JS_NewInt32(ctx, fields->tx_pwr_lvl));
        JS_SetPropertyStr(ctx, obj, "appearance", JS_NewUint32(ctx, fields->appearance));

        // 转换 UUID16
        if (fields->uuids16_is_complete && fields->uuids16 && fields->num_uuids16 > 0) {
            JSValue uuids16_array = JS_NewArray(ctx);
            for (uint8_t i = 0; i < fields->num_uuids16; i++) {
                JS_SetPropertyUint32(ctx, uuids16_array, i, JS_NewUint32(ctx, fields->uuids16[i].value));
            }
            JS_SetPropertyStr(ctx, obj, "uuids16", uuids16_array);
        } else {
            JS_SetPropertyStr(ctx, obj, "uuids16", JS_NULL);
        }

        // 转换 UUID32
        if (fields->uuids32_is_complete && fields->uuids32 && fields->num_uuids32 > 0) {
            JSValue uuids32_array = JS_NewArray(ctx);
            for (uint8_t i = 0; i < fields->num_uuids32; i++) {
                JS_SetPropertyUint32(ctx, uuids32_array, i, JS_NewUint32(ctx, fields->uuids32[i].value));
            }
            JS_SetPropertyStr(ctx, obj, "uuids32", uuids32_array);
        } else {
            JS_SetPropertyStr(ctx, obj, "uuids32", JS_NULL);
        }

        // 转换 UUID128
        if (fields->uuids128_is_complete && fields->uuids128 && fields->num_uuids128 > 0) {
            JSValue uuids128_array = JS_NewArray(ctx);
            for (uint8_t i = 0; i < fields->num_uuids128; i++) {
                JSValue uuid_obj = JS_NewObject(ctx);
                JS_SetPropertyStr(ctx, uuid_obj, "value", JS_NewString(ctx, (const char *)fields->uuids128[i].value));
                JS_SetPropertyUint32(ctx, uuids128_array, i, uuid_obj);
            }
            JS_SetPropertyStr(ctx, obj, "uuids128", uuids128_array);
        } else {
            JS_SetPropertyStr(ctx, obj, "uuids128", JS_NULL);
        }

        // 转换 name
        if (fields->name_is_complete && fields->name && fields->name_len > 0) {
            JSValue name_str = JS_NewStringLen(ctx, (const char *)fields->name, (fields->name[fields->name_len-1]==0)? (fields->name_len-1): (fields->name_len) );
            JS_SetPropertyStr(ctx, obj, "name", name_str);
        } else {
            JS_SetPropertyStr(ctx, obj, "name", JS_NULL);
        }

        // 转换 tx power level
        if (fields->tx_pwr_lvl_is_present) {
            JS_SetPropertyStr(ctx, obj, "tx_pwr_lvl", JS_NewInt32(ctx, fields->tx_pwr_lvl));
        } else {
            JS_SetPropertyStr(ctx, obj, "tx_pwr_lvl", JS_NULL);
        }

        // 转换 appearance
        if (fields->appearance_is_present) {
            JS_SetPropertyStr(ctx, obj, "appearance", JS_NewUint32(ctx, fields->appearance));
        } else {
            JS_SetPropertyStr(ctx, obj, "appearance", JS_NULL);
        }

        return obj;
    }

    
    void NimBLE::onNativeEvent(JSContext *ctx, void * param) {
        struct ble_gap_event * event = (struct ble_gap_event *) param ;

        struct ble_gap_conn_desc desc;
        int rc;

        switch (event->type) {
        case BLE_GAP_EVENT_DISC: {
            struct ble_hs_adv_fields fields;
            rc = ble_hs_adv_parse_fields(&fields, event->disc.data,
                                        event->disc.length_data);
            if (rc != 0) {
                return ;
            }

            JSValue param = JS_NewObject(ctx) ;

            if(fields.name_is_complete) {
                JS_SetPropertyStr(ctx, param, "name", JS_NewStringLen(ctx, (const char *)fields.name, fields.name_len)) ;
            }

            emitSyncFree("scaning", {ble_hs_adv_fields_to_js(ctx, &fields)}) ;
            return ;
        }

        case BLE_GAP_EVENT_CONNECT: {
            emitSyncFree("connect", {JS_NewInt32(ctx, event->connect.status)}) ;
            return ;
        }
        case BLE_GAP_EVENT_DISCONNECT: {
            emitSyncFree("disconnect", {JS_NewInt32(ctx, event->disconnect.reason)}) ;
            return ;
        }
        case BLE_GAP_EVENT_DISC_COMPLETE: {
            emitSyncFree("scan-complete", {JS_NewInt32(ctx, event->disc_complete.reason)}) ;
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

        case BLE_GAP_EVENT_EXT_DISC:
            /* An advertisment report was received during GAP discovery. */
            // ext_print_adv_report(&event->disc);

            // enc_adv_data_cent_connect_if_interesting(&event->disc);
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

    JSValue NimBLE::init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        // esp_nimble_hci_and_controller_init();
        nimble_port_init();

        ble_hs_cfg.reset_cb = nimble_cb_reset;
        ble_hs_cfg.sync_cb = nimble_cb_sync;

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
        disc_params.limited = limited;
        disc_params.filter_duplicates = filter_duplicates;
        disc_params.passive = passive;

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
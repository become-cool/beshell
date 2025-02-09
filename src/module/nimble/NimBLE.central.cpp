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
#include "nimble/ble.h"
#include "host/ble_gatt.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "blecent.h"




namespace be::nimble {

    static void blecent_on_disc_complete(const struct peer *peer, int status, NimBLE * nm) {
        js_nimble_event event ;
        event.type = JS_NIMBLE_EVENT_DISC_ALL ;
        event.disc_all.status = status ;
        event.disc_all.peer = (struct peer *) peer ;

        if(!nm->emitNativeEvent(&event)){
            printf("post JS_NIMBLE_EVENT_DISC_ALL event failed\n") ;
        }
    }

    static int blecent_gap_event(struct ble_gap_event *event, NimBLE * nm) {
        struct ble_gap_conn_desc desc;

        assert(nm) ;
        // dn(event->type)

        int rc;
        switch (event->type) {

        case BLE_GAP_EVENT_CONNECT:
            /* A new connection was established or a connection attempt failed. */
            if (event->connect.status == 0) {
                /* Connection successfully established. */

                // rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
                // assert(rc == 0);
                // print_conn_desc(&desc);
                // printf("\nBLE Connection established\n");

                /* Remember peer. */
                rc = peer_add(event->connect.conn_handle);
                if (rc != 0) {
                    printf("Failed to add peer; rc=%d\n", rc);
                    // return 0;
                }
                peer_disc_all(event->connect.conn_handle, (peer_disc_fn *)blecent_on_disc_complete, nm) ;

            } else {
                /* Connection attempt failed; resume scanning. */
                printf("Error: Connection failed; status=%d\n", event->connect.status);
            }
            
            return 0;

        case BLE_GAP_EVENT_DISCONNECT:
            /* Connection terminated. */
            printf("disconnect; reason=%d \n", event->disconnect.reason);
            // print_conn_desc(&event->disconnect.conn);
            printf("\n");

            /* Forget about peer. */
            peer_delete(event->disconnect.conn.conn_handle);

            return 0;

        case BLE_GAP_EVENT_NOTIFY_RX:
            /* Peer sent us a notification or indication. */
            printf("received %s; conn_handle=%d attr_handle=%d "
                        "attr_len=%d\n",
                        event->notify_rx.indication ?
                        "indication" :
                        "notification",
                        event->notify_rx.conn_handle,
                        event->notify_rx.attr_handle,
                        OS_MBUF_PKTLEN(event->notify_rx.om));

            /* Attribute data is contained in event->notify_rx.om. Use
            * `os_mbuf_copydata` to copy the data received in notification mbuf */

        case BLE_GAP_EVENT_MTU:
            printf("mtu update event; conn_handle=%d cid=%d mtu=%d\n",
                        event->mtu.conn_handle,
                        event->mtu.channel_id,
                        event->mtu.value);

        default:
            return 0;
        }

        nm->emitNativeEvent(event) ;
    }

    JSValue NimBLE::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1) ;
        NativeModule * nm = ModuleLoader::moduleByName(ctx, name) ;
        if(!nm) {
            JSTHROW("Can not found native module with name %s", name)
        }

        ARGV_TO_CSTRING_LEN(0, caddr, caddr_len)
        if(caddr_len!=17) {
            JS_FreeCString(ctx, caddr) ;
            JSTHROW("not a valid address")
        }
        ble_addr_t bleaddr = { BLE_ADDR_RANDOM, {0} } ;

        /* Convert string to address */
        sscanf(caddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", (char *)bleaddr.val+5, (char *)bleaddr.val+4, (char *)bleaddr.val+3, (char *)bleaddr.val+2, (char *)bleaddr.val+1, (char *)bleaddr.val+0);
        JS_FreeCString(ctx, caddr) ;

        printf("\n%02x:%02x:%02x:%02x:%02x:%02x\n", bleaddr.val[0], bleaddr.val[1], bleaddr.val[2], bleaddr.val[3], bleaddr.val[4], bleaddr.val[5]);

        /* Scanning must be stopped before a connection can be initiated. */
        int rc = ble_gap_disc_cancel();
        if (rc != 0) {
            // JSTHROW("Failed to cancel scan; rc=%d\n", rc)
        }

        uint8_t own_addr_type = 0 ;
        rc = ble_hs_id_infer_auto(0, &own_addr_type);
        if(rc != 0) {
            JSTHROW("error determining address type; rc=%d\n", rc)
        }

        // int rc = ble_gap_connect(NULL, addr, BLE_HS_FOREVER, &conn_params, (ble_gap_event_fn)ble_connected, nm);
        rc = ble_gap_connect(own_addr_type, &bleaddr, 30000, NULL, (ble_gap_event_fn *)blecent_gap_event, nm);
        if(rc != 0) {
            JSTHROW("connect failed")
        }

        return JS_UNDEFINED ;
    }

    JSValue NimBLE::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1) ;
        ARGV_TO_UINT16(0, conn_handle)
        
        return ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM) == 0? JS_TRUE: JS_FALSE ;
    }

}

#endif
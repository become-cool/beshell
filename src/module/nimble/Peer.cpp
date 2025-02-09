#include "Peer.hpp"

#if CONFIG_BT_NIMBLE_ENABLED

#include "NativeClass.hpp"
#include "debug.h"
#include "qjs_utils.h"
#include "quickjs/quickjs.h"
#include "blecent.h"

using namespace std ;

namespace be::nimble {
    DEFINE_NCLASS_META(Peer, NativeClass)
    std::vector<JSCFunctionListEntry> Peer::methods = {
        JS_CFUNC_DEF("read", 0, Peer::read),
        JS_CFUNC_DEF("write", 0, Peer::write),
        JS_CFUNC_DEF("subscribe", 0, Peer::subscribe),
        JS_CFUNC_DEF("print", 0, Peer::print),
    } ;

    Peer::Peer(JSContext * ctx, void * _peer)
        : NativeClass(ctx,build(ctx,JS_NULL))
        , ble_peer(_peer)
    {
    }


    static bool parse_uuid128(const char *uuid_str, uint8_t uuid_bytes[16]) {
        // 检查长度和格式
        if (strlen(uuid_str) != 36) {
            return false; // 长度不符合 UUID 标准
        }

        // 检查格式（8-4-4-4-12）
        for (int i = 0; i < 36; i++) {
            if ((i == 8 || i == 13 || i == 18 || i == 23)) {
                if (uuid_str[i] != '-') {
                    return false; // 检查分隔符
                }
            } else {
                if (!((uuid_str[i] >= '0' && uuid_str[i] <= '9') || 
                    (uuid_str[i] >= 'a' && uuid_str[i] <= 'f') || 
                    (uuid_str[i] >= 'A' && uuid_str[i] <= 'F'))) {
                    return false; // 检查是否是合法的十六进制字符
                }
            }
        }
        // 如果格式合法，开始解析
        const char *pos = uuid_str;
        for (int i = 0; i < 16; i++) {
            char byte_str[3] = {0}; // 存储2个字符和一个结束符
            if (*pos == '-') pos++; // 跳过 '-' 分隔符
            memcpy(byte_str, pos, 2);
            uuid_bytes[i] = (uint8_t)strtol(byte_str, NULL, 16);
            pos += 2;
        }
        return true;
    }

#define ARGV_TO_UUID128(i, varname)                             \
        ARGV_TO_CSTRING_LEN(i, varname##str, varname##str_len)  \
        if(varname##str_len!=36) {                              \
            JS_FreeCString(ctx, varname##str) ;                 \
            JSTHROW("not a valid address")                      \
        }                                                       \
        uint8_t varname[16] = {0} ;                             \
        if( !parse_uuid128(varname##str, varname) ){            \
            JS_FreeCString(ctx, varname##str) ;                 \
            JSTHROW("not a valid address")                      \
        }                                                       \
        JS_FreeCString(ctx, varname##str) ;
    
    static int blecent_on_custom_subscribe(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg)
    {
        const struct peer_chr *chr;
        uint8_t value;
        int rc;
        const struct peer *peer;
        dn(error->status)
        return 0;
    }

    static int blecent_on_write(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg) {

        dn2(conn_handle, error->status)

        return 0;
    }

    JSValue Peer::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Peer, that)
        if(!that->ble_peer) {
            JSTHROW("peer is null, maybe disconnected")
        }
        struct peer * ble_peer = (struct peer *)that->ble_peer ;
        
        CHECK_ARGC(2)

        ARGV_TO_UUID128(0, svcid)
        ARGV_TO_UUID128(1, chrid)

        return JS_UNDEFINED ;
    }

    JSValue Peer::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Peer, that)
        if(!that->ble_peer) {
            JSTHROW("peer is null, maybe disconnected")
        }
        struct peer * ble_peer = (struct peer *)that->ble_peer ;
        
        CHECK_ARGC(3)

        ARGV_TO_UUID128(0, svcid)
        ARGV_TO_UUID128(1, chrid)

        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, argv[2]) ;
        if(!buff || !size) {
            JSTHROW("argv is not a valid ArrayBuffer")
        }
        
        const struct peer_chr * chr = peer_chr_find_uuid(ble_peer
                    , BLE_UUID128_DECLARE(svcid[15], svcid[14], svcid[13], svcid[12], svcid[11], svcid[10], svcid[9], svcid[8], svcid[7], svcid[6], svcid[5], svcid[4], svcid[3], svcid[2], svcid[1], svcid[0])
                    , BLE_UUID128_DECLARE(chrid[15], chrid[14], chrid[13], chrid[12], chrid[11], chrid[10], chrid[9], chrid[8], chrid[7], chrid[6], chrid[5], chrid[4], chrid[3], chrid[2], chrid[1], chrid[0]));
        if (chr == NULL) {
            JSTHROW("Error: Peer doesn't support the Alert "
                        "Notification Control Point characteristic\n");
        }

        // dn(ble_peer->conn_handle)
        int rc = ble_gattc_write_flat(ble_peer->conn_handle, chr->chr.val_handle, buff, size, blecent_on_write, NULL);
        if (rc != 0) {
            JSTHROW("Error: Failed to write characteristic; rc=%d\n", rc);
        }

        return JS_UNDEFINED ;
    }
    
    
    JSValue Peer::subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Peer, that)
        if(!that->ble_peer) {
            JSTHROW("peer is null, maybe disconnected")
        }
        struct peer * ble_peer = (struct peer *)that->ble_peer ;
        
        CHECK_ARGC(2)

        ARGV_TO_UUID128(0, svcid)
        ARGV_TO_UUID128(1, chrid)
    
        const struct peer_dsc *dsc = peer_dsc_find_uuid(ble_peer
                , BLE_UUID128_DECLARE(svcid[15], svcid[14], svcid[13], svcid[12], svcid[11], svcid[10], svcid[9], svcid[8], svcid[7], svcid[6], svcid[5], svcid[4], svcid[3], svcid[2], svcid[1], svcid[0])
                , BLE_UUID128_DECLARE(chrid[15], chrid[14], chrid[13], chrid[12], chrid[11], chrid[10], chrid[9], chrid[8], chrid[7], chrid[6], chrid[5], chrid[4], chrid[3], chrid[2], chrid[1], chrid[0])
                , BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16));
        if (dsc == NULL) {
            JSTHROW("unknown service or charecteristic")
        }
        
        /*** Write 0x00 and 0x01 (The subscription code) to the CCCD ***/
        uint8_t value[2] = {1,0} ;
        // int rc = ble_gattc_write_flat(ble_peer->conn_handle, dsc->dsc.handle,
        //                         value, sizeof(value), blecent_on_custom_subscribe, NULL);
        int rc = ble_gattc_write_no_rsp_flat(ble_peer->conn_handle, dsc->dsc.handle, value, sizeof(value));
        
        if (rc != 0) {
            JSTHROW("Error: Failed to subscribe to the subscribable characteristic; rc=%d\n", rc)
        }

        return JS_UNDEFINED ;
    }
    
    static void print_uuid_(const ble_uuid_t *uuid) {
        char buf[BLE_UUID_STR_LEN];
        printf("%s", ble_uuid_to_str(uuid, buf));
    }

    JSValue Peer::print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Peer, that)
        if(!that->ble_peer) {
            JSTHROW("peer is null, maybe disconnected")
        }
        const struct peer * _peer = (const struct peer *)that->ble_peer ;

        struct peer_svc *svc;
        // struct peer_chr *prev;
        struct peer_chr *chr;

        SLIST_FOREACH(svc, &_peer->svcs, next) {

            printf("svc: ") ;
            print_uuid_(&svc->svc.uuid.u) ;
            printf("\n") ;

            
            SLIST_FOREACH(chr, &svc->chrs, next) {
                // if (chr->chr.val_handle >= chr_val_handle) {
                //     break;
                // }

                // prev = chr;

                
            }

            

            // if (ble_uuid_cmp(&svc->svc.uuid.u, uuid) == 0) {
            //     return svc;
            // }
        }

        return JS_UNDEFINED ;
    }
}

#endif

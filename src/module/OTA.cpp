#include "OTA.hpp"
#include "ota.hpp"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include <esp_wifi.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>
#include <esp_netif.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "quickjs/quickjs.h"
#include "ModuleLoader.hpp"
#include "../js/ota.c"
#include "esp_ota_ops.h"
#include "Flash.hpp"

typedef struct OTAEvent {
    int ev ;
    int value ;
} OTAEvent_t;


namespace be{

    char const * const OTA::name = "ota" ;

    static const char * runningFSPart = nullptr ;

    OTA::OTA(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportName("start") ;
        exportName("rollback") ;
        exportName("commit") ;

        EXPORT_FUNCTION(getBootPartition)
        EXPORT_FUNCTION(setBootPartition)
        EXPORT_FUNCTION(getRunningPartition)
        EXPORT_FUNCTION(getNextUpdatePartition)
        EXPORT_FUNCTION(markValid)
        EXPORT_FUNCTION(markInvalid)
        EXPORT_FUNCTION(isRollbackEnabled)
    }

    void OTA::use(be::BeShell * beshell) {
        beshell->use<be::NVS>() ;
        beshell->use<be::Flash>() ;
    }

    void OTA::exports(JSContext *ctx) {
        JSEngineEvalEmbeded(ctx, ota)
    }

    JSValue OTA::getBootPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        const esp_partition_t * partition = esp_ota_get_boot_partition() ;
        if(!partition) {
            JSTHROW("get boot partition failed")
        }
        auto p = new Partition(partition, ctx) ;
        return p->jsobj ;
    }

    JSValue OTA::setBootPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        if(!JS_IsObject(argv[0])) {
            JSTHROW("argument must be a partition object")
        }
        JSVALUE_TO_NCLASS(Partition, argv[0], partition)
        esp_err_t err = esp_ota_set_boot_partition(partition->partition) ;
        if(err!=ESP_OK) {
            JSTHROW("set boot partition failed: %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue OTA::getRunningPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        const char* type = "bin";
        
        if (argc > 0 && JS_IsString(argv[0])) {
            const char* typeArg = JS_ToCString(ctx, argv[0]);
            if (typeArg) {
                if (strcmp(typeArg, "bin") == 0 || strcmp(typeArg, "fs") == 0) {
                    type = typeArg;
                } else {
                    JS_FreeCString(ctx, typeArg);
                    JSTHROW("Invalid type. Must be 'bin' or 'fs'");
                }
            }
        }

        if (strcmp(type, "bin") == 0) {
            const esp_partition_t* partition = esp_ota_get_running_partition();
            if (!partition) {
                if (argc > 0 && JS_IsString(argv[0])) {
                    JS_FreeCString(ctx, type);
                }
                JSTHROW("get running partition failed");
            }
            auto p = new Partition(partition, ctx);
            if (argc > 0 && JS_IsString(argv[0])) {
                JS_FreeCString(ctx, type);
            }
            return p->jsobj;
        } else { // type == "fs"
            char buff[128];
                const esp_partition_t* partition = esp_partition_find_first(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, runningFSPart);
                if (!partition) {
                    if (argc > 0 && JS_IsString(argv[0])) {
                        JS_FreeCString(ctx, type);
                    }
                    JSTHROW("get running fs partition failed");
                }
                auto p = new Partition(partition, ctx);
                if (argc > 0 && JS_IsString(argv[0])) {
                    JS_FreeCString(ctx, type);
                }
                return p->jsobj;
        }
    }

    JSValue OTA::getNextUpdatePartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        const esp_partition_t * partition = esp_ota_get_next_update_partition(NULL) ;
        if(!partition) {
            JSTHROW("get running partition failed")
        }
        auto p = new Partition(partition, ctx) ;
        return p->jsobj ;
    }

    JSValue OTA::markValid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        NVS::writeString("fsroot.boot", runningFSPart) ;
        esp_err_t err = esp_ota_mark_app_valid_cancel_rollback() ;
        if(err!=ESP_OK) {
            JSTHROW("mark valid failed: %d", err)
        }
        return JS_UNDEFINED ;
    }
    JSValue OTA::markInvalid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        esp_err_t err = esp_ota_mark_app_invalid_rollback_and_reboot() ;
        if(err!=ESP_OK) {
            JSTHROW("mark valid failed: %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue OTA::isRollbackEnabled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        #if CONFIG_APP_ROLLBACK_ENABLE
            return JS_TRUE ;
        #else
            return JS_FALSE ;
        #endif
    }

    const char * OTA::whichFS(const char * fsPartName) {
        static char buff[128] ;
        if( NVS::readString("fsroot.boot.try", buff, sizeof(buff))==ESP_OK ) {
            NVS::erase("fsroot.boot.try") ;
        }
        else if( NVS::readString("fsroot.boot", buff, sizeof(buff))==ESP_OK ) {
            runningFSPart = buff ;
        }
        else {
            runningFSPart = fsPartName ;
        }
        return runningFSPart ;
    }
}

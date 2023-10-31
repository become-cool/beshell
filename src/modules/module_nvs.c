#include "module_nvs.h"
#include "utils.h"
#include <sys/stat.h>


#ifdef PLATFORM_ESP32

#include "esp_system.h"
#include "nvs_flash.h"


#define NVS_OPEN(h, failed)                                 \
    nvs_handle_t h;                                         \
    esp_err_t err = nvs_open("beshell", NVS_READWRITE, &h); \
    if(err!=ESP_OK) {                                       \
        failed                                              \
    }

#define NVS_INT_GETTER(type,ctype,jstype)                   \
    static JSValue js_nvs_read_##type(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        CHECK_ARGC(1)                                       \
        ARGV_TO_STRING(0,key)                               \
        JSValue result = JS_NULL;                           \
        NVS_OPEN(handle, {                                  \
            goto ret ;                                      \
        })                                                  \
        ctype v ;                                           \
        if(nvs_get_##type(handle, key, &v)==ESP_OK) {       \
            result = JS_New##jstype(ctx, v) ;               \
            if( argc>1 && JS_ToBool(ctx,argv[1]) ) {        \
                nvs_erase_key(handle, key) ;                \
            }                                               \
        }                                                   \
        nvs_close(handle) ;                                 \
    ret:                                                    \
        JS_FreeCString(ctx, key) ;                          \
        return result ;                                     \
    }

NVS_INT_GETTER(i8, int8_t, Int32)
NVS_INT_GETTER(i16, int16_t, Int32)
NVS_INT_GETTER(i32, int32_t, Int32)
NVS_INT_GETTER(i64, int64_t, Int64)
NVS_INT_GETTER(u8, uint8_t, Uint32)
NVS_INT_GETTER(u16, uint16_t, Uint32)
NVS_INT_GETTER(u32, uint32_t, Uint32)


#define NVS_INT_SETTER(type,ctype,jstype)                   \
    static JSValue js_nvs_write_##type(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        CHECK_ARGC(2)                                       \
        ARGV_TO_STRING(0,key)                               \
        ctype value = 0 ;                                   \
        if(JS_To##jstype(ctx, &value, argv[1])!=0) {        \
            JS_FreeCString(ctx, key) ;                      \
            THROW_EXCEPTION("arg %s is not a number", "value") \
        }                                                   \
        JSValue result = JS_FALSE ;                         \
        NVS_OPEN(handle, {                                  \
            goto ret ;                                      \
        })                                                  \
        result = nvs_set_##type(handle, key, value)==ESP_OK;\
        nvs_close(handle) ;                                 \
    ret:                                                    \
        JS_FreeCString(ctx, key) ;                          \
        return result ;                                     \
    }

NVS_INT_SETTER(i8, int8_t, Int32)
NVS_INT_SETTER(i16, int16_t, Int32)
NVS_INT_SETTER(i32, int32_t, Int32)
NVS_INT_SETTER(i64, int64_t, Int64)
NVS_INT_SETTER(u8, uint8_t, Uint32)
NVS_INT_SETTER(u16, uint16_t, Uint32)
NVS_INT_SETTER(u32, uint32_t, Uint32)

static JSValue js_nvs_erase_key(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0,key)

    JSValue result = JS_FALSE;
    
    NVS_OPEN(handle, {
        goto ret ;
    })
    result = nvs_erase_key(handle, key)==ESP_OK? JS_TRUE: JS_FALSE;
    nvs_close(handle) ;

ret:
    JS_FreeCString(ctx, key) ;
    return result ;
}

#endif


void nvs_read_onetime(const char * key, uint8_t*pvalue) {
#ifdef PLATFORM_ESP32
    NVS_OPEN(handle, {
        return ;
    })
    if(nvs_get_u8(handle, key, pvalue)==ESP_OK) {
        nvs_erase_key(handle, key) ;
    }
    nvs_close(handle) ;
#else
#endif
}

#ifdef PLATFORM_ESP32
static JSValue js_nvs_write_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_STRING(0,key)
    ARGV_TO_STRING(1,value)

    JSValue result = JS_FALSE;
    
    NVS_OPEN(handle, {
        goto ret ;
    })
    result = nvs_set_str(handle, key, value)==ESP_OK? JS_TRUE: JS_FALSE;
    nvs_close(handle) ;

ret:
    JS_FreeCString(ctx, key) ;
    JS_FreeCString(ctx, value) ;
    return result ;
}

static JSValue js_nvs_read_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0,key)

    char* cvalue = NULL ;
    JSValue result = JS_NULL;
    
    NVS_OPEN(handle, {
        goto ret ;
    })

    size_t cvalue_len = 0;
    if(nvs_get_str(handle, key, NULL, &cvalue_len)!=ESP_OK) {
        nvs_close(handle) ;
        goto ret ;
    }
    cvalue = malloc(cvalue_len);
    if(!cvalue) {
        nvs_close(handle) ;
        goto ret ;
    }
    if(nvs_get_str(handle, key, cvalue, &cvalue_len)!=ESP_OK){
        nvs_close(handle) ;
        goto ret ;
    }
    
    result = JS_NewStringLen(ctx, cvalue, cvalue_len-1) ;

    if( argc>1 && JS_ToBool(ctx,argv[1]) ) {
        nvs_erase_key(handle, key) ;
    }

    nvs_close(handle) ;

ret:
    JS_FreeCString(ctx, key) ;
    if(cvalue) {
        free(cvalue) ;
    }
    return result ;
}
#endif

// static JSValue js_nvs_set_next_boot_level(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(1)
//     ARGV_TO_UINT8(0,level)
//     JSValue result = JS_FALSE;
// #ifdef PLATFORM_ESP32
//     NVS_OPEN(handle, {
//         goto ret ;
//     })
//     result = nvs_set_u8(handle, "next_boot_lv", level)==ESP_OK? JS_TRUE: JS_FALSE; ;
//     nvs_close(handle) ;
// #else
// #ifdef WIN32
//         mkdir("./nvs") ;
// #else
//         mkdir("./nvs", ACCESSPERMS) ;
// #endif
//     FILE * h = fopen("./nvs/next_boot_lv.bin","w") ;
//     if(!h) {
//         return JS_FALSE ;
//     }
//     result = fwrite(&level, 1, 1, h) == 1? JS_TRUE: JS_FALSE ;
//     fclose(h) ;
// #endif

// ret:
//     return result ;
// }

void be_module_nvs_init() {}

void be_module_nvs_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue nvs = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "nvs", nvs);

#ifdef PLATFORM_ESP32
    JS_SetPropertyStr(ctx, nvs, "erase", JS_NewCFunction(ctx, js_nvs_erase_key, "erase", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt8", JS_NewCFunction(ctx, js_nvs_read_i8, "readInt8", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt16", JS_NewCFunction(ctx, js_nvs_read_i16, "readInt16", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt32", JS_NewCFunction(ctx, js_nvs_read_i32, "readInt32", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt64", JS_NewCFunction(ctx, js_nvs_read_i64, "readInt64", 1));

    JS_SetPropertyStr(ctx, nvs, "readUint8", JS_NewCFunction(ctx, js_nvs_read_u8, "readUint8", 1));
    JS_SetPropertyStr(ctx, nvs, "readUint16", JS_NewCFunction(ctx, js_nvs_read_u16, "readUint16", 1));
    JS_SetPropertyStr(ctx, nvs, "readUint32", JS_NewCFunction(ctx, js_nvs_read_u32, "readUint32", 1));

    JS_SetPropertyStr(ctx, nvs, "writeInt8", JS_NewCFunction(ctx, js_nvs_write_i8, "writeInt8", 1));
    JS_SetPropertyStr(ctx, nvs, "writeInt16", JS_NewCFunction(ctx, js_nvs_write_i16, "writeInt16", 1));
    JS_SetPropertyStr(ctx, nvs, "writeInt32", JS_NewCFunction(ctx, js_nvs_write_i32, "writeInt32", 1));
    JS_SetPropertyStr(ctx, nvs, "writeInt64", JS_NewCFunction(ctx, js_nvs_write_i64, "writeInt64", 1));

    JS_SetPropertyStr(ctx, nvs, "writeUint8", JS_NewCFunction(ctx, js_nvs_write_u8, "writeUint8", 1));
    JS_SetPropertyStr(ctx, nvs, "writeUint16", JS_NewCFunction(ctx, js_nvs_write_u16, "writeUint16", 1));
    JS_SetPropertyStr(ctx, nvs, "writeUint32", JS_NewCFunction(ctx, js_nvs_write_u32, "writeUint32", 1));

    JS_SetPropertyStr(ctx, nvs, "readString", JS_NewCFunction(ctx, js_nvs_read_str, "readString", 1));
    JS_SetPropertyStr(ctx, nvs, "writeString", JS_NewCFunction(ctx, js_nvs_write_str, "writeString", 1));
#endif

    JS_FreeValue(ctx, beapi);
}

void be_module_nvs_loop(JSContext *ctx) {}

void be_module_nvs_reset(JSContext *ctx) {}


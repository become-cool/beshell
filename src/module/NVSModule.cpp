#include "NVSModule.hpp"
#include "debug.h"
#include "utils.h"
#include <iostream>


#ifdef PLATFORM_ESP32

#include "esp_system.h"
#include "nvs_flash.h"


#define NVS_OPEN(ns, h, failed)                                 \
    nvs_handle_t h;                                         \
    esp_err_t err = nvs_open(ns, NVS_READWRITE, &h); \
    if(err!=ESP_OK) {                                       \
        failed                                              \
    }

#define NVS_INT_GETTER(name,type,ctype)                             \
        bool NVSModule::read##name(const char * key, ctype & value, const char * ns) { \
            NVS_OPEN(ns,handle, {                                   \
                return false ;                                      \
            })                                                      \
            bool res = nvs_get_##type(handle, key, &value)==ESP_OK; \
            nvs_close(handle) ;                                     \
            return res ;                                            \
        }

#define NVS_INT_SETTER(name,type,ctype)                                         \
    bool NVSModule::write##name(const char * key, ctype value, const char * ns) {     \
        NVS_OPEN(ns,handle, {                                       \
            return false ;                                          \
        })                                                          \
        bool res = nvs_set_##type(handle, key, value)==ESP_OK;      \
        nvs_close(handle) ;                                         \
        return res ;                                                \
    }


#endif

#ifdef PLATFORM_LINUX
#define NVS_INT_GETTER(name,type,ctype)                             \
        bool NVSModule::read##name(const char * key, ctype & value, const char * ns) { \
            return false ;                                            \
        }

#define NVS_INT_SETTER(name,type,ctype)                                         \
    bool NVSModule::write##name(const char * key, ctype value, const char * ns) {     \
        return false ;                                                \
    }
#endif

#define NVS_INT_JS_GETTER(name,ctype,jstype)                        \
    JSValue NVSModule::jsRead##name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        CHECK_ARGC(1)                           \
        ARGV_TO_CSTRING(0,key)                  \
        ctype value = 0 ;                       \
        if(read##name(key, value, "beshell")){  \
            return JS_New##jstype(ctx, value) ; \
        }                                       \
        else {                                  \
            return JS_NULL ;                    \
        }                                       \
    }

#define NVS_INT_JS_SETTER(name,ctype,jstype,apitype)               \
    JSValue NVSModule::jsWrite##name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        CHECK_ARGC(2)                                                   \
        ARGV_TO_CSTRING(0,key)                                          \
        ctype value = 0 ;                                               \
        if(JS_To##jstype(ctx, (apitype*)&value, argv[1])!=0) {          \
            JS_FreeCString(ctx, key) ;                                  \
            THROW_EXCEPTION("arg %s is not a number", "value")          \
        }                                                               \
        return write##name(key, value, "beshell")? JS_TRUE: JS_FALSE ;  \
    }


namespace be {
    NVSModule::NVSModule(): NativeModule("nvs") {
        isReplGlobal = true ;
    }

    void NVSModule::defineExports() {
        
        exportFunction("jsErase", jsErase) ;
        // exportFunction("jsReadString", jsReadString) ;
        // exportFunction("jsWriteString", jsWriteString) ;

        exportFunction("readInt8", jsReadInt8) ;
        exportFunction("readInt16", jsReadInt16) ;
        exportFunction("readInt32", jsReadInt32) ;
        exportFunction("readInt64", jsReadInt64) ;
        exportFunction("readUint8", jsReadUint8) ;
        exportFunction("readUint16", jsReadUint16) ;
        exportFunction("readUint32", jsReadUint32) ;
        
        exportFunction("writeInt8", jsWriteInt8) ;
        exportFunction("writeInt16", jsWriteInt16) ;
        exportFunction("writeInt32", jsWriteInt32) ;
        exportFunction("writeInt64", jsWriteInt64) ;
        exportFunction("writeUint8", jsWriteUint8) ;
        exportFunction("writeUint16", jsWriteUint16) ;
        exportFunction("writeUint32", jsWriteUint32) ;
    }

    void NVSModule::setup(JSContext * ctx){
#ifdef PLATFORM_ESP32
        esp_err_t ret = nvs_flash_init();
        if(ret!=ESP_OK) {
            std::cout << "nvs_flash_init() failed: " << ret << std::endl ;
        }
#endif
    }
    
    // c/c++ helper api
    NVS_INT_GETTER(Int8, i8, int8_t)
    NVS_INT_GETTER(Int16, i16, int16_t)
    NVS_INT_GETTER(Int32, i32, int32_t)
    NVS_INT_GETTER(Int64, i64, int64_t)
    NVS_INT_GETTER(Uint8, u8, uint8_t)
    NVS_INT_GETTER(Uint16, u16, uint16_t)
    NVS_INT_GETTER(Uint32, u32, uint32_t)

    NVS_INT_SETTER(Int8, i8, int8_t)
    NVS_INT_SETTER(Int16, i16, int16_t)
    NVS_INT_SETTER(Int32, i32, int32_t)
    NVS_INT_SETTER(Int64, i64, int64_t)
    NVS_INT_SETTER(Uint8, u8, uint8_t)
    NVS_INT_SETTER(Uint16, u16, uint16_t)
    NVS_INT_SETTER(Uint32, u32, uint32_t)
    
    bool NVSModule::readFloat(const char * key, float & value, const char * ns) {
        int32_t ival = * (int32_t*)&value ;
        if(readInt32(key, ival, ns)){
            value = * (float*)&ival ;
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVSModule::writeFloat(const char * key, float value, const char * ns) {
        int32_t ival = * (int32_t*)&value ;
        return writeInt32(key, ival, ns) ;
    }
    
    bool NVSModule::readDouble(const char * key, double & value, const char * ns) {
        int64_t ival = * (int64_t*)&value ;
        if(readInt64(key, ival, ns)){
            value = * (double*)&ival ;
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVSModule::writeDouble(const char * key, double value, const char * ns) {
        int64_t ival = * (int64_t*)&value ;
        return writeInt64(key, ival, ns) ;
    }

    // js api
    NVS_INT_JS_GETTER(Int8, int8_t, Int32)
    NVS_INT_JS_GETTER(Int16, int16_t, Int32)
    NVS_INT_JS_GETTER(Int32, int32_t, Int32)
    NVS_INT_JS_GETTER(Int64, int64_t, Int64)
    NVS_INT_JS_GETTER(Uint8, uint8_t, Uint32)
    NVS_INT_JS_GETTER(Uint16, uint16_t, Uint32)
    NVS_INT_JS_GETTER(Uint32, uint32_t, Uint32)

    NVS_INT_JS_SETTER(Int8, int8_t, Int32, int32_t)
    NVS_INT_JS_SETTER(Int16, int16_t, Int32, int32_t)
    NVS_INT_JS_SETTER(Int32, int32_t, Int32, int32_t)
    NVS_INT_JS_SETTER(Int64, int64_t, Int64, int64_t)
    NVS_INT_JS_SETTER(Uint8, uint8_t, Uint32, uint32_t)
    NVS_INT_JS_SETTER(Uint16, uint16_t, Uint32, uint32_t)
    NVS_INT_JS_SETTER(Uint32, uint32_t, Uint32, uint32_t)

    JSValue NVSModule::jsErase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_CSTRING(0,key)

#ifdef PLATFORM_ESP32
        JSValue result = JS_FALSE;
        
        NVS_OPEN("beshell", handle, {
            goto ret ;
        })
        result = nvs_erase_key(handle, key)==ESP_OK? JS_TRUE: JS_FALSE;
        nvs_close(handle) ;

    ret:
        JS_FreeCString(ctx, key) ;
        return result ;
#endif

#ifdef PLATFORM_LINUX
        return JS_FALSE ;
#endif

    }

    void NVSModule::readOneTime(const char * key, uint8_t * value) const {
#ifdef PLATFORM_ESP32
        NVS_OPEN("beshell", handle, {
            return ;
        })
        if(nvs_get_u8(handle, key, value)==ESP_OK) {
            nvs_erase_key(handle, key) ;
        }
        nvs_close(handle) ;
#endif
    }
}
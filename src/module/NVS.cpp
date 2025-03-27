#include "NVS.hpp"
#include "debug.h"
#include "qjs_utils.h"
#include <iostream>

using namespace std ;

#ifdef ESP_PLATFORM

#include "esp_system.h"
#include "nvs_flash.h"


#define NVS_OPEN(ns, h, failed)                             \
    nvs_handle_t h;                                         \
    esp_err_t err = nvs_open(ns, NVS_READWRITE, &h);        \
    if(err!=ESP_OK) {                                       \
        failed                                              \
    }

#define JS_NVS_OPEN(ns, h, failed)                          \
    NVS_OPEN(ns, h, {                                       \
        failed                                              \
        JSTHROW("nvs_open(%s) failed: %d", ns, err) ;       \
    })


#define NVS_INT_GETTER(name,type,ctype)                             \
        bool NVS::read##name(const char * key, ctype & value, const char * ns) { \
            NVS_OPEN(ns,handle, {                                   \
                return false ;                                      \
            })                                                      \
            bool res = nvs_get_##type(handle, key, &value)==ESP_OK; \
            nvs_close(handle) ;                                     \
            return res ;                                            \
        }

#define NVS_INT_SETTER(name,type,ctype)                                         \
    bool NVS::write##name(const char * key, ctype value, const char * ns) {     \
        NVS_OPEN(ns,handle, {                                       \
            return false ;                                          \
        })                                                          \
        bool res = nvs_set_##type(handle, key, value)==ESP_OK;      \
        nvs_close(handle) ;                                         \
        return res ;                                                \
    }


#endif

#ifdef LINUX_PLATFORM
#define NVS_INT_GETTER(name,type,ctype)                             \
        bool NVS::read##name(const char * key, ctype & value, const char * ns) { \
            return false ;                                            \
        }

#define NVS_INT_SETTER(name,type,ctype)                                         \
    bool NVS::write##name(const char * key, ctype value, const char * ns) {     \
        return false ;                                                \
    }
#endif

#define NVS_INT_JS_GETTER(name,ctype,jstype)                        \
    JSValue NVS::read##name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        ASSERT_ARGC(1)                           \
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
    JSValue NVS::write##name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        ASSERT_ARGC(2)                                                   \
        ARGV_TO_CSTRING(0,key)                                          \
        ctype value = 0 ;                                               \
        if(JS_To##jstype(ctx, (apitype*)&value, argv[1])!=0) {          \
            JS_FreeCString(ctx, key) ;                                  \
            JSTHROW("arg %s is not a number", "value")          \
        }                                                               \
        return write##name(key, value, "beshell")? JS_TRUE: JS_FALSE ;  \
    }


namespace be {
    
    char const * const NVS::name = "nvs" ;

    void NVS::use(BeShell * beshell) {
#ifdef ESP_PLATFORM
        esp_err_t ret = nvs_flash_init();
        if(ret!=ESP_OK) {
            std::cout << "nvs_flash_init() failed: " << ret << std::endl ;
        }
#endif
    }

    NVS::NVS(JSContext * ctx, const char * name,uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
        exportFunction("erase", erase) ;
        exportFunction("readString", readString) ;
        exportFunction("writeString", writeString) ;

        exportFunction("readInt8", readInt8) ;
        exportFunction("readInt16", readInt16) ;
        exportFunction("readInt32", readInt32) ;
        exportFunction("readInt64", readInt64) ;
        exportFunction("readUint8", readUint8) ;
        exportFunction("readUint16", readUint16) ;
        exportFunction("readUint32", readUint32) ;
        
        exportFunction("writeInt8", writeInt8) ;
        exportFunction("writeInt16", writeInt16) ;
        exportFunction("writeInt32", writeInt32) ;
        exportFunction("writeInt64", writeInt64) ;
        exportFunction("writeUint8", writeUint8) ;
        exportFunction("writeUint16", writeUint16) ;
        exportFunction("writeUint32", writeUint32) ;
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
    
    int NVS::erase(const char * key, const char * ns) {
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_erase_key(handle, key) ;
        nvs_close(handle) ;
        return (int)err ;
    }

    bool NVS::readFloat(const char * key, float & value, const char * ns) {
        int32_t ival = * (int32_t*)&value ;
        if(readInt32(key, ival, ns)){
            value = * (float*)&ival ;
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVS::writeFloat(const char * key, float value, const char * ns) {
        int32_t ival = * (int32_t*)&value ;
        return writeInt32(key, ival, ns) ;
    }
    
    bool NVS::readDouble(const char * key, double & value, const char * ns) {
        int64_t ival = * (int64_t*)&value ;
        if(readInt64(key, ival, ns)){
            value = * (double*)&ival ;
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVS::writeDouble(const char * key, double value, const char * ns) {
        int64_t ival = * (int64_t*)&value ;
        return writeInt64(key, ival, ns) ;
    }

    
    int NVS::readString(const char * key, char * buff, size_t buffsize, const char * ns) {
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_get_str(handle, key, buff, & buffsize);
        nvs_close(handle) ;
        return (int)err ;
    }
    int NVS::writeString(const char * key, const char * value, const char * ns) {
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_set_str(handle, key, value);
        nvs_close(handle) ;
        return (int)err ;
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

    JSValue NVS::erase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)

#ifdef ESP_PLATFORM
        ARGV_TO_CSTRING(0,key)
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

#ifdef LINUX_PLATFORM
        return JS_FALSE ;
#endif

    }

    JSValue NVS::readString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        string ARGV_TO_STRING(0,key)
        ARGV_TO_UINT32_OPT(1,buff_size,128)

        if(buff_size<1) {
            JSTHROW("buff_size must be greater than 0") ;
        }

        JSValue strvalue = JS_NULL ;

#ifdef ESP_PLATFORM

        JS_NVS_OPEN("beshell", handle, )

        // size_t buff_size = 0 ;
        // esp_err_t res = nvs_get_blob(handle, key.c_str(), NULL, &buff_size) ;
        // if(res!= ESP_OK) {
        //     ds(key.c_str())
        //     dn(buff_size)
        //     JSTHROW("nvs_get_blob() failed: %d", res) ;
        // }

        char * value = (char *)malloc(buff_size) ;
        if(!value) {
            nvs_close(handle) ;
            JSTHROW("malloc(%d) failed", buff_size) ;
        }

        esp_err_t res = nvs_get_str(handle, key.c_str(), value, (size_t*)&buff_size);
        if(res==ESP_ERR_NVS_NOT_FOUND) {
            return JS_NULL ;
        }
        if(res!= ESP_OK) {
            free(value) ;
            nvs_close(handle) ;
            JSTHROW("nvs_get_str() failed: %d", res) ;
        }
        strvalue = JS_NewString(ctx, value) ;

        nvs_close(handle) ;
        free(value) ;
#endif

        return strvalue ;
    }

    JSValue NVS::writeString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        string ARGV_TO_STRING(0,key)
        string ARGV_TO_STRING(1,value)

        JSValue ret = JS_FALSE ;
#ifdef ESP_PLATFORM
        JS_NVS_OPEN("beshell", handle, )
        ret = nvs_set_str(handle, key.c_str(), value.c_str()) == ESP_OK ? JS_TRUE : JS_FALSE ;
        nvs_close(handle) ;
#endif

        return ret ;
    }

    void NVS::readOneTime(const char * key, uint8_t * value) const {
#ifdef ESP_PLATFORM
        NVS_OPEN("beshell", handle, {})
        if(nvs_get_u8(handle, key, value)==ESP_OK) {
            nvs_erase_key(handle, key) ;
        }
        nvs_close(handle) ;
#endif
    }
}
#include "NVS.hpp"
#include "debug.h"
#include "qjs_utils.h"
#include <iostream>
#include <cstring>

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

#ifndef ESP_PLATFORM
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
#ifdef ESP_PLATFORM
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_erase_key(handle, key) ;
        nvs_close(handle) ;
        return (int)err ;
#else
        return -1 ;
#endif
    }

    bool NVS::readFloat(const char * key, float & value, const char * ns) {
        int32_t ival = 0;
        if(readInt32(key, ival, ns)){
            static_assert(sizeof(float) == sizeof(int32_t), "float size mismatch");
            std::memcpy(&value, &ival, sizeof(value));
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVS::writeFloat(const char * key, float value, const char * ns) {
        static_assert(sizeof(float) == sizeof(int32_t), "float size mismatch");
        int32_t ival = 0;
        std::memcpy(&ival, &value, sizeof(value));
        return writeInt32(key, ival, ns) ;
    }
    
    bool NVS::readDouble(const char * key, double & value, const char * ns) {
        int64_t ival = 0;
        if(readInt64(key, ival, ns)){
            static_assert(sizeof(double) == sizeof(int64_t), "double size mismatch");
            std::memcpy(&value, &ival, sizeof(value));
            return true ;
        }
        else {
            return false ;
        }
    }
    bool NVS::writeDouble(const char * key, double value, const char * ns) {
        static_assert(sizeof(double) == sizeof(int64_t), "double size mismatch");
        int64_t ival = 0;
        std::memcpy(&ival, &value, sizeof(value));
        return writeInt64(key, ival, ns) ;
    }

    
    int NVS::readString(const char * key, char * buff, size_t buffsize, const char * ns) {
#ifdef ESP_PLATFORM
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_get_str(handle, key, buff, & buffsize);
        nvs_close(handle) ;
        return (int)err ;
#else
        return -1 ;
#endif
    }
    int NVS::writeString(const char * key, const char * value, const char * ns) {
#ifdef ESP_PLATFORM
        nvs_handle_t handle;
        esp_err_t err = nvs_open(ns, NVS_READWRITE, &handle);
        if(err!=ESP_OK) {
            return (int)err ;
        }
        err = nvs_set_str(handle, key, value);
        nvs_close(handle) ;
        return (int)err ;
#else
        return -1 ;
#endif
    }

    // js api

    /**
     * 从 NVS 读取有符号 8 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 int8 值
     * 
     * @module nvs
     * @function readInt8
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值（-128 ~ 127），不存在时返回 null
     */
    NVS_INT_JS_GETTER(Int8, int8_t, Int32)

    /**
     * 从 NVS 读取有符号 16 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 int16 值
     * 
     * @module nvs
     * @function readInt16
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值（-32768 ~ 32767），不存在时返回 null
     */
    NVS_INT_JS_GETTER(Int16, int16_t, Int32)

    /**
     * 从 NVS 读取有符号 32 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 int32 值。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 写入整数
     * nvs.writeInt32("counter", 42)
     * 
     * // 读取整数
     * const count = nvs.readInt32("counter")
     * console.log("Count:", count)  // 42
     * 
     * // 读取不存在的键返回 null
     * const notExist = nvs.readInt32("nonexistent")
     * console.log(notExist)  // null
     * 
     * // 用于存储时间戳
     * nvs.writeInt32("lastBoot", Date.now())
     * const lastBoot = nvs.readInt32("lastBoot")
     * console.log("Last boot:", new Date(lastBoot))
     * ```
     *
     * @module nvs
     * @function readInt32
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值，不存在时返回 null
     */
    NVS_INT_JS_GETTER(Int32, int32_t, Int32)

    /**
     * 从 NVS 读取有符号 64 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 int64 值
     * 
     * @module nvs
     * @function readInt64
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值，不存在时返回 null
     */
    NVS_INT_JS_GETTER(Int64, int64_t, Int64)

    /**
     * 从 NVS 读取无符号 8 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 uint8 值。
     * 适合存储小范围数值（0-255），如状态标志、模式选择等。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 存储设备模式（0=正常, 1=省电, 2=性能）
     * nvs.writeUint8("deviceMode", 1)
     * 
     * // 读取设备模式
     * const mode = nvs.readUint8("deviceMode")
     * console.log("Device mode:", mode)
     * 
     * // 存储布尔值（0=false, 1=true）
     * nvs.writeUint8("firstBoot", 0)
     * const isFirstBoot = nvs.readUint8("firstBoot") !== 0
     * console.log("First boot:", isFirstBoot)
     * ```
     *
     * @module nvs
     * @function readUint8
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值（0 ~ 255），不存在时返回 null
     */
    NVS_INT_JS_GETTER(Uint8, uint8_t, Uint32)

    /**
     * 从 NVS 读取无符号 16 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 uint16 值
     * 
     * @module nvs
     * @function readUint16
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值（0 ~ 65535），不存在时返回 null
     */
    NVS_INT_JS_GETTER(Uint16, uint16_t, Uint32)

    /**
     * 从 NVS 读取无符号 32 位整数值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的 uint32 值
     * 
     * @module nvs
     * @function readUint32
     * @param key:string 要读取的键名
     * @return number|null 读取到的整数值，不存在时返回 null
     */
    NVS_INT_JS_GETTER(Uint32, uint32_t, Uint32)

    /**
     * 向 NVS 写入有符号 8 位整数值
     * 
     * 将 int8 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeInt8
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值（-128 ~ 127）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Int8, int8_t, Int32, int32_t)

    /**
     * 向 NVS 写入有符号 16 位整数值
     * 
     * 将 int16 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeInt16
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值（-32768 ~ 32767）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Int16, int16_t, Int32, int32_t)

    /**
     * 向 NVS 写入有符号 32 位整数值
     * 
     * 将 int32 值写入默认命名空间 "beshell" 中指定的 key。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 写入计数器
     * nvs.writeInt32("bootCount", 1)
     * 
     * // 读取并递增
     * let count = nvs.readInt32("bootCount") || 0
     * count++
     * nvs.writeInt32("bootCount", count)
     * console.log("Boot count:", count)
     * 
     * // 写入时间戳
     * nvs.writeInt32("lastUpdate", Date.now())
     * 
     * // 写入负数
     * nvs.writeInt32("temperature", -5)
     * console.log("Temperature:", nvs.readInt32("temperature"))
     * ```
     *
     * @module nvs
     * @function writeInt32
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Int32, int32_t, Int32, int32_t)

    /**
     * 向 NVS 写入有符号 64 位整数值
     * 
     * 将 int64 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeInt64
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Int64, int64_t, Int64, int64_t)

    /**
     * 向 NVS 写入无符号 8 位整数值
     * 
     * 将 uint8 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeUint8
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值（0 ~ 255）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Uint8, uint8_t, Uint32, uint32_t)

    /**
     * 向 NVS 写入无符号 16 位整数值
     * 
     * 将 uint16 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeUint16
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值（0 ~ 65535）
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Uint16, uint16_t, Uint32, uint32_t)

    /**
     * 向 NVS 写入无符号 32 位整数值
     * 
     * 将 uint32 值写入默认命名空间 "beshell" 中指定的 key
     * 
     * @module nvs
     * @function writeUint32
     * @param key:string 要写入的键名
     * @param value:number 要写入的整数值
     * @return bool 写入成功返回 true，失败返回 false
     * @throws value 不是数字
     */
    NVS_INT_JS_SETTER(Uint32, uint32_t, Uint32, uint32_t)

    /**
     * 删除 NVS 中指定的键值对
     * 
     * 从默认命名空间 "beshell" 中删除指定 key 的数据。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 删除一个键
     * const deleted = nvs.erase("myKey")
     * console.log("Deleted:", deleted)
     * 
     * // 删除后再读取会返回 null
     * const value = nvs.readString("myKey")
     * console.log(value)  // null
     * ```
     *
     * @module nvs
     * @function erase
     * @param key:string 要删除的键名
     * @return bool 删除成功返回 true，失败返回 false
     */
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
#else
        return JS_FALSE ;
#endif

    }

    /**
     * 从 NVS 读取字符串值
     * 
     * 从默认命名空间 "beshell" 中读取指定 key 的字符串值。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 写入字符串
     * nvs.writeString("username", "admin")
     * 
     * // 读取字符串
     * const username = nvs.readString("username")
     * console.log("Username:", username)
     * 
     * // 读取不存在的键返回 null
     * const notExist = nvs.readString("nonexistent")
     * console.log(notExist)  // null
     * 
     * // 存储 JSON 数据
     * const config = { wifi: { ssid: "MyWiFi", password: "secret" } }
     * nvs.writeString("config", JSON.stringify(config))
     * 
     * // 读取并解析 JSON
     * const saved = nvs.readString("config")
     * const parsed = JSON.parse(saved)
     * console.log(parsed.wifi.ssid)
     * ```
     *
     * @module nvs
     * @function readString
     * @param key:string 要读取的键名
     * @param buff_size:number=128 缓冲区大小
     * @return string|null 读取到的字符串值，不存在时返回 null
     * @throws buff_size 必须大于 0
     * @throws NVS 打开失败
     * @throws 内存分配失败
     */
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

    /**
     * 向 NVS 写入字符串值
     * 
     * 将字符串值写入默认命名空间 "beshell" 中指定的 key。
     * 
     * 示例：
     * ```javascript
     * import * as nvs from "nvs"
     * 
     * // 写入简单字符串
     * nvs.writeString("deviceName", "ESP32-C3")
     * 
     * // 写入 JSON 字符串
     * const config = { brightness: 80, volume: 50 }
     * nvs.writeString("settings", JSON.stringify(config))
     * 
     * // 写入后再读取
     * const name = nvs.readString("deviceName")
     * console.log("Device:", name)
     * ```
     *
     * @module nvs
     * @function writeString
     * @param key:string 要写入的键名
     * @param value:string 要写入的字符串值
     * @return bool 写入成功返回 true，失败返回 false
     */
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

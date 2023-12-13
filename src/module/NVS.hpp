#pragma once

#include <stdint.h>
#include "NativeModule.hpp"

namespace be {
    class NVS: public NativeModule {
    private:
    public:
        NVS() ;
        void readOneTime(const char * key, uint8_t * value) const ;
        
        static bool readInt8(const char * key, int8_t & value, const char * ns="beshell") ;
        static bool readInt16(const char * key, int16_t & value, const char * ns="beshell") ;
        static bool readInt32(const char * key, int32_t & value, const char * ns="beshell") ;
        static bool readInt64(const char * key, int64_t & value, const char * ns="beshell") ;
        static bool readUint8(const char * key, uint8_t & value, const char * ns="beshell") ;
        static bool readUint16(const char * key, uint16_t & value, const char * ns="beshell") ;
        static bool readUint32(const char * key, uint32_t & value, const char * ns="beshell") ;
        
        static bool writeInt8(const char * key, int8_t value, const char * ns="beshell") ;
        static bool writeInt16(const char * key, int16_t value, const char * ns="beshell") ;
        static bool writeInt32(const char * key, int32_t value, const char * ns="beshell") ;
        static bool writeInt64(const char * key, int64_t value, const char * ns="beshell") ;
        static bool writeUint8(const char * key, uint8_t value, const char * ns="beshell") ;
        static bool writeUint16(const char * key, uint16_t value, const char * ns="beshell") ;
        static bool writeUint32(const char * key, uint32_t value, const char * ns="beshell") ;

        
        static bool readFloat(const char * key, float & value, const char * ns="beshell") ;
        static bool writeFloat(const char * key, float value, const char * ns="beshell") ;
        
        static bool readDouble(const char * key, double & value, const char * ns="beshell") ;
        static bool writeDouble(const char * key, double value, const char * ns="beshell") ;

        static JSValue jsErase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue jsReadInt8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadInt16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadInt32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadInt64(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadUint16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadUint32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue jsWriteInt8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteInt16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteInt32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteInt64(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteUint16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteUint32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
    } ;
}
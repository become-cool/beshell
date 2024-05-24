#pragma once

#include <stdint.h>
#include "NativeModule.hpp"

namespace be {

    class BeShell ;

    class NVS: public NativeModule {
    private:
    protected:
    public:
        static const char * name ;
        static void use(BeShell * beshell) ;

        NVS(JSContext * ctx, const char * name,uint8_t flagGlobal=0);

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

        static JSValue erase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue readInt8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readInt16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readInt32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readInt64(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readUint16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readUint32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue writeInt8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeInt16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeInt32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeInt64(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeUint16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeUint32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
    } ;
}
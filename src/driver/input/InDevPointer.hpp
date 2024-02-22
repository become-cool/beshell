#pragma once

#include <NativeClass.hpp>

namespace be::driver {
    class InDevPointer: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        InDevPointer(JSContext * ctx) ;
        
        virtual bool readPos(uint8_t i, uint16_t &x, uint16_t &y) = 0 ;
        virtual uint8_t readPointCount() = 0 ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}
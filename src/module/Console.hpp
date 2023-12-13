#pragma once
#include "NativeModule.hpp"

#undef stringify

namespace be {
    class Console: public NativeModule {
    private:
        JSValue jsStringify ;
    public:
        Console() ;
        ~Console() ;
        void setup(JSContext *ctx) ;
        std::string stringify(JSContext *ctx, JSValue val) ;

        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
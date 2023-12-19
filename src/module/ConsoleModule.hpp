#pragma once
#include "NativeModule.hpp"

#undef stringify

namespace be {
    class ConsoleModule: public NativeModule {
    private:
        JSValue jsStringify ;
    protected:
        void defineExports() ;
    public:
        ConsoleModule() ;
        ~ConsoleModule() ;
        void setup(JSContext *ctx) ;
        std::string stringify(JSContext *ctx, JSValue val) ;

        static JSValue jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
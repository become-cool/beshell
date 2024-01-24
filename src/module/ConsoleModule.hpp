#pragma once
#include "NativeModule.hpp"

#undef stringify

namespace be {
    class ConsoleModule: public NativeModule {
    private:
        JSValue jsStringify ;
    protected:
    public:
        static NativeModule* factory(JSContext * ctx, const char * name) ;

        ConsoleModule(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;
        ~ConsoleModule() ;
        
        std::string stringify(JSContext *ctx, JSValue val) ;

        static JSValue jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        void setup(JSContext * ctx) ;
    } ;
}
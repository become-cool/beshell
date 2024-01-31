#pragma once

#include "NativeObject.hpp"

namespace be {

    class Console: public NativeObject {

    public:
        Console(JSContext * ctx) ;


        std::string stringify(JSContext * ctx, JSValue val) ;

        static JSValue jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSClassID classID ;
        inline static NativeClass* defineClass(JSContext * ctx) ;

    private:
        static std::map<JSContext*, NativeClass*> mapCtxClasses ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        JSValue jsStringify = JS_UNDEFINED ;
    } ;

}
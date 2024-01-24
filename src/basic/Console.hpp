#pragma once

#include "NativeObject.hpp"

namespace be {

    class Console: public NativeObject {

    protected:
        void constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst * argv) ;

    public:
        Console(JSContext * ctx) ;


        std::string stringify(JSContext * ctx, JSValue val) ;

        static JSValue jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSClassID classID ;
    
    private:
        JSValue jsStringify = JS_UNDEFINED ;
    } ;

}
#pragma once

#include "deps/quickjs/quickjs-libc.h"

namespace be {
    class NativeClass {
    private:
    protected:
        JSValue constructor ;
        JSValue proto ;
        JSValue parentProto ;

        static JSCFunctionListEntry * methodList ;
    public:
        NativeClass() ;

        static JSValue defineClass(JSContext * ctx, JSClassID classId, JSClassDef classDef) ;
    } ;
}
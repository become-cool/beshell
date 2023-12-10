#pragma once
#include "NativeModule.hpp"

namespace be {
    class FSModule: public NativeModule {
    private:
    public:
        FSModule() ;
        
        static JSValue mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
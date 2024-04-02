#pragma once
#include "NativeModule.hpp"

namespace be {
    class FSModule: public NativeModule {
    private:
    protected:
    public:
        FSModule(JSContext * ctx, const char * name,uint8_t flagGlobal=1);
        
        static JSValue mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue statSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue existsSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
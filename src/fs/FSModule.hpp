#pragma once
#include "NativeModule.hpp"

namespace be {
    class FSModule: public NativeModule {
    private:
    protected:
    public:
        FSModule(JSContext * ctx, const char * name,uint8_t flagGlobal);
        static NativeModule* factory(JSContext * ctx, const char * name) ;
        
        static JSValue jsMkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsUnlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsWriteFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsListDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRenameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsStatSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsExistsSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
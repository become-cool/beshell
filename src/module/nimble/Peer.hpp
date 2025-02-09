#pragma once

#include "sdkconfig.h"
#if CONFIG_BT_NIMBLE_ENABLED

#include <NativeClass.hpp>
#include "host/ble_gap.h"

namespace be::nimble {
    class Peer: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        void * ble_peer ;

    public:
        Peer(JSContext * ctx, void *) ;

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}

#endif

#pragma once

#include "sdkconfig.h"
#include "soc/soc_caps.h"

#if SOC_USB_SERIAL_JTAG_SUPPORTED

#include "../../NativeModule.hpp"

namespace be {
    class CDC: public be::EventModule {
    public:
        static char const * const name ;
        CDC(JSContext * ctx, const char * name) ;

        void exports(JSContext *ctx) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        virtual void onNativeEvent(JSContext *ctx, void * param) ;
    
    private :
        static void taskListen(CDC* cdc) ;
        static bool setuped ;
    } ;
}
#endif

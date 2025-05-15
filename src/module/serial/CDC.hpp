#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

#if CONFIG_USB_OTG_SUPPORTED
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
        static void CDC::taskListen(CDC* cdc) ;
    } ;
}
#endif

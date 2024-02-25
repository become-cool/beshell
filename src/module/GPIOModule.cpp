#include "GPIOModule.hpp"

using namespace std ;

namespace be {

    GPIOModule::GPIOModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("mode",mode,0) ;
        exportFunction("setMode",setMode,0) ;
        exportFunction("write",write,0) ;
        exportFunction("read",read,0) ;
        exportFunction("readAnalog",readAnalog,0) ;
        exportFunction("writeAnalog",writeAnalog,0) ;
        exportFunction("writePWM",writePWM,0) ;
        exportFunction("readPWM",readPWM,0) ;
        exportFunction("watch",watch,0) ;
        exportFunction("unwatch",unwatch,0) ;
    }

    JSValue GPIOModule::mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_UINT8(1, value)
        gpio_set_level(pin, value);
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::readAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::writeAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::writePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::readPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::watch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::unwatch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}


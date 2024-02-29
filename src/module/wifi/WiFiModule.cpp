#include "WiFiModule.hpp"

namespace be {
    
    WiFiModule::WiFiModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("start",start,0) ;
        exportFunction("stop",stop,0) ;

        exportFunction("setPS",setPS,0) ;
        exportFunction("setMode",setMode,0) ;
        exportFunction("mode",mode,0) ;
        exportFunction("setAPConfig",setAPConfig,0) ;
        exportFunction("setStaConfig",setStaConfig,0) ;
        exportFunction("config",config,0) ;
        exportFunction("connect",connect,0) ;
        exportFunction("disconnect",disconnect,0) ;
        exportFunction("getIpInfo",getIpInfo,0) ;
        exportFunction("setHostname",setHostname,0) ;
        exportFunction("allSta",allSta,0) ;
        exportFunction("registerEventHandle",registerEventHandle,0) ;
        exportFunction("scanStart",scanStart,0) ;
        exportFunction("scanStop",scanStop,0) ;
        exportFunction("isScanning",isScanning,0) ;
        exportFunction("scanRecords",scanRecords,0) ;
        exportFunction("staStarted",staStarted,0) ;
        exportFunction("apStarted",apStarted,0) ;
    }

    // void WiFiModule::import(JSContext *ctx) {
    // }

    JSValue WiFiModule::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setPS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setAPConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setStaConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::getIpInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setHostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::allSta(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::registerEventHandle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

    JSValue WiFiModule::scanStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::scanStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::scanRecords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

    JSValue WiFiModule::staStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::staConnected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::apStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

}

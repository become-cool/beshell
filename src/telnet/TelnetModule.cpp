#include "TelnetModule.hpp"
#include "TelnetChannelNClass.hpp"
#include "../js/telnet.c"
#include "qjs_utils.h"

namespace be{

    char const * const TelnetModule::name = "telnet" ;

    std::vector<NativeModuleExportorFunc> TelnetModule::exportors ;

    TelnetModule::TelnetModule(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportClass<TelnetChannelNClass>() ;
        exportName("ws") ;
        exportName("ble") ;
        
        EXPORT_FUNCTION(enableCrypto) ;
        EXPORT_FUNCTION(disableCrypto) ;
        EXPORT_FUNCTION(setCryptoKey) ;
    }

    void TelnetModule::exports(JSContext *ctx) {
        for(auto func : exportors) {
            func(ctx, this) ;
        }

        JSEngineEvalEmbeded(ctx, telnet)
    }

    JSValue TelnetModule::enableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->telnet->enableCrypto = true ;
        return JS_UNDEFINED ;
    }
    JSValue TelnetModule::disableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->telnet->enableCrypto = false ;
        return JS_UNDEFINED ;
    }
    JSValue TelnetModule::setCryptoKey(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        if(!JS_IsArrayBuffer(argv[0])){
            JSTHROW("crypto key must be array buffer") ;
        }
        if(!JS_IsArrayBuffer(argv[1])){
            JSTHROW("crypto vi must be array buffer") ;
        }

        size_t key_len, vi_len ;
        uint8_t * key = JS_GetArrayBuffer(ctx,&key_len, argv[0]) ;
        uint8_t * vi = JS_GetArrayBuffer(ctx,&vi_len, argv[1]) ;

        if(key_len!=16) {
            JSTHROW("crypto key length must be 16") ;
        }
        if(vi_len!=16) {
            JSTHROW("crypto vi length must be 16") ;
        }

        auto telnet = JSEngine::fromJSContext(ctx)->beshell->telnet ;
        memcpy( telnet->cryptoKey, key, 16) ;
        memcpy( telnet->cryptoVI, vi, 16) ;

        return JS_UNDEFINED ;
    }
}
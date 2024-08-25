#include "TelnetModule.hpp"

namespace be{

    const char * const TelnetModule::name = "telnet" ;

    std::vector<NativeModuleExportorFunc> TelnetModule::exportors ;

    TelnetModule::TelnetModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportName("bt") ;
    }
    
    static JSValue btSetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        TelnetBLE * channel = (TelnetBLE*) JSEngine::fromJSContext(ctx)->beshell->telnet->channel("bt") ;
        if( !channel ){
            JSTHROW("call BeShell::telnet->useBLE() first")
        }

        std::string ARGV_TO_STRING_OPT(0, name, "BeShell")
        ARGV_TO_UINT16_OPT(1, charId, 0x0512)
        ARGV_TO_UINT16_OPT(2, serverId, 0x0b0c)

        channel->setup(name.c_str(),charId,serverId) ;
        
        return JS_UNDEFINED ;
    }

    static JSValue btSend(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        TelnetChannel * channel = JSEngine::fromJSContext(ctx)->beshell->telnet->channel("bt") ;
        if( !channel ){
            JSTHROW("call BeShell::telnet->useBLE() first")
        }

        CHECK_ARGC(1)

        bool needfree = false ;
        size_t length = 0 ;
        uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[0]) ;

        // ArrayBuffer
        if(buff) {
            channel->sendData((char *)buff, length) ;
            return JS_UNDEFINED ;
        }

        // array 
        else if(JS_IsArray(ctx, argv[0])) {
            buff = JS_ArrayToBufferUint8(ctx, argv[0], (int *)&length) ;
            if(length && buff) {
                channel->sendData((char *)buff, length) ;
                free(buff) ;
            } 
            return JS_UNDEFINED ;
        }

        // string
        else {
            ARGV_TO_CSTRING_LEN(0, buff, length)
            channel->sendData((char *)buff, length) ;
            JS_FreeCString(ctx, (const char *)buff) ;
            return JS_UNDEFINED ;
        }
    }

    void TelnetModule::import(JSContext *ctx) {
        for(auto func : exportors) {
            func(ctx, this) ;
        }
    }
        
    void TelnetModule::exportBT(JSContext *ctx, NativeModule * module) {
        JSValue btapi = JS_NewObject(ctx);
        JS_SetPropertyStr(ctx, btapi, "setup", JS_NewCFunction(ctx, btSetup, "setup", 0));
        JS_SetPropertyStr(ctx, btapi, "send", JS_NewCFunction(ctx, btSend, "send", 0));
        module->exportValue("bt", btapi) ;
    }
}
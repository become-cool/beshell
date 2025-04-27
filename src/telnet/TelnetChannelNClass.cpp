#include "TelnetChannelNClass.hpp"
#include "TelnetChannel.hpp"
#include "BeShell.hpp"
#include <cstdint>

using namespace std ;

namespace be {

    DEFINE_NCLASS_META_NAME(TelnetChannelNClass, EventEmitter, "TelnetChannel")
    std::vector<JSCFunctionListEntry> TelnetChannelNClass::methods = {
        JS_CFUNC_DEF("process", 0, TelnetChannelNClass::process),
    } ;

    TelnetChannelNClass::TelnetChannelNClass(JSContext * ctx, JSValue _jsobj)
        : EventEmitter(ctx,build(ctx,_jsobj))
        , TelnetChannel(JSEngine::fromJSContext(ctx)->beshell->telnet)
        , parser([this](std::unique_ptr<Package> pkg, void * opaque) {
            assert(telnet) ;
            telnet->onReceived(this, std::move(pkg)) ;
        })
    {
        telnet->addChannel((TelnetChannel*)this) ;
    }

    TelnetChannelNClass::~TelnetChannelNClass(){
        telnet->removeChannel((TelnetChannel*)this) ;
    }

    JSValue TelnetChannelNClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new TelnetChannelNClass(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue TelnetChannelNClass::process(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(TelnetChannelNClass, self)
        ARGV_TO_UINT8_OPT(1, cmd, Cmd::CALL)
        ARGV_TO_UINT8_OPT(2, pkgid, -1)

        if(JS_IsString(argv[0])) {
            size_t len = 0 ;
            const char * cstr = JS_ToCStringLen(ctx, &len, argv[0]) ;
            if(cstr == nullptr || len == 0) {
                JS_FreeCString(ctx, cstr) ;
                return JS_UNDEFINED ;
            }
            self->parser.parse((uint8_t *)cstr, len) ;
            JS_FreeCString(ctx, cstr) ;
        }
        else if(JS_IsArrayBuffer(argv[0])) {
            size_t len = 0 ;
            uint8_t * data = (uint8_t *)JS_GetArrayBuffer(ctx, &len, argv[0]) ;
            if(data == nullptr || len == 0) {
                return JS_UNDEFINED ;
            }
            self->parser.parse(data, len) ;
        }
        else {
            JSTHROW("arg must be string or ArrayBuffer") ;
        }

        return JS_UNDEFINED ;
    }

    
    void TelnetChannelNClass::sendData (const char * data, size_t datalen) {
        Console::setChannel("serial") ;
        emitSyncFree("output-stream", {
            JS_NewArrayBufferCopy(ctx, (const uint8_t *)data, datalen)
        }) ;
        Console::setChannel(nullptr) ;
    }

    void TelnetChannelNClass::send (Package & pkg) {
        
        size_t len = 0 ;
        uint8_t * data = pkg.toStream(&len) ;

        Console::setChannel("serial") ;
        emitSyncFree("output-stream", {
            JS_NewArrayBuffer(ctx, data, len, freeArrayBuffer, NULL, false)
        }) ;
        Console::setChannel(nullptr) ;
    }
}
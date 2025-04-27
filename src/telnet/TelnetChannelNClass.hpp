#pragma once

#include <EventEmitter.hpp>
#include "TelnetChannel.hpp"

namespace be {
    class TelnetChannelNClass: public be::EventEmitter, public TelnetChannel {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        Parser parser ;
    public:
        TelnetChannelNClass(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        ~TelnetChannelNClass() ;
        
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue process(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        virtual void sendData (const char * data, size_t datalen) ;
        virtual void send (Package & pkg) ;
    } ;
}
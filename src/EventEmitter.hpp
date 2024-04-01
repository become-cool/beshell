#pragma once

#include <NativeClass.hpp>
#include <initializer_list>

namespace be {

    class EventEmitter: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        EventEmitter(JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        virtual void eventAdded(const char * eventName) ;
        virtual void eventRemoved(const char * eventName) ;

        void emitSync(const JSValue & eventName, std::initializer_list<JSValue> args) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue eventAdded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue eventRemoved(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
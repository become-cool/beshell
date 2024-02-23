#pragma once

#include <NativeClass.hpp>
#include <initializer_list>

namespace be {

    class EventEmitter: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        EventEmitter(JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        void emit(const JSValue & eventName, std::initializer_list<JSValue> args) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}
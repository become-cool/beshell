#pragma once

#include <NativeClass.hpp>
#include "JSEngine.hpp"
// #include <initializer_list>

#ifdef ESP_PLATFORM
#include "freertos/queue.h"
#endif

namespace be {

    class EventEmitter: public NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    protected: 
        virtual void eventAdded(const char * eventName) ;
        virtual void eventRemoved(const char * eventName) ;

#ifdef ESP_PLATFORM
        QueueHandle_t nevent_queue = nullptr;
        void * native_param = nullptr ;
        void enableNativeEvent(JSContext *ctx, size_t param_size, size_t queue_size=5) ;
        static void nativeEventLoop(JSContext * ctx, void * opaque) ;
        virtual void onNativeEvent(JSContext *ctx, void * param) ;
#endif

    public:
        EventEmitter(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        ~EventEmitter() ;
        
        void emitSync(const char * eventName, std::initializer_list<JSValue> args) ;
        void emitSync(const JSValue & eventName, std::initializer_list<JSValue> args) ;

#ifdef ESP_PLATFORM
        void emitNativeEvent(void * param) ;
#endif

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue eventAdded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue eventRemoved(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
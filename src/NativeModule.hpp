#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include "NativeClass.hpp"
#include "debug.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>


#define EXPORT_FUNCTION(funcname)   exportFunction(#funcname, funcname, 0) ;

namespace be {

	typedef void (*NativeClassFactoryFunc)(JSContext * ctx, JSModuleDef * m) ;

    class BeShell ;
    class NativeModule {
    private:
        std::vector<JSCFunctionListEntry> exportFuncs ;
        std::vector<NativeClassFactoryFunc> classFactories ;

        static int importModule(JSContext *ctx, JSModuleDef *m) ;

        template <typename C>
        static void classFactory(JSContext * ctx, JSModuleDef * m) {
            JSValue jscotr = C::defineClass(ctx) ;
            JS_SetModuleExport(ctx, m, C::className, jscotr) ;
        }

    protected:
        JSContext * ctx = nullptr ;
        JSModuleDef * m = nullptr ;
        virtual void exports(JSContext *ctx) ;

    public:
        static const char * name ;

        uint8_t flagGlobal = 0 ;
        
        NativeModule(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;
        virtual ~NativeModule() ;

        static void use(BeShell * beshell) ;

        void exportName(const char * name) ;

        void exportValue(const char * name, JSValue value) ;

        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;

        template <typename C>
        void exportClass() {
            static_assert(std::is_base_of<be::NativeClass, C>::value, "C must be a subclass of NativeClass") ;
            assert(ctx) ;
            assert(m) ;
            JS_AddModuleExport(ctx, m, C::className) ;
            classFactories.push_back(classFactory<C>) ;
        }

        inline static NativeModule * fromJSModuleDef(JSModuleDef *) ;
        
        friend class ModuleLoader ;
    } ;

    class EventModule : public NativeModule {
    
    public:
        EventModule(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;
        ~EventModule() ;

#ifdef ESP_PLATFORM
        void enableNativeEvent(JSContext *ctx, size_t param_size, size_t queue_size=5) ;
        static void nativeEventLoop(JSContext * ctx, EventModule * opaque) ;
        virtual void onNativeEvent(JSContext *ctx, void * param) ;
        void emitNativeEvent(void * param) ;
#endif

    void emitSync(const char * eventName, std::initializer_list<JSValue> args) ;
    void emitSync(const JSValue & eventName, std::initializer_list<JSValue> args) ;

    protected:
        void exports(JSContext *ctx) ;

    private:
    
#ifdef ESP_PLATFORM
        void * nevent_queue = nullptr;
        void * native_param = nullptr ;
#endif
    } ;       
}

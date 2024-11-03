#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include "telnet/Telnet.hpp"
#include "JSTimer.hpp"
#include "ModuleLoader.hpp"
#include "basic/Console.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace be {
    class BeShell ;

	typedef void (*EngineLoopFunction)(JSContext *, void *) ;
    struct LoopingFunction {
        EngineLoopFunction func ;
        void * opaque ;
    } ;

    class ILoopable {
    public:
        virtual void loop(JSContext *) = 0 ;
    } ;

    struct Looping {
        // 1: function (LoopingFunction), 2: object (ILoopable)
        uint8_t type ;
        union {
            struct {
                EngineLoopFunction function ;
                void * opaque ;
            } func ;
            ILoopable * obj ;
        } exec ;
        uint32_t id ;
        uint8_t priority ;
        uint8_t priorityCount ;
    } ;

    struct LoopingOp {
        int op ;                // 1: add, -1: remove
        union {
            struct {
                bool ignoreRepeat ;
                struct Looping looping ;
            } add ;
            uint16_t remove ;
        } target ;
    } ;

    class JSEngine {
    private:

        Console * console = nullptr ;

        SemaphoreHandle_t xSemaphore;
        bool inLooping = false ;

        uint16_t loopingAssignedId = 0 ;
        std::vector<struct Looping> lstLoopings ; 
        std::vector<LoopingOp> waitingLoopingOps ;

        static JSContext * SetupContext(JSRuntime *rt) ;

    public:
        BeShell * beshell ;
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;

        JSTimer timer ;
        ModuleLoader mloader ;
        
        JSEngine(BeShell *) ;

        void setup() ;
        void loop() ;

        void print(JSValue content, int pkgId=-1, uint8_t cmd=OUTPUT, TelnetChannel * ch=nullptr) ;
        void dumpError(int pkgId=-1, TelnetChannel * ch=nullptr) ;
        static std::string getExceptionStr(JSContext * ctx, JSValue exception) ;

        static void setGlobalValue(JSContext * ctx, const char * name, JSValue) ;
        static JSValue getGlobalValue(const char * name) ;

        JSValue eval(const char * code, int code_len=-1, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
        JSValue evalScript(const char * filepath, int flags=JS_EVAL_TYPE_MODULE, bool dumpException=true) ;
        void runScript(const char * filepath, int flags=JS_EVAL_TYPE_MODULE) ;

        static JSEngine * fromJSContext(JSContext *) ;
        static JSEngine * fromJSRuntime(JSRuntime *) ;
        
        int32_t addLoopFunction(EngineLoopFunction func, void * opaque=nullptr, bool ignoreRepeat=true, uint8_t priority=0) ;
        int32_t addLoopObject(ILoopable* obj, bool ignoreRepeat=true, uint8_t priority=0) ;

        int32_t findLooping(EngineLoopFunction func, void * opaque) ;
        int32_t findLooping(EngineLoopFunction func) ;
        int32_t findLooping(ILoopable* obj) ;
        
        void removeLooping(int32_t id) ;
        void removeLooping(EngineLoopFunction func, void * opaque) ;
        void removeLooping(EngineLoopFunction func) ;
        void removeLooping(ILoopable* obj) ;

        bool take(int timeout=portMAX_DELAY) ;
        void give() ;
    } ;
}

#define JSEngineEvalEmbeded(ctx, filename) \
    JSEval(ctx, (const char *)embeded_js_src_##filename, -1, "embeded://"#filename ".js", JS_EVAL_TYPE_MODULE);

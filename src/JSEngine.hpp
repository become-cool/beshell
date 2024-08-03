#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include "telnet/Telnet.hpp"
#include "JSTimer.hpp"
#include "ModuleLoader.hpp"
#include "basic/Console.hpp"


namespace be {
    class BeShell ;
	typedef void (*EngineLoopFunction)(JSContext *, void *) ;

    class ILoopable {
    public:
        virtual void loop(JSContext *) = 0 ;
    } ;

    struct ILoopableOp {
        int op ;  // 1: add, -1: remove
        ILoopable * obj ;
        bool ignoreRepeat ;
    } ;

    class JSEngine {
    private:

        Console * console = nullptr ;

        
        bool looping = false ;

        std::vector<std::pair<EngineLoopFunction,void *>> loopFunctions ; 

        std::vector<ILoopable*> loopables ;
        std::vector<ILoopableOp> queueLoopableOps ;

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
        
        void addLoopFunction(EngineLoopFunction func, void * opaque=nullptr, bool ignoreRepeat=true) ;
        void addLoopObject(ILoopable* obj, bool ignoreRepeat=true) ;
        void removeLoopObject(ILoopable* obj) ;
    } ;
}

#define JSEngineEvalEmbeded(ctx, filename) \
    JSEval(ctx, (const char *)embeded_js_src_##filename, -1, "embeded://"#filename ".js", JS_EVAL_TYPE_MODULE);

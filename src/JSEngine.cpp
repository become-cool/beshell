#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "qjs_utils.h"
#include <sys/stat.h>
#include <string.h>
#include "debug.h"
#include "js/json.c"
#include "ModuleLoader.hpp"
#include "EventEmitter.hpp"
#include "module/ProcessModule.hpp"
#include <cassert>
#include <iostream>
#include <iomanip>

#ifdef ESP_PLATFORM
#include "malloc_funcs.h"
#endif

using namespace std ;


namespace be {

    static JSValue js_ArrayBuffer_asString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, this_val) ;
        if(!buff || !size) {
            return JS_NewStringLen(ctx, NULL, 0) ;
        }
        return JS_NewStringLen(ctx, buff, size) ;
    }
    static JSValue js_ArrayBuffer_toArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, this_val) ;
        JSValue array = JS_NewArray(ctx) ;
        if(buff) {
            for(int i=0;i<(int)size;i++) {
                JS_SetPropertyUint32(ctx, array, i, JS_NewUint32(ctx, buff[i])) ;
            }
        }
        return array ;
    }


    JSEngine::JSEngine(BeShell * beshell)
        : beshell(beshell)
        , mloader(beshell)
    {}

    void JSEngine::setup() {
        if(rt!=NULL) {
            return ;
        }
        
#ifdef ESP_PLATFORM
        // // esp32 平台优先使用 PSRAM内存
        // if( getPsramTotal()>1024 ) {
        //     static const JSMallocFunctions def_malloc_funcs = {
        //         js_def_malloc,
        //         js_def_free,
        //     js_def_realloc,
        //         malloc_usable_size,
        //     };
        //     rt = JS_NewRuntime2(&def_malloc_funcs, NULL);
        // }
        // else {
        //     rt = JS_NewRuntime();
        // }
        
        rt = JS_NewRuntime();
#else
        rt = JS_NewRuntime();
#endif

        // js_std_set_worker_new_context_func(SetupContext);
        // js_std_init_handlers(rt);
        
        JS_SetRuntimeOpaque2(rt, this) ;

        // mloader.init(rt) ;
        
        ctx = SetupContext(rt);
    }

    JSContext * JSEngine::SetupContext(JSRuntime *rt) {

        JSContext *ctx;
        ctx = JS_NewContextRaw(rt);
        if (!ctx) {
            return NULL;
        }

        JS_AddIntrinsicBaseObjects(ctx);
        JS_AddIntrinsicDate(ctx);
        JS_AddIntrinsicEval(ctx);
        JS_AddIntrinsicStringNormalize(ctx);
        JS_AddIntrinsicRegExp(ctx);
        JS_AddIntrinsicJSON(ctx);
        JS_AddIntrinsicProxy(ctx);
        JS_AddIntrinsicMapSet(ctx);
        JS_AddIntrinsicTypedArrays(ctx);
        JS_AddIntrinsicPromise(ctx);
#ifdef CONFIG_BIGNUM
        JS_AddIntrinsicBigInt(ctx);
#endif

        // global 对象
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, "global", global);
        
        JSValue ArrayBufferProto = js_get_prop(ctx, global, 2, "ArrayBuffer", "prototype") ;
        JS_SetPropertyStr(ctx, ArrayBufferProto, "asString", JS_NewCFunction(ctx, js_ArrayBuffer_asString, "asString", 0));
        JS_SetPropertyStr(ctx, ArrayBufferProto, "toArray", JS_NewCFunction(ctx, js_ArrayBuffer_toArray, "toArray", 0));
	    JS_FreeValue(ctx, ArrayBufferProto);

        JSEngine * engine = JSEngine::fromJSRuntime(rt) ;
        assert(engine) ;

        engine->timer.setup(ctx) ;
        engine->mloader.setup(ctx) ;

        engine->console = new Console(ctx) ;
        setGlobalValue(ctx, "console", engine->console->jsobj) ;

        JSEngineEvalEmbeded(ctx, json)
        
        EventEmitter::defineClass(ctx) ;
        setGlobalValue(ctx, "EventEmitter", JS_DupValue(ctx,NativeClass::getClass(ctx,EventEmitter::classID)) ) ;

        return ctx;
    }

    void JSEngine::loop() {
        timer.loop(ctx) ;
        js_std_loop(ctx) ;

        for(auto pair:loopFunctions) {
            pair.first(ctx, pair.second) ;
        }

        for(auto obj:loopables) {
            obj->loop(ctx) ;
        }
    }

    JSEngine * JSEngine::fromJSContext(JSContext * ctx) {
        return (JSEngine *)JS_GetRuntimeOpaque2( JS_GetRuntime(ctx) ) ;
    }
    JSEngine * JSEngine::fromJSRuntime(JSRuntime * rt) {
        return (JSEngine *)JS_GetRuntimeOpaque2(rt) ;
    }

    void JSEngine::print(JSValue content, int pkgId, uint8_t cmd, TelnetChannel * ch) {
        assert(beshell) ;

        std::string str = console->stringify(ctx, content) ;

        if(!str.length()) {
            return ;
        }
        if(ch) {
            ch->send(str.c_str(), str.length(), pkgId, cmd) ;
        } else if(beshell->telnet) {
            beshell->telnet->output(str.c_str(), str.length(), pkgId, cmd) ;
        } else {
            cout << str ;
        }
    }

    void JSEngine::dumpError(int pkgId, TelnetChannel * ch) {
        string str = getExceptionStr() ;
        if(ch) {
            ch->send(str.c_str(), str.length(), pkgId, EXCEPTION) ;
        } else if(beshell->telnet) {
            beshell->telnet->output(str.c_str(), str.length(), pkgId, EXCEPTION) ;
        }
    }
    
    string JSEngine::getExceptionStr() {
        JSValue exception_val = JS_GetException(ctx);
        if(JS_IsNull(exception_val)) {
            return "" ;
        }

        const char * cstr = JS_ToCString(ctx, exception_val) ;
        std::string str = cstr ;
        JS_FreeCString(ctx, cstr) ;
        // std::string str = console->stringify(ctx, exception_val) ;

        bool is_error = JS_IsError(ctx, exception_val);
        if (is_error) {
            JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
            if (!JS_IsUndefined(val)) {
                cstr = JS_ToCString(ctx, val) ;
                str+= "\n";
                str+= cstr;
                JS_FreeCString(ctx,cstr) ;
            }
            JS_FreeValue(ctx, val);
        }
        JS_FreeValue(ctx, exception_val);

        return str ;
    }

    JSValue JSEngine::eval(const char * code, int code_len,const char * filename, int flags) {
        if(code_len<0) {
            code_len = strlen(code) ;
        }
        return JS_Eval(ctx, code, code_len, filename, flags) ;   // JS_EVAL_FLAG_STRIP
    }

    JSValue JSEngine::evalScript(const char * filepath, int flags) {
        assert(beshell) ;
        // if( !beshell->fs ) {
        //     JSTHROW("call useFS() first()")
        // }

        int readed ;
        unique_ptr<char> content = FS::readFileSync(filepath,&readed) ;
        if(readed<0) {
            JSTHROW("read file failed")
        }

        string code(content.get(), readed) ;

        return JS_Eval(ctx, code.c_str(), code.length(), filepath, flags) ;
    }

    void JSEngine::setGlobalValue(JSContext * ctx, const char * name, JSValue value) {
        JSValue global = JS_GetGlobalObject(ctx) ; 
        JS_SetPropertyStr(ctx, global, name, value) ;
        JS_FreeValue(ctx,global) ;
    }

    void JSEngine::addLoopFunction(EngineLoopFunction func, void * opaque, bool ignoreRepeat) {
        if(ignoreRepeat) {
            for(auto pair:loopFunctions) {
                if(func==pair.first) {
                    return ;
                }
            }
        }
        loopFunctions.push_back( pair(func,opaque) ) ;
    }

    void JSEngine::addLoopObject(ILoopable* obj, bool ignoreRepeat) {
        if(ignoreRepeat) {
            auto it = std::find(loopables.begin(), loopables.end(), obj);
            if(it != loopables.end()) {
                return ;
            }
        }
        loopables.push_back(obj) ;
    }

    
    void JSEngine::removeLoopObject(ILoopable* obj) {
        auto it = std::find(loopables.begin(), loopables.end(), obj);
        if(it != loopables.end()) {
            loopables.erase(it) ;
        }
    }

}
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "utils.h"
#include <sys/stat.h>
#include <string.h>
#include "debug.h"
#include "module/ModuleLoader.hpp"
#include <cassert>

#ifdef PLATFORM_ESP32
#include "malloc_funcs.h"
#endif


namespace be {
    
    // static void eval_rc_script(JSContext *ctx, const char * path) {
        
    //     const char * fullpath = path ;
    //     // char * fullpath = vfspath_to_fs(path) ;

    // #ifdef PLATFORM_ESP32
    //     char * binpath = mallocf("%s.bin", fullpath) ;
    //     struct stat statbuf;
    //     if(stat(binpath,&statbuf)<0) {
    //         evalScript(ctx, fullpath, false, false) ;
    //     }
    //     else {
    //         evalScript(ctx, binpath, true, false) ;
    //     }
    //     free(binpath) ;
    // #else
    //     evalScript(ctx, fullpath, false, false) ;
    // #endif

    //     // free(fullpath) ;

    // }



    JSEngine::JSEngine(Telnet * _telnet)
        : telnet(_telnet)
    {
    }

    void JSEngine::setup(BeShell * beshell) {
        if(rt!=NULL) {
            return ;
        }
        
#ifdef PLATFORM_ESP32
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

        // js_std_set_worker_new_context_func(InitContext);
        // js_std_init_handlers(rt);
        
        JS_SetRuntimeOpaque2(rt, this) ;
        ctx = InitContext(rt);
    }

    JSContext * JSEngine::InitContext(JSRuntime *rt) {

        JSContext *ctx;
        ctx = JS_NewContextRaw(rt);
        if (!ctx) {
            return NULL;
        }

        JS_AddIntrinsicBaseObjects(ctx);
        // JS_AddIntrinsicDate(ctx);
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

        // base 函数
        // eval_rc_script(ctx, "/lib/base/base.js") ;
        // eval_rc_script(ctx, "/lib/base/console.js") ;
        // eval_rc_script(ctx, "/lib/base/events.js") ;
        // eval_rc_script(ctx, "/lib/base/require.js") ;


        JSEngine * engine = JSEngine::fromJSRuntime(rt) ;
        assert(engine) ;
        engine->mloader.setup(ctx) ;

        return ctx;
    }
    
    // void telnet_run(JSContext * ctx, uint8_t pkgid, uint8_t cmd, uint8_t * data, size_t datalen) {
    //     if(!JS_IsNull(_func_repl_input) && JS_IsFunction(ctx, _func_repl_input)) {
    //         JSValueConst * argv = malloc(sizeof(JSValue)*4) ;
    //         argv[0] = JS_NewInt32(ctx, pkgid) ;
    //         argv[1] = JS_NewInt32(ctx, 0) ;
    //         argv[2] = JS_NewInt32(ctx, cmd) ;
    //         argv[3] = JS_NewStringLen(ctx, (char *)data, datalen) ;

    //         // printf(">>> %.*s\n", datalen, data) ;

    //         JSValue ret = JS_Call(ctx, _func_repl_input, JS_NULL, 4, argv) ;
    //         if( JS_IsException(ret) ) {
    //             echo_error(ctx) ;
    //         }

    //         JS_FreeValue(ctx, ret) ;
    //         JS_FreeValue(ctx, argv[3]) ;
    //         free(argv) ;
    //     }
    //     else {
    //         printf("_func_repl_input is NULL or not Function\n") ;
    //     }
    // }

    void JSEngine::loop() {

    }

    JSEngine * JSEngine::fromJSContext(JSContext * ctx) {
        return (JSEngine *)JS_GetRuntimeOpaque( JS_GetRuntime(ctx) ) ;
    }
    JSEngine * JSEngine::fromJSRuntime(JSRuntime * rt) {
        return (JSEngine *)JS_GetRuntimeOpaque2(rt) ;
    }

    void JSEngine::print(JSValue content, int pkgId, TelnetChannel * ch) {
        assert(telnet) ;
        size_t len ;
        const char * str = JS_ToCStringLen(ctx, &len, content);
        if (len) {
            if(ch) {
                ch->send(str, len) ;
            } else {
                telnet->output(str, len) ;
            }
        }
        if(str) {
            JS_FreeCString(ctx, str) ;
        }
    }

    void JSEngine::dumpError(bool pack) {
        JSValue exception_val = JS_GetException(ctx);
        if(JS_IsNull(exception_val)) {
            return ;
        }
        bool is_error = JS_IsError(ctx, exception_val);
        print(exception_val);
        if (is_error) {
            JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
            if (!JS_IsUndefined(val)) {
                print(val);
            }
            JS_FreeValue(ctx, val);
        }
        JS_FreeValue(ctx, exception_val);
    }

    JSValue JSEngine::eval(const char * code, int code_len,const char * filename, int flags) {
        if(code_len<0) {
            code_len = strlen(code) ;
        }
        return JS_Eval(ctx, code, code_len, filename, JS_EVAL_TYPE_GLOBAL) ;   // JS_EVAL_FLAG_STRIP
    }

}
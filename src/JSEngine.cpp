#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "utils.h"
#include <sys/stat.h>
#include <string.h>
#include "runtime.h"
#include "debug.h"
#include "modules/ModuleLoader.hpp"

#ifdef PLATFORM_ESP32
#include "malloc_funcs.h"
#endif


namespace beshell {
    
    static void eval_rc_script(JSContext *ctx, const char * path) {
        
        const char * fullpath = path ;
        // char * fullpath = vfspath_to_fs(path) ;

    #ifdef PLATFORM_ESP32
        char * binpath = mallocf("%s.bin", fullpath) ;
        struct stat statbuf;
        if(stat(binpath,&statbuf)<0) {
            evalScript(ctx, fullpath, false, false) ;
        }
        else {
            evalScript(ctx, binpath, true, false) ;
        }
        free(binpath) ;
    #else
        evalScript(ctx, fullpath, false, false) ;
    #endif

        // free(fullpath) ;

    }
    static JSModuleDef * js_module_loader(JSContext *ctx, const char *module_name, void *opaque) {
        IS_NULL(opaque)
        return NULL ;
    }



    JSEngine::JSEngine(Telnet * _telnet)
        : telnet(_telnet)
    {
    }

    void JSEngine::setup(BeShell * beshell) {
        if(rt!=NULL) {
            return ;
        }
        
#ifdef PLATFORM_ESP32
        // esp32 平台优先使用 PSRAM内存
        if( getPsramTotal()>1024 ) {
            static const JSMallocFunctions def_malloc_funcs = {
                js_def_malloc,
                js_def_free,
            js_def_realloc,
                malloc_usable_size,
            };
            rt = JS_NewRuntime2(&def_malloc_funcs, NULL);
        }
        else {
            rt = JS_NewRuntime();
        }
#else
        rt = JS_NewRuntime();
#endif

        js_std_set_worker_new_context_func(InitContext);
        js_std_init_handlers(rt);
        
        JS_SetRuntimeOpaque2(rt, beshell) ;
        ctx = InitContext(rt);
    }


    JSContext * JSEngine::InitContext(JSRuntime *rt) {

        JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
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
        
        // beapi 对象
        JSValue beapi = JS_NewObject(ctx);
        JS_SetPropertyStr(ctx, global, "beapi", beapi);
        JS_FreeValue(ctx, global);
        
        // be_module_fs_require(ctx) ;
        // be_module_utils_require(ctx) ;
        // be_module_process_require(ctx) ;

        // base 函数
        eval_rc_script(ctx, "/lib/base/base.js") ;
        eval_rc_script(ctx, "/lib/base/console.js") ;
        eval_rc_script(ctx, "/lib/base/events.js") ;
        eval_rc_script(ctx, "/lib/base/require.js") ;

    // #ifdef CONFIG_BT_ENABLED
    //     be_module_bt_require(ctx) ;
    // #endif
    //     be_module_wifi_require(ctx) ;
    //     be_module_gpio_require(ctx) ;  
    //     be_module_serial_require(ctx) ;
    //     be_module_socks_require(ctx) ;
    //     be_module_driver_require(ctx) ;
    //     // be_module_cron_require(ctx) ;
    // #endif
    //     be_module_gameplayer_require(ctx) ;
    //     be_module_media_require(ctx) ;
    // #ifndef PLATFORM_WASM
    //     be_module_mg_require(ctx) ;
    // #endif
    //     be_telnet_require(ctx) ;
    //     be_module_lvgl_require(ctx) ;
    //     module_metadata_require(ctx) ;
    //     be_module_nvs_require(ctx) ;

    // #ifdef PLATFORM_LINUX
    //     be_simulate_require(ctx) ;
    // #endif

        BeShell * beshell = BeShell::fromJSRuntime(rt) ;
        assert(beshell) ;
        beshell->engine.mloader.setup(ctx) ;

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

    void JSEngine::print(JSValue content, bool pack, int pkgId) {
        assert(telnet) ;
        size_t len ;
        const char * str = JS_ToCStringLen(ctx, &len, content);
        if (len) {
            if(pack) {
                telnet->output(CMD_OUTPUT,(uint8_t*)str, len, pkgId) ;
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
        if(exception_val==JS_NULL) {
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
        return JS_Eval(ctx, code, code_len-1, filename, JS_EVAL_TYPE_GLOBAL) ;   // JS_EVAL_FLAG_STRIP
    }

}
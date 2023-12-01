#include "JSEngine.hpp"
#include "utils.h"
#include <sys/stat.h>
#include "runtime.h"

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

    static JSContext * init_custom_context(JSRuntime *rt) {
        JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
        JSContext *ctx;
        ctx = JS_NewContextRaw(rt);
        if (!ctx)
            return NULL;

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

        return ctx;
    }


    JSEngine::JSEngine() {

    }
    void JSEngine::initRuntime() {
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

        js_std_set_worker_new_context_func(init_custom_context);
        js_std_init_handlers(rt);
        ctx = init_custom_context(rt);
    }

    JSValue JSEngine::evalSync(const char * code, const char * filename, int flags) {
        return JS_UNDEFINED ;
    }

}
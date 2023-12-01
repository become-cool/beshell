#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"


namespace beshell {
    Telnet::Telnet(BeShell * _beshell)
        : beshell(_beshell)
        , onPackage([this](Package * pkg){
            
            switch (pkg->cmd)
            {
            case LINE:
            case RUN:
            case CALL:
                // beshell->eval((const char *)pkg->body) ;

                // JSValue ret = JS_Eval(beshell->ctx, (const char *)pkg->body, pkg->body_len-1, "repl", JS_EVAL_TYPE_GLOBAL) ;
                // if(JS_IsException(ret)) {
                //     echo_error(beshell->ctx) ;
                // }
                // JS_FreeValue(beshell->ctx, ret) ;

                break;
            }
        })
        , channelSeiral(onPackage)
    {}

    void Telnet::setup() {
        channelSeiral.setup() ;
    }
    void Telnet::loop() {
        channelSeiral.loop() ;
    }

    void Telnet::output(int pkgid, uint8_t cmd, uint8_t * data, size_t datalen) {
        Package pkg(pkgid,cmd,data,datalen) ;
        pkg.encodeBodyLength() ;
        pkg.verifysum = pkg.calculateVerifysum() ;

        channelSeiral.send(pkg) ;
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


    // void js_dump_err(JSContext *ctx, JSValueConst val) {
    //     size_t len ;
    //     const char * str = JS_ToCStringLen(ctx, &len, val);
    //     // printf("error:%s\n",str) ;
    //     if (len) {
    //         telnet_output(CMD_OUTPUT, mk_echo_pkgid(), str, len) ;
    //     }
    // }

    // void echo_error(JSContext * ctx) {
    //     JSValue exception_val = JS_GetException(ctx);
    //     bool is_error = JS_IsError(ctx, exception_val);
    //     js_dump_err(ctx, exception_val);
    //     if (is_error) {
    //         JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
    //         if (!JS_IsUndefined(val)) {
    //             js_dump_err(ctx, val);
    //         }
    //         JS_FreeValue(ctx, val);
    //     }
    //     JS_FreeValue(ctx, exception_val);
    // }

}
#include "simulate.h"
#include "debug.h"
#include "module_telnet.h"


unsigned char ___src_beapi_simulate_js[] ;
unsigned int ___src_beapi_simulate_js_len ;


void be_simulate_require(JSContext *ctx) {
    JSValue ret = JS_Eval(ctx, (const char *)___src_beapi_simulate_js, ___src_beapi_simulate_js_len, "beapi.simulate.js", JS_EVAL_TYPE_GLOBAL) ;
    if(JS_IsException(ret)) {
        echo_error(ctx) ;
    }
    JS_FreeValue(ctx, ret);
}

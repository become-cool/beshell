#include "module_telnet.h"
#include "telnet_stdio.h"
#include "telnet_ble.h"
#ifdef PLATFORM_WASM
#include "telnet_wasm.h"
#else
// #include "telnet_ws.h"
#include "module_fs.h"
#endif
#include "telnet_serial.h"
#include "module_utils.h"
#include "utils.h"
#include "beshell.h"
#include "js_main_loop.h"
#include <math.h>
#include <string.h>



// JSValue _func_repl_output ;
JSValue _func_repl_input ;

JSValue js_repl_register_handler(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        THROW_EXCEPTION("REPL input function must be a function type")
    }

	if(!JS_IsNull(_func_repl_input)) {
		JS_FreeValue(ctx, _func_repl_input) ;
	}

	_func_repl_input = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED ;
}

uint8_t _echo_pkgid = 0 ;
uint8_t mk_echo_pkgid() {
	return _echo_pkgid ++ ;
}


void telnet_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) {

	// for WebSocket(wifi)
#ifdef PLATFORM_WASM
	telnet_proto_send_pkg(pkgid, cmd, data, datalen) ;
#endif
	
	// for serial(usb) or stdio(simulators)
#ifdef PLATFORM_LINUX
    // telnet_ws_output(cmd, pkgid, data, datalen) ;

	printf(data) ;
	printf("\n") ;
    fflush(stdout) ;
#endif

#ifdef PLATFORM_ESP32
    telnet_ws_output(cmd, pkgid, data, datalen) ;
	telnet_proto_send_pkg(pkgid, cmd, data, datalen) ;
#endif
}

void telnet_run(JSContext * ctx, uint8_t pkgid, uint8_t cmd, uint8_t * data, size_t datalen) {
	if(!JS_IsNull(_func_repl_input) && JS_IsFunction(ctx, _func_repl_input)) {
		JSValueConst * argv = malloc(sizeof(JSValue)*4) ;
		argv[0] = JS_NewInt32(ctx, pkgid) ;
		argv[1] = JS_NewInt32(ctx, 0) ;
		argv[2] = JS_NewInt32(ctx, cmd) ;
		argv[3] = JS_NewStringLen(ctx, (char *)data, datalen) ;

		// printf(">>> %.*s\n", datalen, data) ;

		JSValue ret = JS_Call(ctx, _func_repl_input, JS_NULL, 4, argv) ;
		if( JS_IsException(ret) ) {
			echo_error(ctx) ;
		}

		JS_FreeValue(ctx, ret) ;
		JS_FreeValue(ctx, argv[3]) ;
		free(argv) ;
	}
	else {
		printf("_func_repl_input is NULL or not Function\n") ;
	}
}


void js_dump_err(JSContext *ctx, JSValueConst val) {
	size_t len ;
    const char * str = JS_ToCStringLen(ctx, &len, val);
	// printf("error:%s\n",str) ;
    if (len) {
		telnet_output(CMD_OUTPUT, mk_echo_pkgid(), str, len) ;
    }
}

void echo_error(JSContext * ctx) {
    JSValue exception_val = JS_GetException(ctx);
    bool is_error = JS_IsError(ctx, exception_val);
    js_dump_err(ctx, exception_val);
    if (is_error) {
        JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(val)) {
            js_dump_err(ctx, val);
        }
        JS_FreeValue(ctx, val);
    }
    JS_FreeValue(ctx, exception_val);
}



void telnet_send_ready() {
	char * buff = mallocf(
		"{"
			"\"firmware\":\"beshell\""
			",\"version\":\"%s\""
			",\"level\":%d"
			",\"partID\":%d"
			",\"partVersion\":%d"
		"}"
		, BESHELL_VERSION
		, task_boot_level()
		, readPartId()
		, readPartVersion()
	) ;
    telnet_output(CMD_READY, 0, buff, strlen(buff)) ;
	free(buff) ;
}



JSValue js_repl_rspn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	CHECK_ARGC(3) 
	
	ARGV_TO_UINT8(1, cmd)
	ARGV_TO_UINT8(0, pkgid)
	ARGV_TO_STRING_LEN(2, data, datalen)

	// printf("rspn(%d):%.*s\n",datalen,datalen,data) ;
    telnet_output(cmd, pkgid, data, datalen) ;

	JS_FreeCString(ctx, data) ;
	
    return JS_UNDEFINED ;
}




void be_telnet_init() {
    _func_repl_input = JS_NULL ;

#ifdef PLATFORM_WASM
	be_telnet_wasm_init() ;
#endif

	// be_telnet_ble_init() ;

#ifdef PLATFORM_LINUX
	be_telnet_stdio_init() ;
#endif

#ifdef PLATFORM_ESP32
	be_telnet_serial_init() ;
#endif
}


void be_telnet_require(JSContext *ctx) {
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue telnet = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, beapi, "telnet", telnet);
    JS_SetPropertyStr(ctx, telnet, "rspn", JS_NewCFunction(ctx, js_repl_rspn, "rspn", 1));
    JS_SetPropertyStr(ctx, telnet, "registerHandler", JS_NewCFunction(ctx, js_repl_register_handler, "registerHandler", 1));

	JS_FreeValue(ctx, beapi);
	JS_FreeValue(ctx, global);
	
#ifndef PLATFORM_WASM
	// be_telnet_ws_require(ctx, telnet) ;
#endif


#ifdef PLATFORM_LINUX
	be_telnet_stdio_require(ctx) ;
#endif

#ifdef PLATFORM_ESP32
	be_telnet_ble_require(ctx) ;
#endif

}
void be_telnet_loop(JSContext *ctx) {
	// printf("be_telnet_loop()\n") ;

#ifdef PLATFORM_LINUX
	be_telnet_stdio_loop(ctx) ;
#endif

#ifdef PLATFORM_ESP32
	be_telnet_serial_loop(ctx) ;
#endif

#ifdef PLATFORM_WASM
	be_telnet_wasm_loop(ctx) ;
#endif
}

void be_telnet_reset(JSContext *ctx) {
    JS_FreeValue(ctx, _func_repl_input) ;
    _func_repl_input = JS_NULL ;

#ifdef PLATFORM_LINUX
	be_telnet_stdio_reset(ctx) ;
#endif

#ifdef PLATFORM_ESP32
	be_telnet_serial_reset(ctx) ;
	be_telnet_ble_reset(ctx) ;
#endif
	
}
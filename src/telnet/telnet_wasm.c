#ifdef PLATFORM_WASM

#include "telnet_wasm.h"
#include "utils.h"
#include <string.h>
#include "telnet_protocal.h"
#include <emscripten.h>
#include <math.h>


EMSCRIPTEN_KEEPALIVE bool be_telnet_wasm_send(uint8_t * data, size_t datalen) {
	telnet_prot_receive(data, &datalen) ;
	return true ;
}


static void send_pkg_func(const uint8_t * data, size_t datalen) {
	EM_ASM_ARGS({
		Module.onOutput(Module['HEAPU8'].buffer.slice($0, $0 + $1)); 
	}, data, datalen);
}

void be_telnet_wasm_init() {
	be_telnet_proto_init(send_pkg_func) ;
}


void be_telnet_wasm_loop(JSContext * ctx) {
	be_telnet_proto_loop(ctx) ;
}



#endif
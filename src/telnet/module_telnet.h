#if !defined(_TELNET_H)
#define _TELNET_H

#include "quickjs-libc.h"
#include "telnet_protocal.h"
#include <stdio.h>
#include <stdlib.h>

void be_telnet_init() ;
void be_telnet_require(JSContext *ctx) ;
void be_telnet_loop(JSContext *ctx) ;
void be_telnet_reset(JSContext *ctx) ;


uint8_t mk_echo_pkgid() ;

void telnet_send_ready() ;
void telnet_run(JSContext * ctx, uint8_t pkgid, uint8_t cmd, uint8_t * data, size_t datalen) ;
void telnet_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) ;

#define echo(sth) 		telnet_output(mk_echo_pkgid(), CMD_OUTPUT, sth, strlen(sth))
#define echon(sth, len) telnet_output(mk_echo_pkgid(), CMD_OUTPUT, sth, len)
#define echof(...) 														\
	{																	\
		char * str = NULL;												\
		int len = asprintf(&str, __VA_ARGS__) ;							\
        if(len>0)								    					\
            telnet_output(mk_echo_pkgid(), CMD_OUTPUT, str, len) ;	    \
		if(len>-1) {													\
			free(str) ;													\
		}																\
	}


#endif
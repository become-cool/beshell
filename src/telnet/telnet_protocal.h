#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "be_list.h"


/**
 * 0518
 * | H1 | H2 | PkgID | CMD | DATA LEN(1-4) | DATA ... | VerifyBit |
 */


// 05 18
#define PKG_HEAD1                   5
#define PKG_HEAD2                   18
#define PKG_MIN_SIZE                5

#define CMD_RUN	                	1		// 执行js代码，无返回值
#define CMD_CALL	                2		// 执行js代码，有返回值
#define CMD_CALL_ASYNC              3		// 执行js代码，有异步返回值
#define CMD_RSPN	                4		// CMD_CALL 的返回
#define CMD_EXCEPTION	            5		// CMD_RUN/CMD_CALL 的异常
#define CMD_CALLBACK                6		// 执行本地js代码(远程发起)
#define CMD_OUTPUT	                7		// 远程输出
#define CMD_DATA	                8		// 数据包(用于 CMD_FILE_PULL_REQ 的回应)
#define CMD_MSG	                    9		// 主动向客户端推送的事件

#define CMD_FILE_OPEN_REQ			10		// 打开文件，等待后续接受文件后写入. DATA区为文件内容
#define CMD_FILE_OFFSET_REQ			11		// 偏移
#define CMD_FILE_PUSH_REQ			12		// 发送文件
#define CMD_FILE_CLOSE_REQ			13		// 关闭文件
#define CMD_FILE_PULL_REQ			20		// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + MaxSize(uint16)
#define CMD_RESET			        30		// 重置命令. DATA区格式：boot_level (uint8)
#define CMD_READY           		31		// 系统准备就绪事件


typedef void (*TelnetProtFuncSend)(const uint8_t * data, size_t datalen);
typedef void (*TelnetProtFuncReset)(uint8_t level);

typedef struct{
    be_list_item_t base ;

	uint8_t pkgid ;
	uint8_t cmd ;

	uint8_t * data ;
	size_t data_len ;
	size_t data_received ;

	uint8_t verifysum ;
    
} telnet_pkg_t ;

extern TelnetProtFuncReset telnet_prot_func_reset ;

void telnet_prot_reset() ;
uint8_t * telnet_prot_pack(uint8_t pkgId, uint8_t cmd, uint8_t * dat, size_t datalen, size_t * pkglen) ;

void telnet_proto_send_pkg(uint8_t pkgid, uint8_t cmd, uint8_t * data, size_t datalen) ;

void telnet_proto_free_pkg(telnet_pkg_t * pkg) ;
void telnet_proto_process_pkg (telnet_pkg_t * pkg, void * ctx) ;


void be_telnet_proto_init() ;
void be_telnet_proto_loop(void * ctx) ;
void be_telnet_proto_receive(uint8_t * bytes, size_t length) ;

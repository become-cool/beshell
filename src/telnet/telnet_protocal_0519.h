#ifndef __TELNET_PROTOCAL_0519
#define __TELNET_PROTOCAL_0519

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


/**
BeProtocal 协议的设计目标是最大减轻单片机的负担：
1. 最小的包头
2. 解析时使用的内存最小

0518 协议数据包长度限制在255以内
0519 协议采用不定长：当包长小于 127 时，使用1个字节表示包长；当包长大于 127时，使用多于1个字节表示包长，第一个长度字节高位为1; 小端，低位在前

*/


#define PKG0519_HEADERLEN_WITHOUT_DATALEN   6
#define PKG0519_HEADERLEN_MIN               PKG0519_HEADERLEN_WITHOUT_DATALEN + 1
#define PKG0519_HEADERLEN_MAX               PKG0519_HEADERLEN_WITHOUT_DATALEN + 4
#define PKG0519_SIZE_1                      (0xFF>>1)            // 由1个字节表示包长
#define PKG0519_SIZE_2                      (0xFFFF>>2)          // 由2个字节表示包长
#define PKG0519_SIZE_3                      (0xFFFFFF>>3)        // 由3个字节表示包长
#define PKG0519_SIZE_4                      (0xFFFFFFFF>>3)      // 由4个字节表示包长

#define BUFFLEN_RECEIVE_0519        132     // > PKG0519_HEADER_MIN + 127

// 05 19
#define PKG_MARK1                   5
#define PKG_MARK2                   19

#define PKG0519_POS_MARK1           0
#define PKG0519_POS_MARK2           1
#define PKG0519_POS_ID              2
#define PKG0519_POS_CMD             3
#define PKG0519_POS_VERIFY          4
#define PKG0519_POS_DATA_LEN        5

#define CMD_RUN	                	1		// 执行js代码，无返回值
#define CMD_CALL	                2		// 执行js代码，有返回值
#define CMD_CALL_ASYNC              3		// 执行js代码，有异步返回值
#define CMD_RSPN	                4		// CMD_CALL 的返回
#define CMD_EXCEPTION	            5		// CMD_RUN/CMD_CALL 的异常
#define CMD_CALLBACK                6		// 执行本地js代码(远程发起)
#define CMD_OUTPUT	                7		// 远程输出
#define CMD_DATA	                8		// 数据包(可用于CMD_FILE_PULL_REQ的回应)
#define CMD_MSG	                    9		// 主动向客户端推送的事件

#define CMD_FILE_PUSH_REQ			10		// 发送文件. DATA区格式：路径(0结尾字符串) + 文件内容
#define CMD_FILE_APPEND_REQ			11		// 发送文件后续内容. DATA区格式和 CMD_FILE_APPEND 相同
#define CMD_FILE_PULL_REQ			12		// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + MaxSize(uint16)
#define CMD_RESET			        30		// 重置命令. DATA区格式：boot_level (uint8)
#define CMD_READY           		31		// 系统准备就绪事件


#define INIT_PKG(var)                               \
    memset(var, 0 , sizeof(bepkg_t));               \
    (var)->header[PKG0519_POS_MARK1] = PKG_MARK1 ;    \
    (var)->header[PKG0519_POS_MARK2] = PKG_MARK2 ;

// 在栈上创建一个包结构
#define DEF_PKG(var) bepkg_t var ; INIT_PKG(&var) ;
// 在堆上创建一个包结构
#define NEW_PKG(var) bepkg_t * var = malloc(sizeof(bepkg_t)) ; INIT_PKG(var) ;

typedef struct {
    uint8_t header[PKG0519_HEADERLEN_MAX] ;
    uint8_t * bodyseg ;
    void * opa ;
} bepkg_t ;

// typedef void (*Pkg0519CmdProcess)(uint8_t pkgId, uint8_t remain, uint8_t pkgCmd, const uint8_t * pkgDat, uint8_t pkgDatLen, const void * ctx);
// extern Pkg0519CmdProcess telnet_prot0519_on_package ;

// struct telnet_prot0519_buffer {
//     uint8_t bytes[BUFFLEN_RECEIVE_0519] ;
//     uint16_t writepos ;
// } ;


// void telnet_prot0519_push_char(struct telnet_prot_buffer * buff, uint8_t byte, const void * ctx) ;
// uint8_t telnet_prot0519_pack_header(uint8_t * header, uint8_t pkgId, uint8_t cmd, size_t datalen) ;

uint8_t telnet_prot0519_pack_data_len(uint32_t len, uint8_t * len_bytes) ;
uint8_t telnet_prot0519_checksum(uint8_t * data, uint32_t len) ;


#endif
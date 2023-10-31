#include "telnet_protocal_0519.h"
#include "utils.h"
#include <string.h>

// PkgCmdProcess telnet_prot0519_on_package = 0 ;



uint8_t telnet_prot0519_checksum(uint8_t * data, uint32_t len) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<len; i++) {
        sum^= data[i] ;
    }
    return sum ;
}



uint8_t telnet_prot0519_pack_data_len(uint32_t len, uint8_t * len_bytes) {
    *(len_bytes+0) = (len>>(7*0)) & 0x7F ;
    *(len_bytes+1) = (len>>(7*1)) & 0x7F ;
    *(len_bytes+2) = (len>>(7*2)) & 0x7F ;
    *(len_bytes+3) = (len>>(7*3)) & 0xFF ;  // 最高位字节可以用满8位

    if( *(len_bytes+3)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        *(len_bytes+2) |= 0x80 ;
        return 4 ;
    }
    else if( *(len_bytes+2)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        return 3 ;
    }
    else if( *(len_bytes+1)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        return 2 ;
    }

    return 1 ;
}
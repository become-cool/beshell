#include "sniffer.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "utils.h"
#include "esp_vfs_dev.h"
#include "logging.h"
#include <sys/errno.h>

#include "module_telnet.h"
#include "module_wifi.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

// LOG_TAG("sniffer")


#define SNIFFER_PORT 8150


struct timeval sniffer_tv = {
    .tv_sec = 0,
    .tv_usec = 1,
} ;

char sniffer_rx_buffer[8];

int sniffer_sock = -1 ;
fd_set sniffer_rfds;

struct sockaddr_in6 sniffer_source_addr; // Large enough for both IPv4 or IPv6
socklen_t sniffer_socklen = sizeof(sniffer_source_addr);


void be_module_sniffer_init() {

    if(!wifi_has_inited()) {
        return ;
    }
    
    if(sniffer_sock>-1) {
        echo("udp sniffer answer startup already.\n") ;
        return ;
    }
    // echo("jswrap_be_sniffer_answerer_init()\n") ;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SNIFFER_PORT);


    sniffer_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sniffer_sock < 0) {
        echof("Unable to create socket: errno %d\n", errno);
        return;
    }
    // echo("Socket created");

    int err = bind(sniffer_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        echof("Socket unable to bind: errno %d\n", errno);
    }
    // echof("Socket bound, port %d", SNIFFER_PORT);
}


inline void be_module_sniffer_loop() {
    if(!wifi_has_inited()) {
        return ;
    }

    FD_ZERO(&sniffer_rfds);

    int fds = -1 ;
    FD_SET(sniffer_sock, &sniffer_rfds);
    fds = sniffer_sock ;

    int s = select( fds + 1, &sniffer_rfds, NULL, NULL, &sniffer_tv);
    // 超时
    if(s==0) {
        return ;
    }
    // 失败
    if(s<0){
        echof("sniffer answerer select() error: %d.\n", errno) ;
        return ;
    }

    // 无数据
    if(!FD_ISSET(sniffer_sock, &sniffer_rfds)) {
        return ;
    }

    uint8_t len = recvfrom(sniffer_sock, sniffer_rx_buffer, sizeof(sniffer_rx_buffer) - 1, 0, (struct sockaddr *)&sniffer_source_addr, &sniffer_socklen);
    if (len < 0) {
        return;
    }

    char rspn = 0 ;
    for(uint8_t i=0;i<len; i++) {
        rspn = 0 ;
        if(sniffer_rx_buffer[i] == 'i')
            rspn = 'o' ;
        else if(sniffer_rx_buffer[i] == 'o')
            rspn = 'i' ;
        if(rspn!=0) {
            sendto(sniffer_sock, &rspn, 1, 0, (struct sockaddr *)&sniffer_source_addr, sniffer_socklen);
        }
    }
}


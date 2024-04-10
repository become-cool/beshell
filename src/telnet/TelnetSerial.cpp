#include "TelnetSerial.hpp"
#include "Telnet.hpp"
#include "Protocal.hpp"
#include <iostream>
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "esp_vfs.h"
#include "debug.h"
#include "qjs_utils.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>

#define UART_NUM        UART_NUM_0
#define BUF_SIZE        1024
#define RD_BUF_SIZE     BUF_SIZE
#define PATTERN_CHR_NUM 3

#define PKG_QUEUE_LEN 64

using namespace std ;

namespace be {

    
    void TelnetSerial::task(void * argv) {
        
        fd_set rfds;
        struct timeval tv;
        int ret;
        uint8_t *buf = (uint8_t *) malloc(RD_BUF_SIZE);

        // forwarding received package to telnet
        Parser parser([argv](std::unique_ptr<Package> pkg){
            // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
            Package * ptr = pkg.release() ;
            xQueueSend(((TelnetSerial*)argv)->pkg_queue, &ptr, 0) ;
        }) ;

        while (1) {
            FD_ZERO(&rfds);
            FD_SET(UART_NUM, &rfds);

            tv.tv_sec = 1;
            tv.tv_usec = 0;

            ret = select(UART_NUM + 1, &rfds, NULL, NULL, &tv);
            if (ret > 0) {
                if (FD_ISSET(UART_NUM, &rfds)) {
                    int len = uart_read_bytes(UART_NUM, buf, BUF_SIZE, 100/portTICK_PERIOD_MS);
                    if (len > 0) {
                        // printf("Received %d bytes\n", len);
                        parser.parse(buf, len) ;
                    }
                }
            }
        }

        free(buf);
        vTaskDelete(NULL);
    }

    void TelnetSerial::setup () {

        cout << flush ;
        vTaskDelay(15/portTICK_PERIOD_MS) ;

        if(!uart_is_driver_installed(UART_NUM)) {
            uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
            vTaskDelay(15/portTICK_PERIOD_MS) ;
        }
        uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };
        uart_param_config(UART_NUM, &uart_config);

        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        //Set uart pattern detect function.
        uart_enable_pattern_det_baud_intr(UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);

        //Reset the pattern queue length to record at most 20 pattern positions.
        uart_pattern_queue_reset(UART_NUM, 20);

        pkg_queue = xQueueCreate(PKG_QUEUE_LEN, sizeof(Package *));
        xTaskCreatePinnedToCore(&TelnetSerial::task, "be-telnet-seiral", 6*1024, this, tskIDLE_PRIORITY, &taskHandle, 1) ;
    }

    void TelnetSerial::loop () {
        Package * ptr ;
        std::unique_ptr<Package> pkg ;
        if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
            pkg.reset(ptr) ;
            // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
            if(telnet){
                telnet->onReceived(this,move(pkg)) ;
            }
        }
    }

    void TelnetSerial::sendData (const char * data, size_t datalen) {
        if(data && datalen) {
            uart_write_bytes(UART_NUM, data, datalen);
        }
    }
}
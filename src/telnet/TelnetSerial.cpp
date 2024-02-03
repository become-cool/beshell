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

    
    void TelnetSerial::task2(void * argv) {
        
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

    void TelnetSerial::task(void * argv) {

        // forwarding received package to telnet
        Parser parser([argv](std::unique_ptr<Package> pkg){
            // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
            Package * ptr = pkg.release() ;
            xQueueSend(((TelnetSerial*)argv)->pkg_queue, &ptr, 0) ;
        }) ;

        uart_event_t event;
        size_t buffered_size;
        uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
        size_t chunklen = 0 ;

        for(;;) {
            //Waiting for UART event.
            if(xQueueReceive(((TelnetSerial*)argv)->uart_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
                bzero(dtmp, RD_BUF_SIZE);
                switch(event.type) {
                    case UART_DATA: {
                        uint64_t t = gettime() ;
                        chunklen = uart_read_bytes(UART_NUM, dtmp, event.size, 100/portTICK_PERIOD_MS);
                        parser.parse(dtmp, chunklen) ;
                        dn64(gettime()-t) ;
                        break;
                    }
                    //Event of HW FIFO overflow detected
                    case UART_FIFO_OVF:
                    //Event of UART ring buffer full
                    case UART_BUFFER_FULL:
                        uart_flush_input(UART_NUM);
                        xQueueReset(((TelnetSerial*)argv)->uart_queue);
                        break;
                    //Event of UART RX break detected

                    //UART_PATTERN_DET
                    case UART_PATTERN_DET:
                        uart_get_buffered_data_len(UART_NUM, &buffered_size);
                        int pos = uart_pattern_pop_pos(UART_NUM);
                        // ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                        if (pos == -1) {
                            // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                            // record the position. We should set a larger queue size.
                            // As an example, we directly flush the rx buffer here.
                            uart_flush_input(UART_NUM);
                        } else {
                            uart_read_bytes(UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                            uint8_t pat[PATTERN_CHR_NUM + 1];
                            memset(pat, 0, sizeof(pat));
                            uart_read_bytes(UART_NUM, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                        }
                        break;
                }
            }
        }
        free(dtmp);
        dtmp = NULL;
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
        xTaskCreatePinnedToCore(&TelnetSerial::task2, "be-telnet-seiral", 6*1024, this, tskIDLE_PRIORITY, &taskHandle, 1) ;   
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
        if(datalen && datalen) {
            uart_write_bytes(UART_NUM, data, datalen);
        }
    }
}
#include "TelnetSerial.hpp"
#include "Protocal.hpp"
#include <iostream>
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "esp_vfs.h"
#include "debug.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>

#define UART_NUM        UART_NUM_0
#define BUF_SIZE        1024
#define RD_BUF_SIZE     BUF_SIZE
#define PATTERN_CHR_NUM 3

#define PKG_QUEUE_LEN 5

namespace beshell {
    TelnetSerial::TelnetSerial() {}

    void TelnetSerial::task(void * argv) {
        
        Parser parser([argv](Package * pkg){
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
                    case UART_DATA:
                        chunklen = uart_read_bytes(UART_NUM, dtmp, event.size, 1/portTICK_PERIOD_MS);
				        parser.parse(dtmp, chunklen) ;

                        break;
                    //Event of HW FIFO overflow detected
                    case UART_FIFO_OVF:
                    //Event of UART ring buffer full
                    case UART_BUFFER_FULL:
                        uart_flush_input(UART_NUM);
                        xQueueReset(((TelnetSerial*)argv)->uart_queue);
                        break;
                    //Event of UART RX break detected
                    case UART_BREAK:
                        std::cout << "UART_BREAK\n" << std::endl ;
                        break;
                    //Event of UART parity check error
                    case UART_PARITY_ERR:
                        std::cout << "UART_PARITY_ERR\n" << std::endl ;
                        break;
                    //Event of UART frame error
                    case UART_FRAME_ERR:
                        std::cout << "UART_FRAME_ERR\n" << std::endl ;
                        break;
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
        
        /* Configure parameters of an UART driver,
        * communication pins and install the driver */
        uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };
        //Install UART driver, and get the queue.
        uart_driver_install(0, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
        uart_param_config(UART_NUM, &uart_config);

        //Set UART log level
        // esp_log_level_set(TAG, ESP_LOG_INFO);
        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        //Set uart pattern detect function.
        uart_enable_pattern_det_baud_intr(UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
        //Reset the pattern queue length to record at most 20 pattern positions.
        uart_pattern_queue_reset(UART_NUM, 20);

        xQueue = xQueueCreate(PKG_QUEUE_LEN, sizeof(Package));
        xTaskCreatePinnedToCore(&TelnetSerial::task, "be-telnet-seiral", 6*1024, this, tskIDLE_PRIORITY, &taskHandle, 0) ;   
    }

    void TelnetSerial::loop () {

    }
}
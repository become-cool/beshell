#include "TelnetCDC.hpp"
#include "Telnet.hpp"
#include "Protocal.hpp"
#include <iostream>
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

    
    // void TelnetCDC::task(void * argv) {
        
    //     fd_set rfds;
    //     struct timeval tv;
    //     int ret;
    //     uint8_t *buf = (uint8_t *) malloc(RD_BUF_SIZE);

    //     // forwarding received package to telnet
    //     Parser parser([argv](std::unique_ptr<Package> pkg){
    //         // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
    //         Package * ptr = pkg.release() ;
    //         xQueueSend(((TelnetCDC*)argv)->pkg_queue, &ptr, 0) ;
    //     }) ;

    //     while (1) {
    //         // 从USB CDC接收数据
    //         int len = uart_read_bytes(UART_NUM, data, 64, pdMS_TO_TICKS(100));
    //         if (len > 0) {
    //             // 处理接收到的数据，这里我们简单地打印出来
    //             ESP_LOGI(TAG, "Received data from USB CDC: %.*s", len, data);

    //             // 发送数据回USB CDC
    //             const char *response = "Hello from ESP32!";
    //             uart_write_bytes(UART_NUM, response, strlen(response));
    //         }

    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }


    //     free(buf);
    //     vTaskDelete(NULL);
    // }

    // void TelnetCDC::setup () {

    //     cout << flush ;
    //     vTaskDelay(15/portTICK_PERIOD_MS) ;
        
    //     esp_usb_config_t usb_config = {
    //         .enable = true,
    //         .dm_pullup = true,
    //         .dp_pullup = true,
    //         .vbus_sense = false,
    //     };
    //     esp_err_t ret = esp_usb_driver_install(&usb_config);
    //     if (ret != ESP_OK) {
    //         ESP_LOGE(TAG, "USB driver install failed");
    //     }

    //     pkg_queue = xQueueCreate(PKG_QUEUE_LEN, sizeof(Package *));
    //     xTaskCreatePinnedToCore(&TelnetCDC::task, "be-telnet-cdc", 5*1024, this, tskIDLE_PRIORITY, &taskHandle, 1) ;
    // }

    // void TelnetCDC::loop () {
    //     Package * ptr ;
    //     std::unique_ptr<Package> pkg ;
    //     if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
    //         pkg.reset(ptr) ;
    //         // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
    //         if(telnet){
    //             telnet->onReceived(this,move(pkg)) ;
    //         }
    //     }
    // }

    // void TelnetCDC::sendData (const char * data, size_t datalen) {
    //     if(data && datalen) {
    //         uart_write_bytes(UART_NUM, data, datalen);
    //     }
    // }
}
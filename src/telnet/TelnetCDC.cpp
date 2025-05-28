#include "TelnetCDC.hpp"
#include "Telnet.hpp"
#include "Protocal.hpp"
#include <iostream>
#include "debug.h"
#include "qjs_utils.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include "Protocal.hpp"
#include "driver/usb_serial_jtag.h"

#define UART_NUM        UART_NUM_0
#define BUF_SIZE        1024
#define RD_BUF_SIZE     BUF_SIZE
#define PATTERN_CHR_NUM 3

#define PKG_QUEUE_LEN 64

using namespace std ;

namespace be {

    
    void TelnetCDC::taskListen(TelnetCDC * cdc) {
        
        assert(cdc) ;
        
        // forwarding received package to telnet
        Parser parser([cdc](std::unique_ptr<Package> pkg, void * opaque) {
            assert(cdc->telnet) ;
            pkg->channle = (TelnetChannel*)cdc ;
            cdc->telnet->execPackage(pkg) ;
        }) ;

        uint8_t buf[256];
        while (1) {
            int len = usb_serial_jtag_read_bytes(buf, sizeof(buf), pdMS_TO_TICKS(1000));
            if (len > 0) {
                parser.parse(buf, len) ;
            }
        }
    }

    void TelnetCDC::setup () {
        setup(256, 256) ;
    }
    void TelnetCDC::setup (uint32_t rx_size, uint32_t tx_size) {
        if(setuped) {
            return ;
        }

        // Create configuration structure
        usb_serial_jtag_driver_config_t config = {
            .tx_buffer_size = tx_size,
            .rx_buffer_size = rx_size
        };
        
        // Call the driver installation function with config struct
        esp_err_t err = usb_serial_jtag_driver_install(&config);
        
        // Throw exception if error occurred
        if (err != ESP_OK) {
            printf("USB Serial JTAG driver installation failed: %d\n", err);
            return ;
        }

        rx_buffer_size = rx_size;
        tx_buffer_size = tx_size;
    
        // 创建持续运行的 USB 任务
        xTaskCreatePinnedToCore((TaskFunction_t)taskListen, "usb_cdc_task", 4096, (void *)this, 5, NULL, 1);
        
        setuped = true ;
    }

    void TelnetCDC::loop () {
        Package * ptr ;
        std::unique_ptr<Package> pkg ;
        if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
            pkg.reset(ptr) ;
            if(telnet){
                telnet->onReceived(this,move(pkg)) ;
            }
        }
    }

    void TelnetCDC::sendData (const char * data, size_t datalen) {
        if(!setuped) {
            return ;
        }
        int buffsize = tx_buffer_size/2;
        if(data && datalen) {

            int chunk_size = 0;
            char * chunk = (char *)data;
            while(datalen) {
                chunk_size = datalen > buffsize ? buffsize : datalen;

                int sentlen = usb_serial_jtag_write_bytes(chunk, chunk_size, 100/portTICK_PERIOD_MS);
            
                datalen -= chunk_size;
                chunk += chunk_size;
            }
        }
    }
}
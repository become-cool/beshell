#include "REPLCDC.hpp"
#include "REPL.hpp"
#include "Protocal.hpp"
#include <iostream>
// #include "debug.h"
#include "qjs_utils.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include "Protocal.hpp"
#include "driver/usb_serial_jtag.h"
#include "driver/uart.h"
#include "soc/usb_serial_jtag_struct.h"
#include "sdkconfig.h"

#define UART_NUM        UART_NUM_0
#define BUF_SIZE        1024
#define RD_BUF_SIZE     BUF_SIZE
#define PATTERN_CHR_NUM 3

#define PKG_QUEUE_LEN 64

using namespace std ;

#if CONFIG_USB_OTG_SUPPORTED

extern "C" {
    bool usb_serial_jtag_write_ready(void) ;
}

void pf(const char *format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    if (len > 0) {
        uart_write_bytes(UART_NUM_0, buf, len);
    }
}
#define dd pf("%s: %d\n", __func__, __LINE__);
#define dn2(a,b) pf("%s: %d, %d\n", __func__, a,b);
#define dn3(a,b,c) pf("%s: %d, %d, %d\n", __func__, a,b,c);
#define dn4(a,b,c,d) pf("%s: %d, %d, %d, %d\n", __func__, a,b,c,d);

namespace be {

    
    void REPLCDC::taskListen(REPLCDC * cdc) {
        
        assert(cdc) ;
        
        // forwarding received package to repl
        Parser parser([cdc](std::unique_ptr<Package> pkg, void * opaque) {
            assert(cdc->repl) ;
            // printf("Received package from USB CDC: cmd=%d, body_len=%d\n", pkg->head.fields.cmd, pkg->body_len) ;
            pkg->channle = (REPLChannel*)cdc ;
            cdc->repl->execPackage(pkg) ;
        }) ;

        uint8_t buf[256];
        while (1) {
            int len = usb_serial_jtag_read_bytes(buf, sizeof(buf), pdMS_TO_TICKS(1000));
            if (len > 0) {
                // printf("Read %d bytes from USB CDC\n", len);
                parser.parse(buf, len) ;
            }
        }
    }

    void REPLCDC::setup () {
        setup(512, 256) ;
    }
    void REPLCDC::setup (uint32_t rx_size, uint32_t tx_size) {
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

    void REPLCDC::loop () {
        Package * ptr ;
        std::unique_ptr<Package> pkg ;
        if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
            pkg.reset(ptr) ;
            if(repl){
                repl->onReceived(this,move(pkg)) ;
            }
        }
    }

    void REPLCDC::sendData (const char * data, size_t datalen) {
        if(!setuped) {
            return ;
        }
        if(!usb_serial_jtag_is_connected()) {
            return ;
        }
        // 不需要显式判断 COM 口是否打开：当无终端连接时，
        // TX FIFO 不被消费 → ring buffer 满 → write_ready 返回 false，
        // 驱动的自然背压机制已处理此情况
        int buffsize = tx_buffer_size/3;
        if(data && datalen) {

            int chunk_size = 0;
            char * chunk = (char *)data;
            for(int i=0; datalen>0; i++) {

                chunk_size = datalen > buffsize ? buffsize : datalen;

                // 当没有上位机连接时，数据会在缓冲区等待，此时等待数据发送完成是无效的
                // 第1个chunk遇到缓冲满，取消发送；后续chunk等待30ms
                int sentlen = usb_serial_jtag_write_bytes(chunk, chunk_size, 30/portTICK_PERIOD_MS);
                if(sentlen!=chunk_size) {
                    return ;
                }

                datalen -= chunk_size;
                chunk += chunk_size;
            }
        }
    }
}
#endif

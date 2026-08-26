#pragma once

#include "sdkconfig.h"
#include "soc/soc_caps.h"

#include "REPLChannel.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#if SOC_USB_SERIAL_JTAG_SUPPORTED
namespace be {
    class REPLCDC: public REPLChannel {
    private:
        TaskHandle_t taskHandle = nullptr ;
        QueueHandle_t pkg_queue;
        bool setuped = false ;

        uint32_t rx_buffer_size = 0;
        uint32_t tx_buffer_size = 0;

        static void taskListen(REPLCDC * cdc) ;

    public:
        using REPLChannel::REPLChannel ;
        void setup() ;
        void setup(uint32_t rx_buffer_size, uint32_t tx_buffer_size) ;
        void loop () ;
        void sendData (const char * data, size_t datalen) ;
    } ;
}
#endif

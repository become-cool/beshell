#pragma once

#include "TelnetChannel.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

namespace be {
    class TelnetCDC: public TelnetChannel {
    private:
        TaskHandle_t taskHandle = nullptr ;
        QueueHandle_t pkg_queue;
        bool setuped = false ;

        uint32_t rx_buffer_size = 0;
        uint32_t tx_buffer_size = 0;

        static void taskListen(TelnetCDC * argv) ;

    public:
        using TelnetChannel::TelnetChannel ;
        void setup() ;
        void setup(uint32_t rx_buffer_size, uint32_t tx_buffer_size) ;
        void loop () ;
        void sendData (const char * data, size_t datalen) ;
    } ;
}
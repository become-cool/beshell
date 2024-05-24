#pragma once

#include "TelnetChannel.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

namespace be {
    class TelnetCDC: public TelnetChannel {
    private:
        TaskHandle_t taskHandle = nullptr ;
        QueueHandle_t uart_queue;
        QueueHandle_t pkg_queue;

        static void task(void * argv) ;

    public:
        using TelnetChannel::TelnetChannel ;
        void setup() ;
        void loop () ;
        void sendData (const char * data, size_t datalen) ;
    } ;
}
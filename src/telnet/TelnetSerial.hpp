#pragma once

#include "TelnetChannel.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

namespace beshell {
    class TelnetSerial: public TelnetChannel {
    private:
        TaskHandle_t taskHandle = nullptr ;
        QueueHandle_t uart_queue;
        QueueHandle_t pkg_queue;

        static void task(void * argv) ;
    public:
        TelnetSerial() ;
        void setup() ;
        void loop () ;
    } ;
}
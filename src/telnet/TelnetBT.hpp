#pragma once
#include "TelnetChannel.hpp"
#include "freertos/queue.h"

namespace be {
    class TelnetBT: public TelnetChannel  {
    private:
        bool setuped = true ;
        QueueHandle_t pkg_queue;
    public:
        using TelnetChannel::TelnetChannel ;
        
        // void setup(const char * name="BeShell", uint16_t characteristicID=0x0512, uint16_t serviceID=0x0B0C) ;
        void loop () ;
    } ;
}
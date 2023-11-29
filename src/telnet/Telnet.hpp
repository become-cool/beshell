#pragma once
#include "TelnetSerial.hpp"

namespace beshell {
    class Telnet {
    private:

        TelnetSerial channelSeiral ;

    public:
        Telnet() ;

        void setup() ;
        void loop() ;
    } ;
}
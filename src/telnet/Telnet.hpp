#pragma once
#include "telnet/TelnetSerial.hpp"

// #include "BeShell.hpp"

namespace beshell {
    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;

        TelnetPkgProcFunc onReceived ;
        TelnetSerial channelSeiral ;

        uint8_t autoIncreasePkgId = 0 ;

    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;

        void output(uint8_t cmd, uint8_t * data, size_t datalen, int pkgid=-1) ;
        void output(const char * data, size_t datalen) ;
    } ;
}
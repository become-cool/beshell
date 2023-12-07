#pragma once

#include "TelnetChannel.hpp"
#ifdef PLATFORM_ESP32
#include "TelnetSerial.hpp"
#endif 


namespace beshell {
    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;

        TelnetPkgProcFunc onReceived ;
        
#ifdef PLATFORM_ESP32
        TelnetSerial channelSeiral ;
#endif

        uint8_t autoIncreasePkgId = 0 ;

    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;

        void output(uint8_t cmd, uint8_t * data, size_t datalen, int pkgid=-1) ;
        void output(const char * data, size_t datalen) ;
    } ;
}
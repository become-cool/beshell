#pragma once
#include "TelnetSerial.hpp"

// #include "BeShell.hpp"

namespace beshell {
    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;

        PackageProcFunc onReceived ;
        TelnetSerial channelSeiral ;

        uint8_t autoIncreasePkgId = 0 ;

    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;

        void output(uint8_t cmd, uint8_t * data, size_t datalen, int pkgid=-1) ;
    } ;
}
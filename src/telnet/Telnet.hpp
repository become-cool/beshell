#pragma once
#include "TelnetSerial.hpp"

// #include "BeShell.hpp"

namespace beshell {
    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;

        PackageProcFunc onPackage ;
        TelnetSerial channelSeiral ;

    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;

        void output(int pkgid, uint8_t cmd, uint8_t * data, size_t datalen) ;
    } ;
}
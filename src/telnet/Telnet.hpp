#pragma once

#include <memory>

#include "TelnetChannel.hpp"
#ifdef PLATFORM_ESP32
#include "TelnetSerial.hpp"
#endif 
#ifdef PLATFORM_LINUX
#include "TelnetStdIO.hpp"
#endif 


namespace be {
    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;

        // TelnetPkgProcFunc onReceived ;
        
#ifdef PLATFORM_ESP32
        TelnetSerial channelSeiral ;
#endif
#ifdef PLATFORM_LINUX
        TelnetStdIO channelStdIO ;
#endif

        uint8_t autoIncreasePkgId = 0 ;
    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;

        void output(const char * data, size_t datalen, int pkgid=-1, uint8_t cmd=OUTPUT) ;

        void onReceived(TelnetChannel * , Package &) ;
        static std::unique_ptr<std::ostream> createStream(Package & pkg) ;

        TelnetChannel * channel(const char * name) ;

    protected:
        void openFile(TelnetChannel * ch, Package & pkg, bool append) ;
        void offsetFile(TelnetChannel * ch, Package & pkg) ;
        void closeFile(TelnetChannel * ch, Package & pkg) ;
        void pushFile(TelnetChannel * , Package &) ;
        void pullFile(TelnetChannel * , Package &) ;
    } ;
}
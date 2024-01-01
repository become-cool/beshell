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

        void onReceived(TelnetChannel * , std::unique_ptr<Package>) ;
        static std::unique_ptr<std::ostream> createStream(Package & pkg) ;

        TelnetChannel * channel(const char * name) ;

    protected:
        void openFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg, bool append) ;
        void offsetFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void closeFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void pushFile(TelnetChannel * , std::unique_ptr<Package> &) ;
        void pullFile(TelnetChannel * , std::unique_ptr<Package> &) ;
    } ;
}
#pragma once

#include <memory>
#include <vector>

#include "TelnetChannel.hpp"

#ifdef ESP_PLATFORM
#include "freertos/queue.h"

#include "TelnetSerial.hpp"
#include "TelnetBLE.hpp"
#endif 
#ifdef LINUX_PLATFORM
#include "TelnetStdIO.hpp"
#endif 


namespace be {

    class BeShell ;
    class Telnet {
    private:

        BeShell * beshell ;
        
#ifdef ESP_PLATFORM
        TelnetSerial channelSeiral ;
#endif
#ifdef LINUX_PLATFORM
        TelnetStdIO channelStdIO ;
#endif

        uint8_t autoIncreasePkgId = 0 ;
        QueueHandle_t pkg_queue;

    public:
        static const char * const name ;
        Telnet(BeShell * beshell) ;

        void setup() ;
        void loop() ;
        TelnetBLE * bt = nullptr ;

        void output(const char * data, size_t datalen, int pkgid=-1, uint8_t cmd=OUTPUT) ;

        void onReceived(TelnetChannel * , std::unique_ptr<Package>) ;
        void execPackage(std::unique_ptr<Package> &) ;

        static std::unique_ptr<std::ostream> createStream(Package & pkg) ;

        TelnetChannel * channel(const char * name) ;
        
        void useBLE() ;

    protected:
        void openFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg, bool append) ;
        void offsetFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void closeFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void pushFile(TelnetChannel * , std::unique_ptr<Package> &) ;
        void pullFile(TelnetChannel * , std::unique_ptr<Package> &) ;
    } ;
}
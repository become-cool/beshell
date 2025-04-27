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
        TelnetBLE * channelBLE = nullptr ;
#endif
#ifdef LINUX_PLATFORM
        TelnetStdIO channelStdIO ;
#endif

        std::vector<TelnetChannel *> channels ;

        uint8_t autoIncreasePkgId = 0 ;
        QueueHandle_t pkg_queue;

    public:
        Telnet(BeShell * beshell) ;

        void setup() ;
        inline void loop() {
            Package * ptr ;
            std::unique_ptr<Package> pkg ;
            if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
                pkg.reset(ptr) ;
                // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
                onReceived(pkg->channle,move(pkg)) ;
            }
#ifdef LINUX_PLATFORM
            channelStdIO.loop() ;
#endif
        }

        void output(const char * data, size_t datalen, int pkgid=-1, uint8_t cmd=OUTPUT) ;
        void output(const std::string & data, int pkgid=-1, uint8_t cmd=OUTPUT) ;

        void onReceived(TelnetChannel * , std::unique_ptr<Package>) ;
        void execPackage(std::unique_ptr<Package> &) ;

        static std::unique_ptr<std::ostream> createStream(Package & pkg) ;

        TelnetChannel * channel(const char * name) ;

        void addChannel(TelnetChannel * ch) ;
        void removeChannel(TelnetChannel * ch) ;

    protected:
        void openFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg, bool append) ;
        void offsetFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void closeFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) ;
        void pushFile(TelnetChannel * , std::unique_ptr<Package> &) ;
        void pullFile(TelnetChannel * , std::unique_ptr<Package> &) ;
    } ;
}
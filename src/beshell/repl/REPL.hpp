#pragma once

#include <memory>
#include <vector>

#include "REPLChannel.hpp"

#ifdef ESP_PLATFORM
#include "freertos/queue.h"

#include "REPLSerial.hpp"
#endif 
#ifdef LINUX_PLATFORM
#include "REPLStdIO.hpp"
#endif 


namespace be {

    typedef void (*REPLDecryptFunc)(Package & pkg) ;
    typedef void (*REPLUnknownPkgHook)(REPLChannel * ch, std::unique_ptr<Package> pkg) ;

    class BeShell ;
    // class REPLModule ;
    class REPL {
    private:

        BeShell * beshell ;
        
#ifdef ESP_PLATFORM
        REPLSerial channelSeiral ;
        REPLChannel * channelBLE = nullptr ;
#endif
#ifdef LINUX_PLATFORM
        REPLStdIO channelStdIO ;
#endif

        std::vector<REPLChannel *> channels ;

        uint8_t autoIncreasePkgId = 0 ;
        QueueHandle_t pkg_queue;

        REPLDecryptFunc decryptFunc = nullptr ;

        bool enableCrypto = false ;
        unsigned char cryptoKey[16] = {0} ;
        unsigned char cryptoVI[16] = {0} ;

    public:
        REPLUnknownPkgHook unknownPkgHook = nullptr ;

        REPL(BeShell * beshell) ;

        void setup() ;
        inline void loop() {
            Package * ptr ;
            std::unique_ptr<Package> pkg ;
            if(xQueueReceive(pkg_queue, (void*)&ptr, 0)){
                pkg.reset(ptr) ;
                // dn3(pkg->head.fields.cmd, pkg->body_len, pkg->chunk_len)
                onReceived(pkg->channle,std::move(pkg)) ;
            }
#ifdef LINUX_PLATFORM
            channelStdIO.loop() ;
#endif
        }

        void output(const char * data, size_t datalen, int pkgid=-1, uint8_t cmd=OUTPUT) ;
        void output(const std::string & data, int pkgid=-1, uint8_t cmd=OUTPUT) ;

        void onReceived(REPLChannel * , std::unique_ptr<Package>) ;
        void execPackage(std::unique_ptr<Package> &) ;

        static std::unique_ptr<std::ostream> createStream(Package & pkg) ;

        REPLChannel * channel(const char * name) ;
        void setBLEChannel(REPLChannel * ch) ;

        void addChannel(REPLChannel * ch) ;
        void removeChannel(REPLChannel * ch) ;

        void setCryptoFunction(REPLDecryptFunc decryptFunc) ;
        void defaultREPLDecryptFunc(Package & pkg) ;

    protected:
        void openFile(REPLChannel * ch, std::unique_ptr<Package> & pkg, bool append) ;
        void offsetFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) ;
        void closeFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) ;
        void pushFile(REPLChannel * , std::unique_ptr<Package> &) ;
        void pullFile(REPLChannel * , std::unique_ptr<Package> &) ;

    friend class REPLModule ;
    } ;
}

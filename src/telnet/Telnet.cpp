#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include <cassert>


namespace be {
    Telnet::Telnet(BeShell * _beshell)
        : beshell(_beshell)
#ifdef PLATFORM_ESP32
        , channelSeiral(this)
#endif
#ifdef PLATFORM_LINUX
        , channelStdIO(this)
#endif
    {}

    void Telnet::setup() {
#ifdef PLATFORM_ESP32
        channelSeiral.setup() ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.setup() ;
#endif
    }
    void Telnet::loop() {
#ifdef PLATFORM_ESP32
        channelSeiral.loop() ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.loop() ;
#endif
    }

    void Telnet::onReceived(TelnetChannel * ch, Package & pkg){
        switch (pkg.head.fields.cmd)
        {
        case LINE:
        case RUN:
        case CALL:
            assert(beshell) ;
            beshell->repl->input(pkg, ch) ;
            break;
        }
    }


    void Telnet::output(const char * data, size_t datalen, int pkgid, uint8_t cmd) {
        if(pkgid<0) {
            pkgid = autoIncreasePkgId ++ ;
        }
        else {
            pkgid%= 255 ;
        }
        Package pkg((uint8_t)pkgid,cmd,(uint8_t *)data,datalen) ;
        pkg.pack() ;

#ifdef PLATFORM_ESP32
        channelSeiral.send(pkg) ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.send(data,datalen) ;
#endif
    }


}
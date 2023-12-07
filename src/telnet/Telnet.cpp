#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include <cassert>


namespace beshell {
    Telnet::Telnet(BeShell * _beshell)
        : beshell(_beshell)
        , onReceived([this](TelnetChannel * ch, Package & pkg){
            switch (pkg.head.fields.cmd)
            {
            case LINE:
            case RUN:
            case CALL:
                assert(beshell) ;
                beshell->repl.input(pkg) ;
                break;
            }
        })
#ifdef PLATFORM_ESP32
        , channelSeiral(onReceived)
#endif
    {}

    void Telnet::setup() {
#ifdef PLATFORM_ESP32
        channelSeiral.setup() ;
#endif
    }
    void Telnet::loop() {
#ifdef PLATFORM_ESP32
        channelSeiral.loop() ;
#endif
    }

    void Telnet::output(uint8_t cmd, uint8_t * data, size_t datalen, int pkgid) {
        if(pkgid<0) {
            pkgid = autoIncreasePkgId ++ ;
        }
        else {
            pkgid%= 255 ;
        }
        Package pkg(pkgid,cmd,data,datalen) ;
        pkg.pack() ;
#ifdef PLATFORM_ESP32
        channelSeiral.send(pkg) ;
#endif
    }

    void Telnet::output(const char * data, size_t datalen) {
#ifdef PLATFORM_ESP32
        channelSeiral.send(data,datalen) ;
#endif
    }

}
#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"


namespace beshell {
    Telnet::Telnet(BeShell * _beshell)
        : beshell(_beshell)
        , onReceived([this](Package & pkg){
            switch (pkg.head.fields.cmd)
            {
            case LINE:
            case RUN:
            case CALL:
                assert(beshell) ;
                ds(pkg.body)
                beshell->engine.evalSync((char *)pkg.body, pkg.body_len,"eval") ;
                break;
            }
        })
        , channelSeiral(onReceived)
    {}

    void Telnet::setup() {
        channelSeiral.setup() ;
    }
    void Telnet::loop() {
        channelSeiral.loop() ;
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
        channelSeiral.send(pkg) ;
    }

}
#include "TelnetChannel.hpp"
#include <cstring>


namespace be {
    TelnetChannel::TelnetChannel(Telnet * _telnet)
        : telnet(_telnet)
    {}
    void TelnetChannel::setup () {}
    void TelnetChannel::loop () {}
    
    void TelnetChannel::send (Package & pkg) {
        sendData((const char *)pkg.head.raw, pkg.head_len);
        sendData((const char *)pkg.body, pkg.body_len);
        sendData((const char *)&pkg.verifysum, 1);
    }
    void TelnetChannel::send (const char * data, int datalen, int pkgId, uint8_t cmd) {
        if(datalen<0) {
            datalen = strlen(data) ;
        }
        if(pkgId>=0) {
            Package pkg((uint8_t)pkgId, cmd, (uint8_t *)data, datalen) ;
            pkg.pack() ;
            send(pkg) ;
        } else if(datalen>0){
            sendData(data,datalen) ;
        }
    }
}
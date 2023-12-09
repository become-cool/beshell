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
    void TelnetChannel::send (const char * data, int datalen) {
        if(datalen<0) {
            datalen = strlen(data) ;
        }
        if(datalen>0) {
            sendData(data,datalen) ;
        }
    }
}
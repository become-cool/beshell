#include "TelnetChannel.hpp"



namespace beshell {
    TelnetChannel::TelnetChannel(TelnetPkgProcFunc _packageHandler)
        : packageHandler(_packageHandler)
    {}
    void TelnetChannel::setup () {}
    void TelnetChannel::loop () {}
    
    void TelnetChannel::send (Package & pkg) {
        send((const char *)pkg.head.raw, pkg.head_len);
        send((const char *)pkg.body, pkg.body_len);
        send((const char *)&pkg.verifysum, 1);
    }
}
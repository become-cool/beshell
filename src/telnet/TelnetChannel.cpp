#include "TelnetChannel.hpp"



namespace beshell {
    TelnetChannel::TelnetChannel(TelnetPkgProcFunc _packageHandler)
        : packageHandler(_packageHandler)
    {}
}
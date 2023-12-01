#include "TelnetChannel.hpp"



namespace beshell {
    TelnetChannel::TelnetChannel(PackageProcFunc _packageHandler)
        : packageHandler(_packageHandler)
    {}
}
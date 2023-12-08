#pragma once

#include "TelnetChannel.hpp"

#ifdef PLATFORM_LINUX

namespace be {
    class TelnetStdIO: public TelnetChannel {
    private:
    public:
        using TelnetChannel::TelnetChannel ;
        void setup () ;
        void loop () ;
        void send (Package & pkg) ;
        void send (const char * data, size_t datalen) ;
    } ;
}

#endif
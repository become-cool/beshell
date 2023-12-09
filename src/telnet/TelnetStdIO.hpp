#pragma once

#include "TelnetChannel.hpp"
#include <unistd.h>
#include <sys/types.h>


#ifdef PLATFORM_LINUX


namespace be {
    class TelnetStdIO: public TelnetChannel {
    private:
    
        fd_set readfds;
        struct timeval tv;
        uint8_t buf[10240] ;

    public:
        using TelnetChannel::TelnetChannel ;
        void setup () ;
        void loop () ;
        void sendData (const char * data, size_t datalen) ;
    
    } ;
}

#endif
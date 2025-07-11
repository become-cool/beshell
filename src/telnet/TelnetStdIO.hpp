#pragma once

#include "TelnetChannel.hpp"
#include <unistd.h>
#include <sys/types.h>


#ifdef LINUX_PLATFORM


namespace be {
    class Telnet ;
    class TelnetStdIO: public TelnetChannel {
    private:
    
        fd_set readfds;
        struct timeval tv;
        uint8_t buf[10240] ;

    protected:
        void sendData (const char * data, size_t datalen) ;

    public:
        // using TelnetChannel::TelnetChannel ;
        
        TelnetStdIO(Telnet *) ;
        void setup () ;
        void loop () ;
    
    } ;
}

#endif
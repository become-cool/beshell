#pragma once

#include "REPLChannel.hpp"
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#endif


#ifdef BESHELL_PLATFORM_PC


namespace be {
    class REPL ;
    class REPLStdIO: public REPLChannel {
    private:

#ifndef _WIN32
        fd_set readfds;
        struct timeval tv;
#endif
        uint8_t buf[10240] ;

    protected:
        void sendData (const char * data, size_t datalen) ;

    public:
        // using REPLChannel::REPLChannel ;
        
        REPLStdIO(REPL *) ;
        void setup () ;
        void loop () ;
    
    } ;
}

#endif
#include "TelnetStdIO.hpp"


#ifdef PLATFORM_LINUX

namespace beshell {
    void TelnetStdIO::loop () {

    }
    void TelnetStdIO::send (Package & pkg) {
        TelnetChannel::send(pkg) ;
    }
    void TelnetStdIO::send (const char * data, size_t datalen) {

    }
}

#endif
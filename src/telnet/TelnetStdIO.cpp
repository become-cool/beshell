#include "TelnetStdIO.hpp"
#include "Telnet.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/select.h>

#ifdef PLATFORM_LINUX

using namespace std ;

namespace be {
    
    TelnetStdIO::TelnetStdIO(Telnet * telnet)
        : TelnetChannel(telnet)
    {
        disableEcho = true ;
    }

    void TelnetStdIO::setup () {
    }
    void TelnetStdIO::loop () {

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        size_t num_readable = select(STDIN_FILENO  + 1, &readfds, NULL, NULL, &tv);
        if(errno==EINTR) {
            num_readable = select(STDIN_FILENO  + 1, &readfds, NULL, NULL, &tv);
        }
        if (num_readable == 0) {
            return ;
        } else {
            size_t num_bytes = read(STDIN_FILENO, buf, 10240-1);
            if (num_bytes < 0) {
                fprintf(stderr, "\nError on read : %s\n", strerror(errno));
                exit(1);
            }

            buf[num_bytes] = 0 ;

            Package pkg(0,LINE, buf, num_bytes) ;
            telnet->onReceived(this, pkg) ;
        }
    }

    void TelnetStdIO::sendData (const char * data, size_t datalen) {
        cout << std::setw(datalen) << data << flush ;
    }
}

#endif
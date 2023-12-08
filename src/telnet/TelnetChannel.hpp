#pragma once
#include "Protocal.hpp"

namespace be {
    
    class Telnet ;
    // class TelnetChannel ;
	// typedef std::function<void(TelnetChannel * ch, Package & pkg)> TelnetPkgProcFunc;

    class TelnetChannel {
    private:
    protected:
        // TelnetPkgProcFunc packageHandler = nullptr;
        Telnet * telnet = nullptr ;
    public:
        TelnetChannel(Telnet *) ;
        virtual void setup () ;
        virtual void loop () ;
        virtual void send (Package & pkg) ;
        virtual void send (const char * data, size_t datalen) = 0 ;
    } ;
}
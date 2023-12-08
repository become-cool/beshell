#pragma once
#include "Protocal.hpp"

namespace beshell {
    
    class TelnetChannel ;
	typedef std::function<void(TelnetChannel * ch, Package & pkg)> TelnetPkgProcFunc;

    class TelnetChannel {
    private:
    protected:
        TelnetPkgProcFunc packageHandler = nullptr;
    public:
        TelnetChannel(TelnetPkgProcFunc packageHandler=nullptr) ;
        virtual void setup () ;
        virtual void loop () ;
        virtual void send (Package & pkg) ;
        virtual void send (const char * data, size_t datalen) = 0 ;
    } ;
}
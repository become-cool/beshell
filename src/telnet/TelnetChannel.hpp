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
        virtual void setup () = 0 ;
        virtual void loop () = 0 ;
        virtual void send (Package & pkg) = 0 ;
    } ;
}
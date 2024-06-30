#pragma once
#include "TelnetChannel.hpp"
#include "Protocal.hpp"

namespace be {
    class TelnetBLE: public TelnetChannel  {
    private:
        bool setuped = false ;

    public:
        using TelnetChannel::TelnetChannel ;
        
        bool setup(const char * name="BeShell", uint16_t characteristicID=0x0512, uint16_t serviceID=0x0B0C, uint16_t appID=0) ;
        void unsetup() ;
        
        void send (Package & pkg) ;
        void send (const char * data, int datalen=-1, int pkgId=-1, uint8_t cmd=OUTPUT) ;
        void sendData (const char * data, size_t datalen) ;
    } ;
}
#pragma once
#include "TelnetChannel.hpp"
#include "Protocal.hpp"

namespace be {
    class TelnetBLE: public TelnetChannel  {
    private:
        bool setuped = false ;

    public:
        using TelnetChannel::TelnetChannel ;
        
        void setup() ;
        bool setup(const char * name, uint16_t characteristicID, uint16_t serviceID, uint16_t appID) ;
        void unsetup() ;
        
        void send (Package & pkg) ;
        // void send (const char * data, int datalen=-1, int pkgId=-1, uint8_t cmd=OUTPUT) ;
        void sendData (const char * data, size_t datalen) ;
    } ;
}
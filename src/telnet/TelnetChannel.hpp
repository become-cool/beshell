#pragma once
#include "Protocal.hpp"
#ifdef PLATFORM_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#endif

namespace be {
    
    class Telnet ;
    // class TelnetChannel ;
	// typedef std::function<void(TelnetChannel * ch, Package & pkg)> TelnetPkgProcFunc;

    class TelnetChannel {
    private:
#ifdef PLATFORM_ESP32
        SemaphoreHandle_t mutex;
#endif

    protected:
        // TelnetPkgProcFunc packageHandler = nullptr;
        Telnet * telnet = nullptr ;
    public:
        bool disableEcho = true ;

        TelnetChannel(Telnet *) ;
        virtual void setup () ;
        virtual void loop () ;
        virtual void sendData (const char * data, size_t datalen) = 0 ;
        void send (const char * data, int datalen=-1, int pkgId=-1, uint8_t cmd=OUTPUT) ;
        void send (Package & pkg) ;

        virtual bool mutexTake();
        virtual void mutexGive();
    } ;
}
#include "TelnetChannel.hpp"
#include <cstring>
#include "mallocf.h"


namespace be {
    TelnetChannel::TelnetChannel(Telnet * _telnet)
        : telnet(_telnet)
    {
#ifdef PLATFORM_ESP32
        mutex  = xSemaphoreCreateMutex();
#endif
    }
    void TelnetChannel::setup () {}
    void TelnetChannel::loop () {}
    
    void TelnetChannel::send (Package & pkg) {
        // mutexTake() ;
        sendData((const char *)pkg.head.raw, pkg.head_len);
        sendData((const char *)pkg.body(), pkg.body_len);
        sendData((const char *)&pkg.verifysum, 1);
        // mutexGive() ;
    }
    void TelnetChannel::send (const char * data, int datalen, int pkgId, uint8_t cmd) {
        if(datalen<0) {
            datalen = strlen(data) ;
        }
        if(pkgId>=0) {
            Package pkg((uint8_t)pkgId, cmd, (uint8_t *)data, datalen) ;
            pkg.pack() ;
            send(pkg) ;
        } else if(datalen>0){
            // mutexTake() ;
            sendData(data,datalen) ;
            // mutexGive() ;
        }
    }
    
    void TelnetChannel::sendError(int pkgid, const char * format, ...) {
        va_list args;
        va_start( args, format );
        char * message = vstrdupf(format, args) ;
        va_end( args );
        send(message, strlen(message), pkgid, EXCEPTION) ;
        free(message) ;
    }
    
    bool TelnetChannel::mutexTake() {
#ifdef PLATFORM_ESP32
        return xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE ;
#endif
    }
    void TelnetChannel::mutexGive() {
#ifdef PLATFORM_ESP32
        xSemaphoreGive(mutex);
#endif
    }
}
#include "TelnetBLE.hpp"
#include <stdio.h>
#include "debug.h"
#include "Telnet.hpp"
#include "gatt_server.h"

#include <memory>
#include <cassert>
#include <cstring>


namespace be {

    
    static Telnet * global_telnet = nullptr ;
    static TelnetBLE * global_telnet_ble = nullptr ;

    static void gatt_msg_handler (uint8_t *data, size_t size) {
        
        if(size<2) {
            return ;
        }
        
        // print_block(data, size, 16) ;
        // ds(data+2)
        
        assert(global_telnet) ;
        
        std::unique_ptr<Package> pkg = std::make_unique<Package>();
        pkg->head.fields.pkgid = data[0];
        pkg->head.fields.cmd = data[1];
        pkg->body_len = (size-2) ;
        pkg->channle = global_telnet_ble ;


        pkg->mallocBody((uint16_t)pkg->body_len+1, true) ;
        memcpy(pkg->body(), data+2, pkg->body_len) ;

        global_telnet->execPackage(pkg) ;
    }

    bool TelnetBLE::setup(const char * name, uint16_t characteristicID, uint16_t serviceID, uint16_t appID) {
        if(setuped) {
            return false ;
        }

        be_telnet_gatt_server_start(name, characteristicID, serviceID, appID) ;
        be_telnet_gatt_server_set_msg_handler(gatt_msg_handler) ;

        global_telnet = this->telnet ;
        global_telnet_ble = this ;

        setuped = true;
        return true ;
    }

    void TelnetBLE::unsetup() {
    }
    
    void TelnetBLE::sendData (const char * data, size_t datalen) {
        be_telnet_gatt_server_send((uint8_t*)data, datalen, false) ;
    }
    
    void TelnetBLE::send (Package & pkg) {
        uint8_t * buff = (uint8_t *)malloc(pkg.body_len+2) ;
        buff[0] = pkg.head.fields.pkgid ;
        buff[1] = pkg.head.fields.cmd ;
        memcpy(buff+2, pkg.body(), pkg.body_len) ;
        be_telnet_gatt_server_send(buff, pkg.body_len+2, false) ;
        free(buff) ;
    }

    void TelnetBLE::send (const char * data, int datalen, int pkgId, uint8_t cmd) {
        if(pkgId>=0) {
            uint8_t * buff = (uint8_t *)malloc(datalen+2) ;
            buff[0] = (uint8_t)pkgId ;
            buff[1] = cmd ;
            memcpy(buff+2, data, datalen) ;
            be_telnet_gatt_server_send(buff, datalen+2, false) ;
            free(buff) ;
        } else {
            be_telnet_gatt_server_send((uint8_t*)data, datalen, false) ;
        }
    }
}

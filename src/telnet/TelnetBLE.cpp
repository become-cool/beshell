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

    struct {
        int pkgid ;
        uint8_t totalChunks ;
        uint8_t * fullBody ;
    } pendingPackageState = {-1, 0, nullptr} ;


    // forwarding received package to telnet
    static Parser parser([](std::unique_ptr<Package> pkg){
        assert(global_telnet) ;
        pkg->channle = global_telnet_ble ;
        global_telnet->execPackage(pkg) ;
    }) ;

    static void gatt_msg_handler (uint8_t *data, size_t size) {
        // print_block(data, size, 8) ;
        parser.parse(data, size) ;
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
        // print_block((uint8_t*)data, datalen, 8) ;
        if(!data || datalen<=0) {
            return ;
        }
        be_telnet_gatt_server_send((uint8_t*)data, datalen, false) ;
    }
    
    void TelnetBLE::send (Package & pkg) {
        int fulllen = pkg.head_len + pkg.body_len + 1;

        uint8_t * buff = (uint8_t *)malloc(fulllen) ;
        
        memcpy(buff, pkg.head.raw, pkg.head_len) ;
        memcpy(buff+pkg.head_len, pkg.body(), pkg.body_len) ;
        buff[fulllen-1] = pkg.verifysum ;

        be_telnet_gatt_server_send(buff, fulllen, false) ;
        free(buff) ;
    }
}

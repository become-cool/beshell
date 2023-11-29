#include "Telnet.hpp"



namespace beshell {
    Telnet::Telnet() {

    }

    void Telnet::setup() {
        channelSeiral.setup() ;
    }
    void Telnet::loop() {
        channelSeiral.loop() ;
    }
}
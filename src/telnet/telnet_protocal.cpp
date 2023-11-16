#include "telnet_protocal.hpp"
#include <cstdio>


namespace beprotocal {

    Parser::Parser()
        : stateLine(new StateLine(this))
        , statePkg(new StatePkg(this))
    {
        current = stateLine ;
    }
    Parser::~Parser() {
        delete stateLine ;
        delete statePkg ;
    }
    
    void Parser::parse(uint8_t * bytes, size_t len){
        current->parse(bytes,len) ;
    }

    State::State(Parser * parser): parser(parser) {}
    State::~State() {}
    
    // 行数据
    StateLine::StateLine(Parser * parser): State(parser) {
    }
    StateLine::~StateLine() {
        printf("StateLine::~StateLine()\n") ;
    }
    void StateLine::parse(uint8_t * bytes, size_t len) {
        printf("StateLine::parse()\n") ;
    }
    
    // 包数据
    StatePkg::StatePkg(Parser * parser): State(parser) {
    }
    StatePkg::~StatePkg() {
        printf("StatePkg::~StatePkg()\n") ;
    }
    void StatePkg::parse(uint8_t * bytes, size_t len) {
        printf("StatePkg::parse()\n") ;
    }
}


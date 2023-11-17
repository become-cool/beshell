#include "telnet_protocal.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace beprotocal {

    uint8_t verifysum(uint8_t * data, size_t len, uint8_t base) {
        for(uint16_t i=0; i<len; i++) {
            base^= data[i] ;
        }
        return base ;
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
        printf("StateLine::parse()%d\n", len) ;
        
        while(len) {

            for(int i=0;i<len;i++) {
                // 到达行尾
                if( bytes[i] == '\n' ) {
                    pkg_t * pkg = new pkg_t() ;
                    pkg->data_len = pending_len + i+1 ;
                    pkg->data = malloc(pkg->data_len+1) ;
                    memcpy(pkg->data, pending, pending_len) ;
                    memcpy(pkg->data+pending_len, bytes, i+1) ;
                    pkg->data[pkg->data_len-1] = 0 ;

                    bytes+= i+1 ;
                    len-= i+1 ;
                    break ;
                }
                // 遇到包头
                else if( i<len-1 && bytes[i]==parser->H1 && bytes[i+1]==parser->H2 ) {

                }
                // 遇到断开的包头(H1前一次到达)
                else if( bytes[i]==parser->H2 && pending_len>0 && pending[pending_len-1]==parser->H1 ){

                }
            }



        }
    }
    
    uint8_t StateLine::savePendingData(uint8_t * data, size_t len) {
        if(pending_len+len > sizeof(pending)-1) {
            len = sizeof(pending)-1 - pending_len ;
        }
        memcpy(pending+pending_len, data, len) ;
        pending_len+= len ;
    }
    
    // 包数据
    StatePkg::StatePkg(Parser * parser)
        : State(parser)
        , pkg(nullptr)
    {}
    StatePkg::~StatePkg() {
        printf("StatePkg::~StatePkg()\n") ;
    }
    void StatePkg::parse(uint8_t * bytes, size_t len) {
        printf("StatePkg::parse()%d\n", len) ;


    }
    
    // 上下文类
    Parser::Parser(uint8_t _H1,uint8_t _H2)
        : stateLine(new StateLine(this))
        , statePkg(new StatePkg(this,H1,H2))
        , H1(_H1), H2(_H2)
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
    void Parser::setPkgHead(uint8_t _H1,uint8_t _H2) {
        H1 = _H1 ;
        H2 = _H2 ;
    }
}


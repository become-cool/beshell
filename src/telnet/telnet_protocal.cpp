#include "telnet_protocal.hpp"
#include "debug.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define STATE_LINE          parser->stateLine
#define STATE_HEAD_FIXED    parser->statePkgHeadFixed
#define STATE_HEAD_LENGTH   parser->statePkgHeadLength
#define STATE_BODY          parser->statePkgBody

#define CHECK_PKG                                                       \
        if(!parser->pkg) {                                              \
            printf("%s() invalid beprotocal state.\n",__FUNCTION__) ;   \
            parser->changeState(STATE_LINE, nullptr, 0) ;               \
            return ;                                                    \
        }

#define RECEIVE_TO_BUFF_PTR(buff, buffsize, received, data, datalen)  \
        {                                               \
            size_t n = datalen ;                        \
            if(n>buffsize-received) {                   \
                n = buffsize-received ;                 \
            }                                           \
            memcpy(buff+received, data, n) ;            \
            received+= n ;                              \
            datalen-= n ;                               \
            data+= n ;                                  \
        }
#define RECEIVE_TO_BUFF(buff, received, data, datalen)  \
            RECEIVE_TO_BUFF_PTR(buff, sizeof(buff), received, data, datalen)

namespace beprotocal {

    uint8_t verifysum(uint8_t * data, size_t len, uint8_t base) {
        for(uint16_t i=0; i<len; i++) {
            base^= data[i] ;
        }
        return base ;
    }
    
    Package::Package(uint8_t _cmd, uint8_t _pkgid, size_t _data_len)
        : pkgid(_pkgid)
        , cmd(_cmd)
        , data(nullptr)
        , data_len(_data_len)
        , verifysum(0)
    {}

    Package::~Package() {
        if(data) {
            delete data ;
            data = nullptr ;
        }
    }

    State::State(Parser * parser): parser(parser) {}
    State::~State() {}
    void State::enter() {}
    
    // 行数据
    void StateLine::parse(uint8_t * bytes, size_t * len) {
        for(int i=0;i<(*len);i++) {
            // 到达行尾
            if( bytes[i] == '\n' ) {
                Package * pkg = parser->newPackage(LINE,0,received+i+2) ;

                memcpy(pkg->data, buff, received) ;
                memcpy(pkg->data+received, bytes, i+1) ;
                pkg->data[pkg->data_len-1] = 0 ;

                bytes+= i+1 ;
                (*len)-= i+1 ;

                parser->commitPackage() ;
                return ;
            }
            // 遇到包头
            else if( i<(*len)-1 && bytes[i]==parser->H1 && bytes[i+1]==parser->H2 ) {
                printf("bingo2\n") ;
                (*len)-=2 ;
                parser->changeState(STATE_HEAD_FIXED, bytes+2, len) ;
                return ;
            }
            // 遇到断开的包头(H1前一次到达)
            else if( bytes[i]==parser->H2 && received>0 && buff[received-1]==parser->H1 ){
                printf("bingo1\n") ;
                (*len)-=1 ;
                parser->changeState(STATE_HEAD_FIXED, bytes+1, len) ;
                return ;
            }
        }
        
        RECEIVE_TO_BUFF(buff, received, bytes, *len) ;
        (*len) = 0 ;
    }
    
    void StateLine::savebuffData(uint8_t * data, size_t len) {
        if(received+len > sizeof(buff)-1) {
            len = sizeof(buff)-1 - received ;
        }
        memcpy(buff+received, data, len) ;
        received+= len ;
    }
    
    // 包头：固定数据区
    void StatePkgHeadFixed::enter() {
        parser->newPackage(0,0,0) ;
        received = 0 ;
    }
    void StatePkgHeadFixed::parse(uint8_t * bytes, size_t * len) {
        CHECK_PKG
        printf("StatePkgHeadFixed::parse()%d\n", *len) ;

        RECEIVE_TO_BUFF(buff, received, bytes, (*len)) ;

        if( sizeof(buff) == received) {
            parser->pkg->pkgid = buff[0] ;
            parser->pkg->cmd = buff[1] ;
            parser->changeState(STATE_HEAD_LENGTH, bytes, len) ;
        }
    }
    
    // 包头：可变长度区
    void StatePkgHeadLength::enter() {
        CHECK_PKG
        received = 0 ;
    }
    void StatePkgHeadLength::parse(uint8_t * bytes, size_t * len) {
        CHECK_PKG
        printf("StatePkgHeadLength::parse()%d\n", *len) ;

        uint8_t m = sizeof(buff) - received ;
        if(m>(*len)) {
            m = *len ;
        }

        uint8_t complete = false ;
        for(uint8_t i = 0; i<m; i++) {

            buff[received] = bytes[i] ;

            (*len)-= 1 ;
            received ++ ;

            // 遇到第一个小于 0x80 的字节，表示完成
            if(bytes[i]<0x80) {

                parser->pkg->data_len = 0;
                if(received>=1) {
                    parser->pkg->data_len|= buff[0] & 0x7F ;
                }
                if(received>=2) {
                    parser->pkg->data_len|= (buff[1] & 0x7F) << 7 ;
                }
                if(received>=3) {
                    parser->pkg->data_len|= (buff[2] & 0x7F) << 14 ;
                }
                if(received>=4) {
                    parser->pkg->data_len|= (buff[3] & 0x7F) << 21 ;
                }

                dn(parser->pkg->data_len) ;

                parser->changeState(STATE_BODY, bytes+i, len) ;
                return ;
            }
        }
    }
    
    // 包头：可变长度区
    void StatePkgBody::enter() {
        CHECK_PKG
        if(parser->pkg->data_len>0) {
            parser->pkg->data = (uint8_t *)malloc(parser->pkg->data_len) ;
        }
        received = 0 ;
        // verifysum_received = false ;
    }
    void StatePkgBody::parse(uint8_t * bytes, size_t * len) {
        CHECK_PKG

        // body 未完成
        if(parser->pkg->data_len > received) {
            if(parser->pkg->data) {
                RECEIVE_TO_BUFF_PTR(parser->pkg->data, parser->pkg->data_len, received, bytes, (*len)) ;
            }
        }

        // body 已完成
        if(parser->pkg->data_len==received) {
            if((*len)>0) {
                parser->pkg->verifysum = bytes[0] ;
                (*len)-= 1 ;
                bytes+= 1 ;
                if(checkVerifysum()){
                    parser->commitPackage() ;
                }
                else {
                    printf("beprotocal bad verifysum\n") ;
                }
                parser->changeState(STATE_LINE, bytes, len) ;
            }
        }

        else {
            // @unreach
        }
    }

    // 上下文类
    Parser::Parser(PackageProcFunc _handler, uint8_t _H1,uint8_t _H2)
        : stateLine(new StateLine(this))
        , statePkgHeadFixed(new StatePkgHeadFixed(this))
        , statePkgHeadLength(new StatePkgHeadLength(this))
        , statePkgBody(new StatePkgBody(this))
        , handler(_handler)
        , H1(_H1), H2(_H2)
    {
        current = stateLine ;
    }
    Parser::~Parser() {
        delete stateLine ;
        delete statePkgHeadFixed ;
        delete statePkgHeadLength ;
        delete statePkgBody ;
    }
    
    void Parser::parse(uint8_t * bytes, size_t len){
        size_t r = len ;
        while(r>0) {
            current->parse(bytes,&r) ;
            bytes+= len - r ;
        }
    }
    void Parser::setPkgHead(uint8_t _H1,uint8_t _H2) {
        H1 = _H1 ;
        H2 = _H2 ;
    }
    void Parser::setProcessHandler(PackageProcFunc _handler) {
        handler = _handler ;
    }
    void Parser::changeState(State * state, uint8_t * bytes, size_t * len) {
        current = state ;
        current->enter() ;
        // 继续处理
        if(bytes!=nullptr && (*len)>0) {
            current->parse(bytes, len) ;
        }
    }

    Package * Parser::newPackage(uint8_t cmd, uint8_t pkgid, size_t data_len) {
        if(pkg) {
            printf("drop uncompleted package\n") ;
            delete pkg ;
            pkg = nullptr ;
        }
        pkg = new Package(cmd,pkgid,data_len) ;
        if(data_len) {
            pkg->data = (uint8_t *)malloc(data_len) ;
        }
        return pkg ;
    }

    void Parser::commitPackage() {
        if(!pkg || !handler) {
            return ;
        }
        handler(pkg) ;
        delete pkg ;
        pkg = nullptr ;
    }

    void defaultPkgProcFunc(Package * pkg) {
        printf("receive package, pkgid:%d, cmd:%d\n",pkg->pkgid,pkg->cmd) ;
    }

}


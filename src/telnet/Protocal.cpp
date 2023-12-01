#include "Protocal.hpp"
#include "debug.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#define STATE_LINE          parser->stateLine
#define STATE_HEAD_FIXED    parser->statePkgHeadFixed
#define STATE_HEAD_LENGTH   parser->statePkgHeadLength
#define STATE_BODY          parser->statePkgBody


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

namespace beshell {
    
    static uint8_t _verifysum(uint8_t base, uint8_t * data, size_t len) {
        for(uint16_t i=0; i<len; i++) {
            base^= data[i] ;
        }
        return base ;
    }
    
    Package::Package(uint8_t _pkgid, uint8_t _cmd, uint8_t * _body, size_t _body_len)
        : pkgid(_pkgid)
        , cmd(_cmd)
        , body(_body)
        , body_len(_body_len)
        , verifysum(0)
    {}

    Package::~Package() {
        if(body) {
            delete body ;
            body = nullptr ;
        }
    }
    
    void Package::encodeBodyLength() {
        if(body_len<=0xFF) {
            head[4] = 0xFF & body_len ;
            head_len = 5 ;
        }
        else {
            // *(len_bytes+0) = (len>>(7*0)) & 0x7F ;
            // *(len_bytes+1) = (len>>(7*1)) & 0x7F ;
            // *(len_bytes+2) = (len>>(7*2)) & 0x7F ;
            // *(len_bytes+3) = (len>>(7*3)) & 0xFF ;  // 最高位字节可以用满8位

            // if( *(len_bytes+3)>0 ) {
            //     *(len_bytes+0) |= 0x80 ;
            //     *(len_bytes+1) |= 0x80 ;
            //     *(len_bytes+2) |= 0x80 ;
            //     return 4 ;
            // }
            // else if( *(len_bytes+2)>0 ) {
            //     *(len_bytes+0) |= 0x80 ;
            //     *(len_bytes+1) |= 0x80 ;
            //     return 3 ;
            // }
            // else if( *(len_bytes+1)>0 ) {
            //     *(len_bytes+0) |= 0x80 ;
            //     return 2 ;
            // }
            // return 1 ;
        }
    }

    uint8_t Package::calculateVerifysum() {
        uint8_t sum = _verifysum(0,head,head_len) ;
        if(body) {
            sum = _verifysum(sum,body,body_len) ;
        }
        return sum ;
    }
        
    size_t Package::calculateSize() {
        return 0 ;
    }
    void Package::pack() {
    }

    State::State(Parser * parser): parser(parser) {}
    State::~State() {}
    void State::enter() {}

    // 行数据
    void StateLine::parse(uint8_t * bytes, size_t * len) {
        for(int i=0;i<(*len);i++) {
            // 到达行尾
            if( bytes[i] == '\n' ) {
                Package * pkg = parser->newPackage(0,LINE,received+i+2) ;

                memcpy(pkg->body, buff, received) ;
                memcpy(pkg->body+received, bytes, i+1) ;
                pkg->body[pkg->body_len-1] = 0 ;

                bytes+= i+1 ;
                (*len)-= i+1 ;
                received = 0 ;

                parser->commitPackage() ;
                return ;
            }
            // 遇到包头
            else if( i<(*len)-1 && bytes[i]==parser->H1 && bytes[i+1]==parser->H2 ) {
                (*len)-=2 ;
                parser->changeState(STATE_HEAD_FIXED, bytes+i+2, len) ;
                return ;
            }
            // 遇到断开的包头(H1前一次到达)
            else if( bytes[i]==parser->H2 && received>0 && buff[received-1]==parser->H1 ){
                (*len)-=1 ;
                received-=1 ;
                parser->changeState(STATE_HEAD_FIXED, bytes+1, len) ;
                return ;
            }
        }
        RECEIVE_TO_BUFF(buff, received, bytes, *len) ;
        (*len) = 0 ;
    }
    
    void StateLine::saveToBuff(uint8_t * data, size_t len) {
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
        assert(parser->pkg) ;

        RECEIVE_TO_BUFF(buff, received, bytes, (*len)) ;

        if( sizeof(buff) == received) {
            parser->pkg->pkgid = buff[0] ;
            parser->pkg->cmd = buff[1] ;
            parser->pkg->head[2] = buff[0] ;
            parser->pkg->head[3] = buff[1] ;
            parser->changeState(STATE_HEAD_LENGTH, bytes, len) ;
        }
    }
    
    // 包头：可变长度区
    void StatePkgHeadLength::enter() {
        assert(parser->pkg) ;
        received = 0 ;
    }
    void StatePkgHeadLength::parse(uint8_t * bytes, size_t * len) {
        assert(parser->pkg) ;
        if((*len)<1) {
            return ;
        }

        // 固定长度
        if(parser->pkg->head[1]==parser->H2) {
            parser->pkg->head_len = 5 ;
            parser->pkg->head[4] = (*bytes) ;
            parser->pkg->body_len = parser->pkg->head[4] ;
            (*len)-= 1 ;
            parser->changeState(STATE_BODY, bytes+1, len) ;
            return ;
        }

        // 可变长度
        else if(parser->pkg->head[1]==parser->H2V)  {
            uint8_t m = sizeof(buff) - received ;
            if(m>(*len)) {
                m = *len ;
            }

            // dn4(bytes[0],bytes[1],bytes[2],bytes[3])

            uint8_t complete = false ;
            for(uint8_t i = 0; i<m; i++) {

                buff[received] = bytes[i] ;

                (*len)-= 1 ;
                received ++ ;

                // 遇到第一个小于 0x80 的字节，表示完成
                if(bytes[i]<0x80) {

                    parser->pkg->body_len = 0;
                    if(received>=1) {
                        parser->pkg->body_len|= buff[0] & 0x7F ;
                        parser->pkg->head[4] = buff[0] ;
                        parser->pkg->head_len = 5 ;
                    }
                    if(received>=2) {
                        parser->pkg->body_len|= (buff[1] & 0x7F) << 7 ;
                        parser->pkg->head[5] = buff[1] ;
                        parser->pkg->head_len = 6 ;
                    }
                    if(received>=3) {
                        parser->pkg->body_len|= (buff[2] & 0x7F) << 14 ;
                        parser->pkg->head[6] = buff[2] ;
                        parser->pkg->head_len = 7 ;
                    }
                    if(received>=4) {
                        parser->pkg->body_len|= (buff[3] & 0x7F) << 21 ;
                        parser->pkg->head[7] = buff[3] ;
                        parser->pkg->head_len = 8 ;
                    }

                    parser->changeState(STATE_BODY, bytes+i, len) ;
                    return ;
                }
            }
        }
    }
    
    // body
    void StatePkgBody::enter() {
        assert(parser->pkg) ;
        if(parser->pkg->body_len>0) {
            parser->pkg->body = (uint8_t *)malloc(parser->pkg->body_len) ;
        }
        else {
            parser->pkg->body = NULL ;
        }
        received = 0 ;
        // verifysum_received = false ;
    }
    void StatePkgBody::parse(uint8_t * bytes, size_t * len) {
        assert(parser->pkg) ;

        // body 未完成
        if(parser->pkg->body_len > received) {

            size_t n = * len ;
            if(n>parser->pkg->body_len-received) {
                n = parser->pkg->body_len-received ;
            }
            if(parser->pkg->body){
                memcpy(parser->pkg->body+received, bytes, n) ;
            }
            received+= n ;
            * len-= n ;
            bytes+= n ;
        }

        // body 已完成
        if(parser->pkg->body_len==received) {
            if((*len)>0) {
                parser->pkg->verifysum = bytes[0] ;
                (*len)-= 1 ;
                bytes+= 1 ;
                if(checkVerifysum()){
                    parser->commitPackage() ;
                }
                else {
                    // printf("beprotocal bad verifysum\n") ;
                }
                parser->changeState(STATE_LINE, bytes, len) ;
            }
        }
    }

    bool StatePkgBody::checkVerifysum() {
        if(!parser->pkg) {
            return false ;
        }
        return parser->pkg->calculateVerifysum() == parser->pkg->verifysum ;
    }

    // 上下文类
    Parser::Parser(PackageProcFunc _handler, uint8_t _H1,uint8_t _H2,uint8_t _H2V)
        : stateLine(new StateLine(this))
        , statePkgHeadFixed(new StatePkgHeadFixed(this))
        , statePkgHeadLength(new StatePkgHeadLength(this))
        , statePkgBody(new StatePkgBody(this))
        , handler(_handler)
        , H1(_H1), H2(_H2), H2V(_H2V)
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
        assert(state!=NULL);
        current = state ;
        current->enter() ;
        // 继续处理
        if(bytes!=nullptr && (*len)>0) {
            current->parse(bytes, len) ;
        }
    }

    Package * Parser::newPackage(uint8_t pkgid, uint8_t cmd, size_t data_len) {
        if(pkg) {
            delete pkg ;
            pkg = nullptr ;
        }
        pkg = new Package(cmd,pkgid,nullptr,data_len) ;
        pkg->head[0] = H1 ;
        pkg->head[1] = H2 ;
        pkg->head[2] = pkgid ;
        pkg->head[3] = cmd ;
        if(data_len) {
            pkg->body = (uint8_t *)malloc(data_len) ;
        }
        return pkg ;
    }

    void Parser::commitPackage() {
        if(pkg) {
            if(handler){
                handler(pkg) ;
            }
            delete pkg ;
            pkg = nullptr ;
        }
    }

    void defaultPkgProcFunc(Package * pkg) {
        printf("receive package, pkgid:%d, cmd:%d, length:%d\n",pkg->pkgid,pkg->cmd,pkg->body_len) ;
    }

}


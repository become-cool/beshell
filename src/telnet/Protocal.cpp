#include "Protocal.hpp"
#include "debug.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#define STATE_LINE          parser->stateLine
#define STATE_HEAD_FIXED    parser->statePkgHeadFields
#define STATE_HEAD_LENGTH   parser->statePkgBodyLength
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

namespace be {
    
    // Package::Package(uint8_t _pkgid, uint8_t _cmd, uint8_t * _body, size_t _body_len)
    //     : pkgid(_pkgid)
    //     , cmd(_cmd)
    //     , body(_body)
    //     , body_len(_body_len)
    //     , verifysum(0)
    // {}
    Package::Package(uint8_t pkgid, uint8_t cmd, uint8_t * _body, size_t _body_len, uint8_t h1, uint8_t h2)
        : body(_body)
        , body_len(_body_len)
    {
        head.fields.h1 = h1 ;
        head.fields.h1 = h1 ;

        head.fields.h1 = h1 ;
        head.fields.h2 = h2 ;
        reset(pkgid,cmd,body_len) ;
    }

    void Package::reset(uint8_t pkgid, uint8_t cmd,size_t bodylen) {
        head.fields.pkgid = pkgid ;
        head.fields.cmd = cmd ;
        head.fields.len1 = 0 ;
        head.fields.len2 = 0 ;
        head.fields.len3 = 0 ;
        head.fields.len4 = 0 ;
        head_len = 5 ;
        body_len = bodylen ;
        verifysum = 0 ;
    }

    Package::~Package() {}

    size_t Package::decodeBodyLength() {
        // body_len = 0;
        // if(head.fields.len1>=1) {
        //     body_len|= buff[0] & 0x7F ;
        //     head.fields.len1 = buff[0] ;
        //     head_len = 5 ;
        // }
        // if(received>=2) {
        //     body_len|= (buff[1] & 0x7F) << 7 ;
        //     head.fields.len2 = buff[1] ;
        //     head_len = 6 ;
        // }
        // if(received>=3) {
        //     body_len|= (buff[2] & 0x7F) << 14 ;
        //     head.fields.len3 = buff[2] ;
        //     head_len = 7 ;
        // }
        // if(received>=4) {
        //     body_len|= (buff[3] & 0x7F) << 21 ;
        //     head.fields.len4 = buff[3] ;
        //     head_len = 8 ;
        // }
        return 0 ;
    }
    
    void Package::encodeBodyLength() {
        if(body_len<=0xFF) {
            head.fields.len1 = 0xFF & body_len ;
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

    void Package::mallocBody(bool endZero) {
        if(body) {
            freeBody() ;
        }
        int len = body_len ;
        if(endZero) {
            len = len + 1 ;
        }
        if(len) {
            body = (uint8_t *)malloc(len) ;
            if(body && endZero) {
                body[len-1] = 0 ;
            }
        }
    }
    void Package::freeBody() {
        if(body) {
            free((void *)body) ;
            body = nullptr ;
        }
    }
    
    static uint8_t _verifysum(uint8_t base, uint8_t * data, size_t len) {
        for(uint16_t i=0; i<len; i++) {
            base^= data[i] ;
        }
        return base ;
    }
    uint8_t Package::calculateVerifysum() {
        uint8_t sum = _verifysum(0,head.raw,head_len) ;
        if(body) {
            sum = _verifysum(sum,body,body_len) ;
        }
        return sum ;
    }
        
    size_t Package::calculateSize() {
        return 0 ;
    }

    void Package::pack() {
        encodeBodyLength() ;
        verifysum = calculateVerifysum() ;
    }

    State::State(Parser * parser): parser(parser) {}
    State::~State() {}
    void State::enter() {}

    bool State::receiveToBuff(uint8_t ** bytes, size_t * len, uint8_t * buff, size_t buffsize) {
        
        if(buffsize == received) {
            return true ;
        }

        size_t n = * len ;
        if(n>buffsize-received) {
            n = buffsize-received ;
        }
        memcpy(buff+received, * bytes, n) ;

        received+= n ;
        * len-= n ;
        * bytes+= n ;

        return buffsize == received ;
    }

    // 行数据
    void StateLine::parse(uint8_t * bytes, size_t * len) {
        
        // 遇到断开的包头(H1在前一次数据的末尾到达)
        if( bytes[0]==parser->H2 && received>0 && buff[received-1]==parser->H1 ){
            (*len)-=1 ;
            received-=1 ;
            parser->changeState(STATE_HEAD_FIXED, bytes+1, len) ;
            return ;
        }

        for(int i=0;i<(*len);i++) {
            // 到达行尾
            if( bytes[i] == '\n' ) {

                parser->pkg.reset(0, LINE, received+i+1) ;
                parser->pkg.mallocBody(true) ;
                // Package * pkg = parser->newPackage(LINE,0,received+i+2) ;

                memcpy(parser->pkg.body, buff, received) ;
                memcpy(parser->pkg.body+received, bytes, i+1) ;

                // printf((char *)parser->pkg.body) ;

                bytes+= i+1 ;
                (*len)-= i+1 ;
                received = 0 ;

                parser->commitPackage() ;
                return ;
            }
            // 遇到包头
            else if( i<(*len)-1 && bytes[i]==parser->H1 && bytes[i+1]==parser->H2 ) {
                (*len)-=i+2 ;
                bytes+=i+2 ;
                parser->changeState(STATE_HEAD_FIXED, bytes, len) ;
                return ;
            }
        }
        RECEIVE_TO_BUFF(buff, received, bytes, *len) ;
        (*len) = 0 ;
    }
    
    
    // 包头：固定数据区
    void StatePkgHeadFields::enter() {
        parser->pkg.reset(0,0,0) ;
        received = 0 ;
    }
    void StatePkgHeadFields::parse(uint8_t * bytes, size_t * len) {
        if( receiveToBuff(&bytes, len, parser->pkg.head.raw+2, 2) ) {
            parser->changeState(STATE_HEAD_LENGTH, bytes, len) ;
        }
    }
    
    // 包头：可变长度区
    void StatePkgBodyLength::enter() {
        received = 0 ;
    }
    void StatePkgBodyLength::parse(uint8_t * bytes, size_t * len) {
        // assert(parser->pkg) ;
        if((*len)<1) {
            return ;
        }

        // 固定长度
        if(parser->pkg.head.fields.h2==parser->H2) {
            parser->pkg.head_len = 5 ;
            parser->pkg.head.fields.len1 = (*bytes) ;
            parser->pkg.body_len = parser->pkg.head.fields.len1 ;
            (*len)-= 1 ;
            parser->changeState(STATE_BODY, bytes+1, len) ;
            return ;
        }

        // 可变长度
        // else if(parser->pkg.head.fields.h2==parser->H2V)  {
        //     uint8_t m = 4 - received ;
        //     if(m>(*len)) {
        //         m = *len ;
        //     }

        //     // dn4(bytes[0],bytes[1],bytes[2],bytes[3])

        //     uint8_t complete = false ;
        //     for(uint8_t i = 0; i<m; i++) {

        //         parser->pkg.head.raw[4+received] = bytes[i] ;

        //         (*len)-= 1 ;
        //         received ++ ;

        //         // 遇到第一个小于 0x80 的字节，表示完成
        //         if(bytes[i]<0x80) {
        //             parser->pkg.decodeBodyLength() ;
        //             parser->changeState(STATE_BODY, bytes+i, len) ;
        //             return ;
        //         }
        //     }
        // }
    }
    
    // body
    void StatePkgBody::enter() {
        if(parser->pkg.body_len>0) {
            // parser->pkg.body = (uint8_t *)malloc(parser->pkg.body_len) ;
            parser->pkg.mallocBody(true) ;
        }
        else {
            parser->pkg.body = NULL ;
        }
        received = 0 ;
        // verifysum_received = false ;
    }
    void StatePkgBody::parse(uint8_t * bytes, size_t * len) {
        // assert(parser->pkg) ;

        // body 未完成
        if(parser->pkg.body_len > received) {

            size_t n = * len ;
            if(n>parser->pkg.body_len-received) {
                n = parser->pkg.body_len-received ;
            }
            if(parser->pkg.body){
                memcpy(parser->pkg.body+received, bytes, n) ;
            }
            received+= n ;
            * len-= n ;
            bytes+= n ;
        }

        // body 已完成
        if(parser->pkg.body_len==received) {
            if((*len)>0) {
                parser->pkg.verifysum = bytes[0] ;
                (*len)-= 1 ;
                bytes+= 1 ;
                if(checkVerifysum()){
                    parser->commitPackage() ;
                }
                else {
                    printf("beprotocal bad verifysum %d!=%d(received)\n", parser->pkg.calculateVerifysum(), parser->pkg.verifysum) ;
                }
                parser->changeState(STATE_LINE, bytes, len) ;
            }
        }
    }

    bool StatePkgBody::checkVerifysum() {
        return parser->pkg.calculateVerifysum() == parser->pkg.verifysum ;
    }

    // 上下文类
    Parser::Parser(PackageProcFunc _handler, uint8_t _H1,uint8_t _H2)
        : pkg(0,0, nullptr, 0, _H1, _H2)
        , stateLine(new StateLine(this))
        , statePkgHeadFields(new StatePkgHeadFields(this))
        , statePkgBodyLength(new StatePkgBodyLength(this))
        , statePkgBody(new StatePkgBody(this))
        , handler(_handler)
        , H1(_H1), H2(_H2)
    {
        current = stateLine ;
    }
    Parser::~Parser() {
        delete stateLine ;
        delete statePkgHeadFields ;
        delete statePkgBodyLength ;
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

    void Parser::commitPackage() {
        if(handler){
            handler(pkg) ;
        }
    }
    
    // void Parser::setStreamHandle(StreamCreateFunc handler) {
    //     streamHandler = handler ;
    // }

    void defaultPkgProcFunc(Package & pkg) {
        printf("receive package, pkgid:%d, cmd:%d, length:%d\n",pkg.head.fields.pkgid,pkg.head.fields.cmd,pkg.body_len) ;
    }
}
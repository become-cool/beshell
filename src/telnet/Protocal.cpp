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

using namespace std ;

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
    
    Package::Package(uint8_t pkgid, uint8_t cmd, uint8_t * __body, size_t _body_len, uint8_t h1, uint8_t h2)
        : body_len(_body_len)
    {
        head.fields.h1 = h1 ;
        head.fields.h2 = h2 ;
        reset(pkgid,cmd,body_len) ;

        if(__body) {
            _body = __body ;
            sharedBody = true ;
        }
    }
    Package::Package(const Package * other)
        : _body(other->_body)
        , head_len(other->head_len)
        , body_len(other->body_len)
        , chunk_len(other->chunk_len)
        , verifysum(other->verifysum)
    {
        std::memcpy(head.raw, other->head.raw, sizeof(head.raw)) ;
    }
    Package::~Package() {
        if(!sharedBody && _body) {
            delete _body ;
        }
    }

    uint8_t * Package::body() const {
        return _body ;
    }

    void Package::reset(uint8_t pkgid, uint8_t cmd,size_t bodylen) {
        head.fields.pkgid = pkgid ;
        head.fields.cmd = cmd ;
        if(bodylen>0xFF) {
            head.fields.cmd|= 0x80 ;
        }
        body_len = bodylen ;
        encodeBodyLength() ;
        verifysum = 0 ;
    }
    
    void Package::encodeBodyLength() {
        if(body_len>0xFF) {
            head.fields.cmd|= 0x80 ;
            head.fields.len1 = (body_len>>8) & 0xFF ;
            head.fields.len2 = body_len & 0xFF ;
            head_len = 6 ;
        } else {
            head.fields.len1 = body_len & 0xFF ;
            head.fields.len2 = 0 ;
            head_len = 5 ;
        }
    }


    void Package::mallocBody(uint16_t len, bool endZero) {
        if(_body && !sharedBody) {
            delete _body ;
            _body = nullptr ;
        }

        if(len) {
            _body = new uint8_t[len] ;
            if(endZero) {
                _body[len-1] = 0 ;
            }
            sharedBody = false ;
        }
    }

    uint8_t Package::calculateVerifysum(uint8_t * data, size_t datalen, uint8_t baseval) {
        for(uint16_t i=0; i<datalen; i++) {
            baseval^= data[i] ;
        }
        return baseval ;
    }
    uint8_t Package::calculateVerifysum() {
        uint8_t sum = Package::calculateVerifysum(head.raw,head_len) ;
        return (_body&&body_len)? Package::calculateVerifysum(_body,body_len,sum): sum ;
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

    // line data (separate by character '\ n')
    void StateLine::parse(uint8_t * bytes, size_t * len) {
        
        // encountered identification bytes that arrived separately (H1 reached at end of previous data)
        if( bytes[0]==parser->H2 && received>0 && buff[received-1]==parser->H1 ){
            (*len)-=1 ;
            received-=1 ;
            parser->changeState(STATE_HEAD_FIXED, bytes+1, len) ;
            return ;
        }

        for(int i=0;i<(int)(*len);i++) {
            // reaching end of line
            if( bytes[i] == '\n' ) {

                parser->pkg->reset(0, LINE, received+i+1) ;
                parser->pkg->mallocBody(parser->pkg->body_len+1, true) ;

                if(parser->pkg->body()) {
                    memcpy(parser->pkg->body(), buff, received) ;
                    memcpy(parser->pkg->body()+received, bytes, i+1) ;
                    parser->pkg->body()[parser->pkg->body_len] = 0 ;
                }

                bytes+= i+1 ;
                (*len)-= i+1 ;
                received = 0 ;

                parser->commitPackage() ;
                return ;
            }
            // encountered packet identifier bytes
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
    
    
    // packet head：fixed length data segment
    void StatePkgHeadFields::enter() {
        parser->pkg->reset(0,0,0) ;
        received = 0 ;
    }
    void StatePkgHeadFields::parse(uint8_t * bytes, size_t * len) {
        if( receiveToBuff(&bytes, len, parser->pkg->head.raw+2, 2) ) {
            parser->changeState(STATE_HEAD_LENGTH, bytes, len) ;
        }
    }
    
    // packet head：variable length data segment
    void StatePkgBodyLength::enter() {
        received = 0 ;
    }
    void StatePkgBodyLength::parse(uint8_t * bytes, size_t * len) {
        size_t lenBytes = parser->pkg->head.fields.cmd<0x80? 1: 2 ;
        if( !receiveToBuff(&bytes, len, &parser->pkg->head.fields.len1, lenBytes) ){
            return ;
        }

        if(lenBytes==1) {
            parser->pkg->head_len = 5 ;
            parser->pkg->body_len = parser->pkg->head.fields.len1 ;
        } else  {
            parser->pkg->head_len = 6 ;
            parser->pkg->body_len = (parser->pkg->head.fields.len1 << 8) | parser->pkg->head.fields.len2 ;
        }
        
        parser->changeState(STATE_BODY, bytes, len) ;
    }
    
    // body
    void StatePkgBody::enter() {
        received = 0 ;
        if(parser->pkg->body_len>0 && parser->pkg->body_len<=0xFF) {
            parser->pkg->mallocBody(parser->pkg->body_len+1, true) ;
        }
        else {
            parser->pkg->_body = nullptr ;
        }

        verifysum = Package::calculateVerifysum(parser->pkg->head.raw, parser->pkg->head_len, 0) ;
    }
    void StatePkgBody::parse(uint8_t * bytes, size_t * len) {
        // _body incomplete
        if( received < parser->pkg->body_len ) {

            size_t n = * len ;
            if(n>parser->pkg->body_len-received) {
                n = parser->pkg->body_len-received ;
            }
            // body length greater than 0xff
            if( parser->pkg->body_len>0xFF ) {
                if(parser->handler){
                    // commit sub packet (body is only partial data)
                    Package * chunk = new Package(parser->pkg) ;

                    chunk->mallocBody(n,false) ;
                    chunk->chunk_len = n ;
                    std::memcpy(chunk->_body, bytes, n);

                    chunk->head.fields.cmd &= 0x7F ;

                    parser->handler(std::unique_ptr<Package>(chunk)) ;
                }
            }

            else if(parser->pkg->body()) {
                memcpy(parser->pkg->body()+received, bytes, n) ;
            }
            
            verifysum = Package::calculateVerifysum(bytes, n, verifysum) ;

            received+= n ;
            * len-= n ;
            bytes+= n ;
        }

        // _body completed
        if( received == parser->pkg->body_len ) {
            if((*len)>0) {
                parser->pkg->verifysum = bytes[0] ;
                (*len)-= 1 ;
                bytes+= 1 ;

                if(verifysum==parser->pkg->verifysum){
                    parser->commitPackage() ;
                }
                else {
                    // printf("be::protocal bad verifysum %d!=%d(received)\n", verifysum, parser->pkg->verifysum) ;
                }

                parser->changeState(STATE_LINE, bytes, len) ;
            }
        }
    }


    // context class
    Parser::Parser(PackageProcFunc _handler, uint8_t _H1,uint8_t _H2)
        : pkg( new Package(0,0, nullptr, 0, _H1, _H2) )
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

        // continue processing unused data
        if(bytes!=nullptr && (*len)>0) {
            current->parse(bytes, len) ;
        }
    }

    void Parser::commitPackage() {
        if(handler){
            pkg->head.fields.cmd &= 0x7F ;
            handler(std::unique_ptr<Package>(pkg)) ;
            pkg = new Package(0,0,nullptr,0,H1,H2) ;
        }
    }

    void defaultPkgProcFunc(std::unique_ptr<Package> pkg) {
        printf("receive package, pkgid:%d, cmd:%d, length:%d\n",pkg->head.fields.pkgid,pkg->head.fields.cmd,pkg->body_len) ;
    }
}
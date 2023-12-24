#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include <cassert>
#include <string.h>
#include <sys/stat.h>

using namespace std ;

namespace be {
    Telnet::Telnet(BeShell * _beshell)
        : beshell(_beshell)
#ifdef PLATFORM_ESP32
        , channelSeiral(this)
#endif
#ifdef PLATFORM_LINUX
        , channelStdIO(this)
#endif
    {}

    void Telnet::setup() {
#ifdef PLATFORM_ESP32
        channelSeiral.setup() ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.setup() ;
#endif
    }
    void Telnet::loop() {
#ifdef PLATFORM_ESP32
        channelSeiral.loop() ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.loop() ;
#endif
    }

    void Telnet::onReceived(TelnetChannel * ch, std::unique_ptr<Package> pkg){
        switch (pkg->head.fields.cmd)
        {
        case LINE:
        case RUN:
        case CALL:
            assert(beshell) ;
            beshell->repl->input(*pkg, ch) ;
            break;
        case FILE_OPEN:
            openFile(ch,pkg,false) ;
            break ;
        case FILE_OPEN_APPEND:
            openFile(ch,pkg,true) ;
            break ;
        case FILE_OFFSET:
            offsetFile(ch,pkg) ;
            break ;
        case FILE_PUSH:
            pushFile(ch,pkg) ;
            break;
        case FILE_CLOSE:
            closeFile(ch,pkg) ;
            break;
        case FILE_PULL:
            pullFile(ch,pkg) ;
            break;
        default: 
            ch->sendError(pkg->head.fields.pkgid, "cmd %d not implements", pkg->head.fields.cmd) ;
            break;
        }
    }

#define CHECK_FS_USED       \
        if(!beshell->fs) {  \
            ch->sendError(pkg->head.fields.pkgid, "Do not use FS.") ; \
            return ;        \
        }

    void Telnet::output(const char * data, size_t datalen, int pkgid, uint8_t cmd) {
        if(pkgid<0) {
            pkgid = autoIncreasePkgId ++ ;
        }
        else {
            pkgid%= 255 ;
        }
        Package pkg((uint8_t)pkgid,cmd,(uint8_t*)data,datalen) ;
        pkg.pack() ;

#ifdef PLATFORM_ESP32
        channelSeiral.send(pkg) ;
#endif
#ifdef PLATFORM_LINUX
        channelStdIO.send(data,datalen) ;
#endif
    }

    TelnetChannel * Telnet::channel(const char * name) {
#ifdef PLATFORM_ESP32
        if(strcmp(name,"serial")==0){
            return & channelSeiral ;
        }
#endif
#ifdef PLATFORM_LINUX
        if(strcmp(name,"stdio")==0){
            return & channelStdIO ;
        }
#endif
        return nullptr ;
    }
    void Telnet::openFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg, bool append) {
        CHECK_FS_USED

        if( pkg->body()[pkg->body_len-1]!=0 ) {
            ch->sendError(pkg->head.fields.pkgid, "Invalid path value(must be null ending)") ;
            return ;
        }

        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }
        
        const char * cpath = (const char *)pkg->body() ;
        string path = beshell->fs->toVFSPath(cpath) ;

        ch->openedFile = fopen(path.c_str(), append?"a+":"w+") ;

        if(ch->openedFile) {
            ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
        } else {
            ch->sendError(pkg->head.fields.pkgid, "can not open file: %s", cpath) ;
        }
    }
    void Telnet::offsetFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        CHECK_FS_USED
        ch->sendError(pkg->head.fields.pkgid, "cmd not implements") ;
    }
    void Telnet::closeFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        CHECK_FS_USED

        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }

        ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
    }
    
    void Telnet::pushFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        CHECK_FS_USED

        if(!ch->openedFile) {
            ch->sendError(pkg->head.fields.pkgid, "file not opened") ;
            return ;
        }

        if(pkg->body()) {
            // 分段数据包
            if(pkg->chunk_len && pkg->body_len>0xFF) {
                size_t writen = fwrite(pkg->body(),1,pkg->chunk_len,ch->openedFile) ;
                // dn(writen)
            }
            // 完整包
            else {
                size_t writen = fwrite(pkg->body(),1,pkg->body_len,ch->openedFile) ;
                // dn(writen)
                ch->send(nullptr, 0, pkg->head.fields.pkgid, RSPN) ;
            }
            fflush(ch->openedFile) ;
        }

        // 最后一个空包
        else {
            ch->send(nullptr, 0, pkg->head.fields.pkgid, RSPN) ;
        }
    }
    void Telnet::pullFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        CHECK_FS_USED

        const char * cpath = (const char *)pkg->body() ;
        string path = beshell->fs->toVFSPath(cpath) ;

        int pathlen = strlen(cpath) + 1 ;
        
        if( pathlen+6 != pkg->body_len ) {
            ch->sendError(pkg->head.fields.pkgid, "body length invalid") ;
            return ;
        }
        
        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)!=0) {
            ch->sendError(pkg->head.fields.pkgid, "file not exists") ;
            return ;
        }
        if(!S_ISREG(statbuf.st_mode)) {
            ch->sendError(pkg->head.fields.pkgid, "path is not a file") ;
            return ;
        }
        
        uint8_t * argptr = pkg->body()+pathlen ;

        size_t offset = (argptr[0]<<24) | (argptr[1]<<16) | (argptr[2]<<8) | argptr[3] ;
        uint16_t length = (argptr[4]<<8) | argptr[5] ;

        if(offset>=statbuf.st_size) {
            ch->sendError(pkg->head.fields.pkgid, "invalid arg offset") ;
            return ;
        }

        if(length>statbuf.st_size-offset) {
            length = statbuf.st_size-offset ;
        }
        if(length>0xFFFF) {
            length=0xFFFF ;
        }
        
        FILE * h = fopen(path.c_str(), "r") ;
        if(!h) {
            ch->sendError(pkg->head.fields.pkgid, "can not open file") ;
            return ;
        }

        Package rspnpkg(pkg->head.fields.pkgid, DATA, nullptr, length) ;
        uint8_t verifysum = Package::calculateVerifysum(rspnpkg.head.raw,(size_t)rspnpkg.head_len) ;

        ch->sendData((const char *)rspnpkg.head.raw,(size_t)rspnpkg.head_len) ;
        // dn4( rspnpkg.head.fields.cmd, rspnpkg.head.fields.len1, rspnpkg.head.fields.len2, rspnpkg.head_len )

        uint8_t data[256];
        while(length>0) {
            size_t chunklen = length>sizeof(data)? sizeof(data): length ;
            size_t readed = fread(data, 1, chunklen, h);
            // dn2(chunklen,readed)
            length-= chunklen ;
            ch->sendData((const char *)data,readed) ;
            verifysum = Package::calculateVerifysum(data,readed,verifysum) ;
        }

        fclose(h) ;

        // dn(verifysum)
        ch->sendData((const char *)&verifysum,1) ;
    }
}
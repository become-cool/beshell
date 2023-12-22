#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include <cassert>
#include <string.h>


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

    void Telnet::onReceived(TelnetChannel * ch, Package & pkg){
        switch (pkg.head.fields.cmd)
        {
        case LINE:
        case RUN:
        case CALL:
            assert(beshell) ;
            beshell->repl->input(pkg, ch) ;
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
            pushFile(ch,pkg) ;
            break;
        case FILE_PULL:
            pullFile(ch,pkg) ;
            break;
        default: 
            ch->sendError(pkg.head.fields.pkgid, "cmd %d not implements", pkg.head.fields.cmd) ;
            break;
        }
    }

#define CHECK_FS_USED       \
        if(!beshell->fs) {  \
            ch->sendError(pkg.head.fields.pkgid, "Do not use FS.") ; \
            return ;        \
        }

    void Telnet::output(const char * data, size_t datalen, int pkgid, uint8_t cmd) {
        if(pkgid<0) {
            pkgid = autoIncreasePkgId ++ ;
        }
        else {
            pkgid%= 255 ;
        }
        Package pkg((uint8_t)pkgid,cmd,(uint8_t *)data,datalen) ;
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
    void Telnet::openFile(TelnetChannel * ch, Package & pkg, bool append) {

        CHECK_FS_USED

        if( pkg.body[pkg.body_len-1]!=0 ) {
            ch->sendError(pkg.head.fields.pkgid, "Invalid path value(must be null ending)") ;
            return ;
        }

        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }
        
        const char * cpath = (const char *)pkg.body ;
        string path = beshell->fs->toVFSPath(cpath) ;

        cout << path << endl ;

        ch->openedFile = fopen(path.c_str(), append?"a+":"w+") ;
        dp(ch->openedFile)

        if(ch->openedFile) {
            ch->send(nullptr,0,pkg.head.fields.pkgid,RSPN) ;
        } else {
            ch->sendError(pkg.head.fields.pkgid, "can not open file: %s", cpath) ;
        }
    }
    void Telnet::offsetFile(TelnetChannel * ch, Package & pkg) {
        CHECK_FS_USED
        ch->sendError(pkg.head.fields.pkgid, "cmd not implements") ;
    }
    void Telnet::closeFile(TelnetChannel * ch, Package & pkg) {
        CHECK_FS_USED

        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }

        ch->send(nullptr,0,pkg.head.fields.pkgid,RSPN) ;
    }
    
    void Telnet::pushFile(TelnetChannel * ch, Package & pkg) {
        CHECK_FS_USED

        if(!ch->openedFile) {
            ch->sendError(pkg.head.fields.pkgid, "file not opened") ;
            return ;
        }

        if(pkg.body_len) {
            assert(pkg.body) ;
            dp(ch->openedFile) ;
            size_t writen = fwrite(pkg.body,1,pkg.body_len,ch->openedFile) ;
            dn(writen)
            fflush(ch->openedFile) ;
            fclose(ch->openedFile) ;
        }

        ch->send(nullptr, 0, pkg.head.fields.pkgid, RSPN) ;
    }
    void Telnet::pullFile(TelnetChannel * ch, Package & pkg) {

        CHECK_FS_USED

        const char * cpath = (const char *)pkg.body ;
        string path = beshell->fs->toVFSPath(cpath) ;

        dn(pkg.body_len)
    }

    // std::unique_ptr<std::ostream> Telnet::createStream(Package & pkg) {
    //     if( pkg.head.fields.cmd==FILE_PUSH
    //         || pkg.head.fields.cmd==FILE_PUSH_APPEND
    //         || pkg.head.fields.cmd==FILE_PUSH_OFFSET) {
    //         return new fs
    //     }
    //     return nullptr ;
    // }
}
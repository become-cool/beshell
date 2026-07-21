#include "REPL.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include "../module/Process.hpp"
#include <cassert>
#include <string.h>
#include <sys/stat.h>
#include "mbedtls/aes.h"

#ifdef ESP_PLATFORM
#include "REPLModule.hpp"
#endif


#define PKG_QUEUE_LEN 64

using namespace std ;

namespace be {

    REPL::REPL(BeShell * beshell)
        : beshell(beshell)
#ifdef ESP_PLATFORM
        , channelSeiral(this)
#endif
#ifdef LINUX_PLATFORM
        , channelStdIO(this)
#endif
    {
#ifdef ESP_PLATFORM
        addChannel(&channelSeiral) ;
#endif
#ifdef LINUX_PLATFORM
        addChannel(&channelStdIO) ;
#endif
    }

    void REPL::setup() {
        pkg_queue = xQueueCreate(PKG_QUEUE_LEN, sizeof(Package *));
        for(auto ch: channels) {
            ch->setup() ;
        }
    }

    void REPL::execPackage(std::unique_ptr<Package> & pkg) {
        Package * ptr = pkg.release() ;
        xQueueSend(pkg_queue, &ptr, 0) ;
    }

    void REPL::defaultREPLDecryptFunc(Package & pkg) {

        if(!pkg.body_len) {
            return ;
        }

        int datalen = pkg.chunk_len ? pkg.chunk_len : pkg.body_len ;
        
        uint8_t decrypted_data[datalen] ;
        uint8_t counter[sizeof(cryptoVI)] ;
        memcpy(counter, cryptoVI, sizeof(cryptoVI)) ;

        mbedtls_aes_context aes_ctx;
        unsigned char stream_block[16]; // Block size for AES
        size_t nc_off = 0; // Offset in the current stream block

        // Initialize AES context
        mbedtls_aes_init(&aes_ctx);

        // Set encryption key (CTR mode uses the same key for encryption and decryption)
        if (mbedtls_aes_setkey_enc(&aes_ctx, cryptoKey, 128) != 0) {
            return ;
        }

        // Perform AES-CTR decryption
        if (mbedtls_aes_crypt_ctr(&aes_ctx, datalen, &nc_off, counter, stream_block, pkg.body(), decrypted_data) != 0) {
            return ;
        }

        // Free AES context
        mbedtls_aes_free(&aes_ctx);

        memcpy(pkg.body(), decrypted_data, datalen) ;
    }

    void REPL::onReceived(REPLChannel * ch, std::unique_ptr<Package> pkg){
        if(enableCrypto) {
            if(decryptFunc) {
                decryptFunc(*pkg) ;
            }
            else {
                defaultREPLDecryptFunc(*pkg) ;
            }
        }
        
        switch (pkg->head.fields.cmd)
        {
        case LINE:
        case RUN:
        case CALL:
            assert(beshell) ;
            if(beshell->repl) {
                beshell->cammonds->input(*pkg, ch) ;
            } else {
                cout << "call useREPL() first" << endl ;
            }
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

        case SYNC:
            if(pkg->body_len==8) {
                uint8_t * p = pkg->body() ;
                uint64_t ms = ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32)
                            | ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] << 8)  | (uint64_t)p[7] ;
                beshell->engine->timer.setTime(ms) ;
            }
            ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
            break;
        case PING:
            {
                int64_t ms = gettime() ;
                uint8_t data[8] ;
                data[0] = (ms >> 56) & 0xFF ;
                data[1] = (ms >> 48) & 0xFF ;
                data[2] = (ms >> 40) & 0xFF ;
                data[3] = (ms >> 32) & 0xFF ;
                data[4] = (ms >> 24) & 0xFF ;
                data[5] = (ms >> 16) & 0xFF ;
                data[6] = (ms >> 8) & 0xFF ;
                data[7] = ms & 0xFF ;
                ch->send((const char *)data, 8, pkg->head.fields.pkgid, PONG) ;
            }
            break;

        default:
            if(unknownPkgHook) {
                unknownPkgHook(ch, std::move(pkg)) ;
            }
            else {
                ch->sendError(pkg->head.fields.pkgid, "cmd %d not implements", pkg->head.fields.cmd) ;
            }
            break;
        }
    }

    void REPL::setCryptoFunction(REPLDecryptFunc decryptFunc) {
        this->decryptFunc = decryptFunc ;
    }

    void REPL::output(const char * data, size_t datalen, int pkgid, uint8_t cmd) {

        pkgid%= 255 ;

        Package pkg((uint8_t)pkgid,cmd,(uint8_t*)data,datalen) ;
        pkg.pack() ;
        
        for(auto ch: channels) {
            ch->send(pkg) ;
        }
    }

    void REPL::output(const std::string & data, int pkgid, uint8_t cmd) {
        output(data.c_str(), data.length(), pkgid, cmd) ;
    }

    REPLChannel * REPL::channel(const char * name) {
#ifdef ESP_PLATFORM
        if(strcmp(name,"serial")==0){
            return & channelSeiral ;
        }
        if(strcmp(name,"bt")==0){
            return channelBLE ;
        }
#endif
#ifdef LINUX_PLATFORM
        if(strcmp(name,"stdio")==0){
            return & channelStdIO ;
        }
#endif
        return nullptr ;
    }
    void REPL::setBLEChannel(REPLChannel * ch) {
        if(channelBLE) {
            removeChannel(channelBLE) ;
        }
        channelBLE = ch ;
        if(channelBLE) {
            addChannel(channelBLE) ;
        }
    }
    void REPL::openFile(REPLChannel * ch, std::unique_ptr<Package> & pkg, bool append) {
        
        if( pkg->body()[pkg->body_len-1]!=0 ) {
            ch->sendError(pkg->head.fields.pkgid, "Invalid path value(must be null ending)") ;
            return ;
        }

        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }
        
        const char * cpath = (const char *)pkg->body() ;
        string path = FS::toVFSPath(cpath) ;

        ch->openedFile = fopen(path.c_str(), append?"a+":"w+") ;

        if(ch->openedFile) {
            ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
        } else {
            ch->sendError(pkg->head.fields.pkgid, "can not open file: %s", cpath) ;
        }
    }
    void REPL::offsetFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) {
        ch->sendError(pkg->head.fields.pkgid, "cmd not implements") ;
    }
    void REPL::closeFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) {
        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }
        ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
    }
    
    void REPL::pushFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) {
        if(!ch->openedFile) {
            ch->sendError(pkg->head.fields.pkgid, "file not opened") ;
            return ;
        }

        if(pkg->body()) {
            // 分段数据包
            if(pkg->chunk_len) {
                necho_time("write file",{
                    fwrite(pkg->body(),1,pkg->chunk_len,ch->openedFile) ;
                })
            }
            // 完整包
            else {
                fwrite(pkg->body(),1,pkg->body_len,ch->openedFile) ;
                ch->send(nullptr, 0, pkg->head.fields.pkgid, RSPN) ;
            }
            fflush(ch->openedFile) ;
        }

        // 最后一个空包
        else {
            ch->send(nullptr, 0, pkg->head.fields.pkgid, RSPN) ;
        }
    }
    void REPL::pullFile(REPLChannel * ch, std::unique_ptr<Package> & pkg) {
        
        const char * cpath = (const char *)pkg->body() ;
        int pathlen = strlen(cpath) + 1 ;
        
        if( pathlen+6 != (int)pkg->body_len ) {
            // dn2(pathlen, pkg->body_len)
            ch->sendError(pkg->head.fields.pkgid, "body length invalid") ;
            return ;
        }

        string path = FS::toVFSPath(cpath) ;
        // dstr(path)
        
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

        if(offset>=(size_t)statbuf.st_size) {
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

        Package rspnpkg(pkg->head.fields.pkgid, FILE_PULL_DATA, nullptr, length) ;
        uint8_t verifysum = Package::calculateVerifysum(rspnpkg.head.raw,(size_t)rspnpkg.head_len) ;

        ch->sendData((const char *)rspnpkg.head.raw,(size_t)rspnpkg.head_len) ;
        // dn4( rspnpkg.head.fields.cmd, rspnpkg.head.fields.len1, rspnpkg.head.fields.len2, rspnpkg.head_len )
// dn(length)
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

    void REPL::addChannel(REPLChannel * ch) {
        channels.push_back(ch) ;
    }
    void REPL::removeChannel(REPLChannel * ch) {
        channels.erase(std::remove(channels.begin(), channels.end(), ch), channels.end()) ;
    }
}

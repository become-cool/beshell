#include "Telnet.hpp"
#include "debug.h"
#include "BeShell.hpp"
#include <cassert>
#include <string.h>
#include <sys/stat.h>
#include "mbedtls/aes.h"

#ifdef ESP_PLATFORM
#include "TelnetModule.hpp"
#endif


#define PKG_QUEUE_LEN 64

using namespace std ;

namespace be {

    Telnet::Telnet(BeShell * beshell)
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

    void Telnet::setup() {
        pkg_queue = xQueueCreate(PKG_QUEUE_LEN, sizeof(Package *));
        for(auto ch: channels) {
            ch->setup() ;
        }
    }

    void Telnet::execPackage(std::unique_ptr<Package> & pkg) {
        Package * ptr = pkg.release() ;
        xQueueSend(pkg_queue, &ptr, 0) ;
    }

    void Telnet::defaultTelnetDecryptFunc(Package & pkg) {

        if(!pkg.body_len) {
            return ;
        }
        
        uint8_t decrypted_data[pkg.body_len] ;
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
        if (mbedtls_aes_crypt_ctr(&aes_ctx, pkg.body_len, &nc_off, counter, stream_block, pkg.body(), decrypted_data) != 0) {
            return ;
        }

        // Free AES context
        mbedtls_aes_free(&aes_ctx);

        memcpy(pkg.body(), decrypted_data, pkg.body_len) ;
    }

    void Telnet::onReceived(TelnetChannel * ch, std::unique_ptr<Package> pkg){

        if(enableCrypto) {
            if(decryptFunc) {
                decryptFunc(*pkg) ;
            }
            else {
                defaultTelnetDecryptFunc(*pkg) ;
            }
        }

        switch (pkg->head.fields.cmd)
        {
        case LINE:
        case RUN:
        case CALL:
            assert(beshell) ;
            if(beshell->repl) {
                beshell->repl->input(*pkg, ch) ;
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
        default: 
            ch->sendError(pkg->head.fields.pkgid, "cmd %d not implements", pkg->head.fields.cmd) ;
            break;
        }
    }

    void Telnet::setCryptoFunction(TelnetDecryptFunc decryptFunc) {
        this->decryptFunc = decryptFunc ;
    }

    void Telnet::output(const char * data, size_t datalen, int pkgid, uint8_t cmd) {
        // if(pkgid<0) {
            // channelSeiral.send(data,datalen) ;
        // }
        // else {
            pkgid%= 255 ;

            Package pkg((uint8_t)pkgid,cmd,(uint8_t*)data,datalen) ;
            pkg.pack() ;
            
            for(auto ch: channels) {
                ch->send(pkg) ;
            }
        // }

// #ifdef LINUX_PLATFORM
//         channelStdIO.send(data,datalen) ;
// #endif
    }

    void Telnet::output(const std::string & data, int pkgid, uint8_t cmd) {
        output(data.c_str(), data.length(), pkgid, cmd) ;
    }

    TelnetChannel * Telnet::channel(const char * name) {
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
    void Telnet::openFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg, bool append) {
        
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
    void Telnet::offsetFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        ch->sendError(pkg->head.fields.pkgid, "cmd not implements") ;
    }
    void Telnet::closeFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        if(ch->openedFile) {
            fclose(ch->openedFile) ;
            ch->openedFile = nullptr ;
        }
        ch->send(nullptr,0,pkg->head.fields.pkgid,RSPN) ;
    }
    
    void Telnet::pushFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        
        if(!ch->openedFile) {
            ch->sendError(pkg->head.fields.pkgid, "file not opened") ;
            return ;
        }

        if(pkg->body()) {
            // 分段数据包
            if(pkg->chunk_len && pkg->body_len>0xFF) {
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
    void Telnet::pullFile(TelnetChannel * ch, std::unique_ptr<Package> & pkg) {
        
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

    void Telnet::addChannel(TelnetChannel * ch) {
        channels.push_back(ch) ;
    }
    void Telnet::removeChannel(TelnetChannel * ch) {
        channels.erase(std::remove(channels.begin(), channels.end(), ch), channels.end()) ;
    }
}
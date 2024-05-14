#pragma once
#include "NativeModule.hpp"

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "deps/quickjs/quickjs.h"
#include "FSPartition.hpp"


namespace be {
    class FS: public NativeModule {
    private:

#ifdef ESP_PLATFORM
        static std::map<std::string, FSPartition *> partitions;
        static std::string pwd ;
#endif
        static std::string prefix ;
        static bool inited ;

    protected:
    public:
    
        static const char * name ;

        // for JS
        FS(JSContext * ctx, const char * name,uint8_t flagGlobal=1);
        
        static JSValue mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue statSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue existsSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue seek(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        
        // for c++
        static void setPrefix(const char * path) ;
        static std::string toVFSPath(const char * path) ;
        static std::string & toVFSPath(std::string & path) ;
        static std::string toVFSPath(JSContext * ctx, JSValue path) ;
        static std::string trimVFSPath(const std::string & path) ;

        static void mount(const char * mountPoint, FSPartition * partition) ;
        static void unmountAll() ;

        static bool setCwd(const std::string &) ;
        static std::string cwd() ;
        static std::string resolve(const std::string & path) ;

        static bool exist(const char * path) ;
        static bool isDir(const char * path) ;
        static bool isFile(const char * path) ;
        static bool rm(const char * path, bool recursive=false) ;
        static bool mkdir(const char * path, bool recursive=false) ;
        static bool touch(const char * path) ;

        static std::unique_ptr<char> readFileSync(const char * path, int * readed=nullptr, int offset=0, int readlen=-1) ;
        static bool writeFileSync(const char * path, const char * data, size_t len, bool append=false) ;
    } ;
}
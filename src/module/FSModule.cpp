#include "module/FSModule.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "FSModule.hpp"
#include "fs/FS.hpp"
#include "utils.h"
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>
#include <cassert>

using namespace std ;


namespace be {
    FSModule::FSModule(): NativeModule("fs") {
        isReplGlobal = true ;

        exportFunction("mkdirSync", mkdirSync) ;
        exportFunction("rmdirSync", rmdirSync) ;
        exportFunction("unlinkSync", unlinkSync) ;
        exportFunction("readFileSync", readFileSync) ;
        exportFunction("writeFileSync", writeFileSync) ;
        exportFunction("listDirSync", listDirSync) ;
        exportFunction("rmSync", rmSync) ;
        exportFunction("renameSync", renameSync) ;
        exportFunction("info", info) ;
    }

    #define FETCH_FS                                                \
            FS * fs = JSEngine::fromJSContext(ctx)->beshell->fs ;   \
            if(!fs) {                                               \
                THROW_EXCEPTION("call BeShell::USE_FS() first")     \
            }
    #define ARGV_PATH(var, i)                                       \
            string var ;                                            \
            {                                                       \
                const char * cpath = JS_ToCString(ctx, argv[i]) ;   \
                var = fs->toVFSPath(cpath) ;                        \
                JS_FreeCString(ctx, cpath) ;                        \
            }

    #define STAT_PATH(path)                                                             \
        struct stat statbuf;                                                            \
        if(stat(path.c_str(),&statbuf)<0) {                                             \
            JS_ThrowReferenceError(ctx, "Failed to stat file %s", path.c_str());        \
            return JS_EXCEPTION ;                                                       \
        }

    #define CHECK_IS_NOT_DIR(path)                                                      \
        STAT_PATH(path)                                                                 \
        if(S_ISDIR(statbuf.st_mode)) {                                                  \
            JS_ThrowReferenceError(ctx, "Path is a directory %s", path.c_str());        \
            return JS_EXCEPTION ;                                                       \
        }

    /**
     * 同步创建目录
     * 
     * @beapi beapi.fs.mkdirSync
     * @param path:string 路径
     * @param recursive:bool=false 路径
     * @return bool
     */
    JSValue FSModule::mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)

        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
        }

        bool recursive = false ;
        if(argc>1) {
            recursive = JS_ToBool(ctx, argv[1]) ;
        }

        bool ret = false ;
        if(recursive) {
#if defined(WIN32)
            // ret = mkdir_p(path.c_str())>=0 ;
#else
            // ret = mkdir_p(path.c_str(), ACCESSPERMS)>=0 ;
#endif
        }
        else {
#if defined(WIN32)
            ret = mkdir(path.c_str())>=0 ;
#else
            ret = mkdir(path.c_str(), ACCESSPERMS)>=0 ;
#endif
        }
        return ret? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步删除目录。如果目录非空返回 false。
     * 
     * @beapi beapi.fs.rmdirSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FSModule::rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)
        return (rmdir(path.c_str())>=0)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步删除文件
     * 
     * @beapi beapi.fs.unlinkSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FSModule::unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)
        return (unlink(path.c_str())>=0)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步读取文件
     * 
     * > 函数返回的是一个 ArrayBuffer 对象，可以使用 ArrayBuffer.prototype.asString() 方法转换为字符串，例如：
     * > ```
     * > let content = beapi.fs.readFileSync("~/foo.bar").asString()
     * > console.log(content)
     * > ```
     * 
     * @beapi beapi.fs.readFileSync
     * @param path:string 路径
     * @param readlen:number=-1 读取长度，-1表示全文
     * @param offset:number=0 开始位置
     * @return ArrayBuffer
     */
    JSValue FSModule::readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)

        CHECK_IS_NOT_DIR(path)

        int32_t readlen = -1 ;
        if(argc>1) {
            JS_ToInt32(ctx, &readlen, argv[1]) ;
        }
        int offset = 0 ;
        if(argc>2) {
            JS_ToInt32(ctx, (int32_t*)&offset, argv[2]) ;
        }

        if(readlen<0) {
            readlen = statbuf.st_size ;
        }

        if(readlen<1){
            return JS_NewArrayBuffer(ctx, NULL, 0, freeArrayBuffer, NULL, false) ;
        }

        char * buff = (char *)malloc((int)readlen) ;

        if(!buff) {
            THROW_EXCEPTION("Failed to malloc buff");
        }

        FILE * fd = fopen(path.c_str(), "r");

        if(NULL==fd) {
            free(buff) ;
            THROW_EXCEPTION("Failed to open file (%d).", errno);
        }
        if(offset>0) {
            fseek(fd, offset, SEEK_SET) ;
        }

        int readedBytes = fread(buff, 1, readlen, fd) ;
        fclose(fd) ;

        return JS_NewArrayBuffer(ctx, (uint8_t *)buff, readedBytes, freeArrayBuffer, NULL, false) ;
    }


    /**
     * 同步写入文件，返回写入字节数量
     * 
     * @beapi beapi.fs.writeFileSync
     * @param path:string 路径
     * @param data:string|ArrayBuffer 数据内容
     * @param append:bool=false 是否追加写入
     * @return number
     */
    JSValue FSModule::writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

        CHECK_ARGC(2)
        FETCH_FS
        ARGV_PATH(path, 0)

        bool append = false ;

        // 文件已经存在
        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)>=0) {
            CHECK_IS_NOT_DIR(path)
            // offset
            if(argc>2) {
                append = JS_ToBool(ctx, argv[2]) ;
            }
        }

        FILE * fd = fopen(path.c_str(), append? "a+": "w");
        if(NULL==fd) {
            JS_ThrowReferenceError(ctx, "Failed to open file %s", path.c_str());
            return JS_EXCEPTION ;
        }

        const char * buff = NULL ;
        size_t length;
        bool buffIsString = JS_IsString(argv[1]) ;

        if(buffIsString) {
            buff = JS_ToCString(ctx, argv[1]) ;
            length = strlen(buff) ;
        }

        else {
            buff = (const char *)JS_GetArrayBuffer(ctx, &length, argv[1]) ;
            if(!buff || !length) {
                fclose(fd) ;
                JS_ThrowReferenceError(ctx, "arg data is invalid type or empty");
                return JS_EXCEPTION ;
            }
        }

        int wroteBytes = 0 ;
        if(buff) {
            // printf("write(%d): %s\n", length, buff) ;
            wroteBytes = fwrite(buff, 1, length, fd);
        }

        fclose(fd) ;
        if(buffIsString) {
            JS_FreeCString(ctx, buff) ;
        }
        else {
            // @todo
            // free ArrayBuffer
        }

        return JS_NewInt32(ctx, wroteBytes) ;
    }

    /**
     * 同步读取目录下的所有成员。
     * 如果 detail 参数为 false，仅返回文件名数字。
     * 
     * @beapi beapi.fs.readdirSync
     * @param path:string 路径
     * @param detail:bool=false 是否范围详细信息
     * @return string[]|{name:string, type:"file"|"dir"|"unknown", size:number}[]
     */
    JSValue FSModule::listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)

        DIR* dir = opendir(path.c_str());
        string childpath ;

        bool detail = false ;
        if(argc>1) {
            detail = JS_ToBool(ctx, argv[1]) ;
        }

        if(!dir) {
            JS_ThrowReferenceError(ctx, "Cound not open dir %s", path.c_str());
            return JS_EXCEPTION ;
        }
        
        struct dirent *dirEnt;
        JSValue ret = JS_NewArray(ctx) ;
        // uint64_t t = gettime(), tt = 0 ;
        int idx = 0 ;
        struct stat statbuf;
        while((dirEnt = readdir(dir))) {
                // tt = gettime() ;
                // printf("readdir() time: %lld\n", tt-t) ;
                // t = tt ;

            if(detail) {

                JSValue item = JS_NewObject(ctx) ;
                JS_SetPropertyUint32(ctx, ret, idx++, item) ;
                JS_SetPropertyStr(ctx, item, "name", JS_NewString(ctx, dirEnt->d_name) ) ;

                childpath = path + "/" + dirEnt->d_name ;
                
                necho_time("stat",{
                    if(stat(childpath.c_str(),&statbuf)!=0) {
                        JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "unknow") ) ;
                    }
                    else {
                        if(S_ISDIR(statbuf.st_mode)) {
                            JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "dir") ) ;
                        }
                        else {
                            JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "file") ) ;
                            JS_SetPropertyStr(ctx, item, "size", JS_NewUint32(ctx, statbuf.st_size) ) ;
                        }
                    }
                })

            } else {
                JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, dirEnt->d_name)) ;
            }
        }


#ifdef PLATFORM_ESP32
        // esp32 vfs 读取目录时，忽略了挂载点，将已知挂载点补充上
        if(strcmp(path.c_str(),"/fs/")==0 || strcmp(path.c_str(),"/fs")==0) {
            if(detail) {
                JSValue item = JS_NewObject(ctx) ;
                JS_SetPropertyStr(ctx, item, "name", JS_NewString(ctx, "home") ) ;
                JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "dir") ) ;
                JS_SetPropertyUint32(ctx, ret, idx++, item) ;
            }
            else {
                JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, "home")) ;

            }
        }
#endif

        closedir(dir);
        return ret ;
    }

    /**
     * 同步删除文件
     * 
     * @param path {string} 路径
     * @return bool
     */
    JSValue FSModule::rmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(path, 0)

        bool recursive = false ;
        if(argc>1) {
            recursive = JS_ToBool(ctx, argv[1]) ;
        }

        bool ret = fs->rm(path.c_str(), recursive) ;

        return ret? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步重命名文件
     * 操作成功，返回 0
     * 如果错误，返回错误代码
     * 
     * @param path {string} 路径
     * @param name {string} 名称
     * @return number
     */
    JSValue FSModule::renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

        CHECK_ARGC(1)
        FETCH_FS
        ARGV_PATH(oldpath, 0)
        ARGV_PATH(newpath, 0)

        int ret = rename(oldpath.c_str(), newpath.c_str()) ;
        return JS_NewInt32(ctx, ret) ;
    }

    /**
     * 文件分区的信息
     * 返回的对象包括了分区的总大小，和已用大小
     * 
     * @param path {string} 分区路径
     * @return {total:number, used:number}
     */
    JSValue FSModule::info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        CHECK_ARGC(1)
        ARGV_TO_CSTRING(0, jslabel)

        size_t total = 0 ;
        size_t used = 0 ;
        size_t free_ = 0 ;

// #ifdef PLATFORM_ESP32
//         if(strcmp(jslabel,"/home")==0){
//             // littlefs
//             if( esp_littlefs_info(PARTITION_LABEL_HOME, &total, &used) != ESP_OK ) {
//                 JS_FreeCString(ctx, jslabel) ;
//                 THROW_EXCEPTION("esp_littlefs_info() bad")
//             }

//             // fatfs
//             // size_t free_ = 0 ;
//             // esp_err_t err = esp_vfs_fat_info(jslabel, &total, &free_) ;
//             // if( err != ESP_OK ) {
//             //     JS_ThrowReferenceError(ctx, "unknow mount point: %s, (%d)", jslabel, err) ;
//             //     JS_FreeCString(ctx, jslabel) ;
//             //     return JS_EXCEPTION ;
//             // }
//             // used = total - free_ ;
//         }
//         // else if(strcmp(jslabel,"/")==0) {
//         //     vfs_rawfs_t * rootfs = be_rawfs_root() ;
//         //     if(fs) {
//         //         used = rootfs->size ;
//         //         total = rootfs->partition_size ;
//         //     }
//         // }
//         else {
// #if ESP_IDF_VERSION_MAJOR >= 5
//             esp_err_t err = esp_vfs_fat_info(jslabel, &total, &free_) ;
//             if( err != ESP_OK ) {
//                 JS_ThrowReferenceError(ctx, "unknow mount point: %s, (%d)", jslabel, err) ;
//                 JS_FreeCString(ctx, jslabel) ;
//                 return JS_EXCEPTION ;
//             }
//             used = total - free_ ;
// #else
//             total = 0 ;
//             used = 0 ;
//             free_ = 0 ;
// #endif
//         }



// #endif
        JS_FreeCString(ctx, jslabel) ;

        JSValue obj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "total", JS_NewUint32(ctx, total));
        JS_SetPropertyStr(ctx, obj, "used", JS_NewUint32(ctx, used));
        // JS_SetPropertyStr(ctx, obj, "free", JS_NewUint32(ctx, free_));

        return obj ;
    }

}
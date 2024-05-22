#include "FS.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "FS.hpp"
#include "fs/FS.hpp"
#include "qjs_utils.h"
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>
#include <cassert>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../deps/littlefs/src/esp_littlefs.h"

using namespace std ;


namespace be {

    FS::FS(JSContext * ctx, const char * name,uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
        EXPORT_FUNCTION(mkdirSync) ;
        EXPORT_FUNCTION(rmdirSync) ;
        EXPORT_FUNCTION(unlinkSync) ;
        EXPORT_FUNCTION(readFileSync) ;
        EXPORT_FUNCTION(writeFileSync) ;
        EXPORT_FUNCTION(listDirSync) ;
        EXPORT_FUNCTION(rmSync) ;
        EXPORT_FUNCTION(renameSync) ;
        EXPORT_FUNCTION(info) ;
        EXPORT_FUNCTION(statSync) ;
        EXPORT_FUNCTION(existsSync) ;
        EXPORT_FUNCTION(isFileSync) ;
        EXPORT_FUNCTION(isDirSync) ;
        EXPORT_FUNCTION(open) ;
        EXPORT_FUNCTION(read) ;
        EXPORT_FUNCTION(write) ;
        EXPORT_FUNCTION(seek) ;
        EXPORT_FUNCTION(sync) ;
        EXPORT_FUNCTION(flush) ;
        EXPORT_FUNCTION(close) ;
    }

    #define ARGV_PATH(var, i)                                       \
            string var ;                                            \
            {                                                       \
                const char * cpath = JS_ToCString(ctx, argv[i]) ;   \
                var = FS::toVFSPath(cpath) ;                        \
                JS_FreeCString(ctx, cpath) ;                        \
            }

    #define STAT_PATH(path)                                                             \
        struct stat statbuf;                                                            \
        if(stat(path.c_str(),&statbuf)<0) {                                             \
            JS_ThrowReferenceError(ctx, "Failed to stat file %s", FS::trimVFSPath(path).c_str()); \
            return JS_EXCEPTION ;                                                       \
        }

    #define CHECK_IS_NOT_DIR(path)                                                      \
        STAT_PATH(path)                                                                 \
        if(S_ISDIR(statbuf.st_mode)) {                                                  \
            JS_ThrowReferenceError(ctx, "Path is a directory %s", FS::trimVFSPath(path).c_str()); \
            return JS_EXCEPTION ;                                                       \
        }

    /**
     * 同步创建目录
     * 
     * @function mkdirSync
     * @param path:string 路径
     * @param recursive:bool=false 是否递归创建目录
     * @return bool
     */
    JSValue FS:: mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)

        bool recursive = false ;
        if(argc>1) {
            recursive = JS_ToBool(ctx, argv[1]) ;
        }
        return FS::mkdir(path.c_str(), recursive)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步删除目录。如果目录非空返回 false。
     * 
     * @function rmdirSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FS:: rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        bool recursive = false ;
        if(argc>1) {
            recursive = JS_ToBool(ctx, argv[1]) ;
        }
        return FS::rm(path.c_str(), recursive)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步删除文件
     * 
     * @function unlinkSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FS:: unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return (unlink(path.c_str())>=0)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步读取文件
     * 
     * > 函数返回的是一个 ArrayBuffer 对象，可以使用 ArrayBuffer.prototype.asString() 方法转换为字符串，例如：
     * > ```
     * > let content = fs.readFileSync("~/foo.bar").asString()
     * > console.log(content)
     * > ```
     * 
     * @function readFileSync
     * @param path:string 路径
     * @param readlen:number=-1 读取长度，-1表示全文
     * @param offset:number=0 开始位置
     * @return ArrayBuffer
     */
    JSValue FS:: readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        
        ASSERT_ARGC(1)
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
            JSTHROW("Failed to malloc buff");
        }

        FILE * fd = fopen(path.c_str(), "r");

        if(NULL==fd) {
            free(buff) ;
            JSTHROW("Failed to open file (%d).", errno);
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
     * @function writeFileSync
     * @param path:string 路径
     * @param data:string|ArrayBuffer 数据内容
     * @param append:bool=false 是否追加写入
     * @return number
     */
    JSValue FS:: writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

        ASSERT_ARGC(2)
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
            JS_ThrowReferenceError(ctx, "Failed to open file %s", FS::trimVFSPath(path).c_str());
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
     * 
     * 如果 detail 参数为 false，仅返回文件名数字。
     * 
     * 如果 detail 参数为 true，返回一个数组，数组元素为对象，对象格式如下：
     * 
     * ```js
     * [
     *     {
     *         name: string ,
     *         type: "file"|"dir"|"unknown" ,
     *         size: number
     *     } ,
     *     ...
     * ]
     * ```
     * 
     * @function readdirSync
     * @param path:string 路径
     * @param detail:bool=false 是否范围详细信息
     * @return string[] | object[]
     */
    JSValue FS:: listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)

        DIR* dir = opendir(path.c_str());
        string childpath ;

        bool detail = false ;
        if(argc>1) {
            detail = JS_ToBool(ctx, argv[1]) ;
        }

        if(!dir) {
            JS_ThrowReferenceError(ctx, "Cound not open dir %s", FS::trimVFSPath(path).c_str());
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


#ifdef ESP_PLATFORM
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
    JSValue FS:: rmSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)

        bool recursive = false ;
        if(argc>1) {
            recursive = JS_ToBool(ctx, argv[1]) ;
        }

        bool ret = FS::rm(path.c_str(), recursive) ;

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
    JSValue FS:: renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

        ASSERT_ARGC(1)
        ARGV_PATH(oldpath, 0)
        ARGV_PATH(newpath, 1)

        int ret = rename(oldpath.c_str(), newpath.c_str()) ;
        return JS_NewInt32(ctx, ret) ;
    }


    /**
     * 同步返回文件状态
     * 如果文件不存在,返回 null; 否则返回包含详细信息的对象
     * 
     * 返回对象格式:
     * ```js
     * {
     *     dev:number,
     *     ino:number,
     *     mode:number,
     *     size:number,
     *     isDir:bool,
     *     atime:number,
     *     mtime:number,
     *     ctime:number
     * }
     * ```
     * 
     * @import fs
     * @function statSync
     * @param path:string 文件路径
     * @return null | object
     */
    JSValue FS:: statSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)<0) {
            return JS_NULL ;
        }
        
        JSValue obj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "dev", JS_NewInt32(ctx, statbuf.st_dev)) ;
        JS_SetPropertyStr(ctx, obj, "ino", JS_NewInt32(ctx, statbuf.st_ino)) ;
        JS_SetPropertyStr(ctx, obj, "mode", JS_NewInt32(ctx, statbuf.st_mode)) ;
        if(S_ISDIR(statbuf.st_mode)) {
            JS_SetPropertyStr(ctx, obj, "isDir", JS_TRUE) ;
        }
        else {
            JS_SetPropertyStr(ctx, obj, "isDir", JS_FALSE) ;
        }
        JS_SetPropertyStr(ctx, obj, "size", JS_NewInt32(ctx, statbuf.st_size)) ;
        JS_SetPropertyStr(ctx, obj, "atime", JS_NewInt64(ctx, statbuf.st_atime)) ;
        JS_SetPropertyStr(ctx, obj, "mtime", JS_NewInt64(ctx, statbuf.st_mtime)) ;
        JS_SetPropertyStr(ctx, obj, "ctime", JS_NewInt64(ctx, statbuf.st_ctime)) ;

        return obj ;
    }


    /**
     * 同步返回路径是否存在
     * 
     * @import fs
     * @function existsSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FS:: existsSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::exist(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步返回路径是否存在
     * 
     * @import fs
     * @function isFileSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FS:: isFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::isFile(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步返回路径是否存在
     * 
     * @import fs
     * @function isDirSync
     * @param path:string 路径
     * @return bool
     */
    JSValue FS::isDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::isDir(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 文件分区的信息
     * 返回的对象包括了分区的总大小，和已用大小
     * 
     * 返回对象格式：
     * ```js
     * { total:number, used:number }
     * ```
     * 
     * @param path {string} 分区路径
     * @return object
     */
    JSValue FS:: info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(1)
        ARGV_TO_CSTRING(0, jslabel)

        size_t total = 0 ;
        size_t used = 0 ;
        size_t free_ = 0 ;

#ifdef ESP_PLATFORM
#if ESP_IDF_VERSION_MAJOR >= 5
        esp_err_t err = esp_littlefs_info(jslabel, &total, &free_) ;
        if( err != ESP_OK ) {
            JS_ThrowReferenceError(ctx, "unknow mount point: %s, (%d)", jslabel, err) ;
            JS_FreeCString(ctx, jslabel) ;
            return JS_EXCEPTION ;
        }
        used = total - free_ ;
#else
        total = 0 ;
        used = 0 ;
        free_ = 0 ;
#endif

#endif
        JS_FreeCString(ctx, jslabel) ;

        JSValue obj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "total", JS_NewUint32(ctx, total));
        JS_SetPropertyStr(ctx, obj, "used", JS_NewUint32(ctx, used));
        JS_SetPropertyStr(ctx, obj, "free", JS_NewUint32(ctx, free_));

        return obj ;
    }

    
    /**
     * 打开文件，返回文件句柄，用于后续操作
     * 
     * @function open
     * @param path string 文件路径
     * @param mode="rw" string 打开方式
     * @return number
     */
    JSValue FS::open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        string ARGV_TO_PATH(0, path)
        string ARGV_TO_STRING_OPT(1, mode, "rw")

        FILE * handle = fopen(path.c_str(), mode.c_str()) ;
        if(!handle) {
            JSTHROW("Failed to open file %s", path.c_str()) ;
        }

        return JS_NewInt64(ctx, (int64_t)handle) ;
    }

    /**
     * 从文件中读指定长度的数据, 如果遇到错误抛出异常
     * 
     * ```js
     * import {open,read,close} from "fs"
     * 
     * const handle = open("/sdcard/test.txt", "w") ;
     * const data = read(handle, 10) ;
     * console.log("readed string:",data.toString()) ;
     * close(handle) ;
     * ```
     * 
     * @function read
     * @param handle number 由open()返回的文件句柄
     * @param length number 读取长度
     * @return ArrayBuffer
     */
    JSValue FS::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_INT64(0, handle)
        ARGV_TO_UINT32(1, length)

        void * buff = malloc(length) ;
        if(!buff){
            JSTHROW("Failed to malloc %d bytes", length) ;
        }
        int readed = fread(buff, 1, length, (FILE*)handle) ;
        return JS_NewArrayBuffer(ctx, (uint8_t*)buff, readed, freeArrayBuffer, NULL, false) ;
    }

    /**
     * 向文件写入数据,返回写入的字节数
     * 
     * ```js
     * import {open,write,close} from "fs"
     * 
     * const handle = open("/sdcard/test.txt", "w") ;
     * let wrote = write(handle, "hello world") ;
     * console.log("wrote bytes:",wrote) ;
     * close(handle) ;
     * ```
     * 
     * @function write
     * @param handle number 由open()返回的文件句柄
     * @param data ArrayBuffer|string 写入的数据
     * @return number 写入的字节数
     */
    JSValue FS::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_INT64(0, handle)

        
        size_t length = 0 ;
        uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[1]) ;

        // if ArrayBuffer
        if(buff && length) {
            const int wrote = fwrite(buff, 1, length, (FILE*)handle) ;
            return JS_NewInt32(ctx, wrote) ;
        }
        // as string
        else {
            ARGV_TO_CSTRING_LEN(1, buff, length)
            const int wrote = fwrite(buff, 1, length, (FILE*)handle) ;
            JS_FreeCString(ctx, (const char *)buff) ;
            return JS_NewInt32(ctx, wrote) ;
        }
    }

    /**
     * 移动文件指针到指定位置，影响 read/write 操作
     * 
     * @function seek
     * @param handle number 由open()返回的文件句柄
     * @param offset number 偏移量
     * @param whence 0|1|2 0:从文件头开始，1:从当前位置开始，2:从文件尾开始
     * @return number 返回0表示成功，非0表示失败原因
     */
    JSValue FS::seek(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_INT64(0, handle)
        ARGV_TO_UINT32(1, offset)
        ARGV_TO_INT32_OPT(2, whence, SEEK_SET)
        return JS_NewInt32(ctx, fseek((FILE*)handle, offset, whence)) ;
    }
    
    /**
     * 刷新文件缓冲区，将数据写入磁盘 (esp32 平台上没有实际效果)
     * 
     * @function flush
     * @param handle number 由open()返回的文件句柄
     * @return number 返回0表示成功，非0表示失败原因
     */
    JSValue FS::flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        return JS_NewInt32(ctx, fflush((FILE*)handle)) ;
    }
    
    /**
     * 将数据立即写入存储介质（esp32 平台上没有实际效果）
     * 
     * @function sync
     * @param handle number 由open()返回的文件句柄
     * @return number 返回0表示成功，非0表示失败原因
     */
    JSValue FS::sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        int fd = fileno((FILE*)handle);
        return JS_NewInt32(ctx, fsync(fd)) ;
    }

    /**
     * 关闭 open 打开的文件句柄
     * 
     * @function close
     * @param handle number 由open()返回的文件句柄
     * @return number 返回0表示成功，非0表示失败原因
     */
    JSValue FS::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        return JS_NewInt32(ctx, fclose((FILE*)handle)) ;
    }
}
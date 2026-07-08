/**
 * FS 文件系统模块
 * 
 * 提供文件系统操作功能，支持在 Flash 分区和 SD 卡上创建和使用文件系统。
 * 
 * ## 支持的文件系统格式
 * 
 * BeShell 支持多种文件系统格式：
 * 
 * | 文件系统 | 适用场景 | 特点 |
 * |---------|---------|------|
 * | **LittleFS** | Flash 分区 | **推荐**。专为嵌入式设备设计，具有断电安全保护、磨损均衡 |
 * | FATFS | Flash 分区 / SD 卡 | 兼容性好，适合与 PC 交换数据 |
 * | SPIFFS | Flash 分区 | 旧版支持，不推荐新项目使用 |
 * 
 * ### 嵌入式设备推荐使用 LittleFS
 * 
 * 1. **断电安全**：LittleFS 具有断电安全保护机制，写操作过程中断电不会损坏文件系统
 * 2. **磨损均衡**：自动均衡 Flash 各区块的擦写次数，延长 Flash 寿命
 * 3. **小文件优化**：特别适合存储配置文件、脚本等小文件
 * 4. **无需格式化**：首次挂载时自动格式化，简化部署流程
 * 
 * ### FATFS 适用场景
 * 
 * - SD 卡文件系统（与 PC 交换数据）
 * - 需要与电脑直接读取存储内容的场景
 * 
 * ## 使用示例
 * 
 * ### 基本文件操作
 * ```javascript
 * import * as fs from "fs"
 * 
 * // 写入文件（根目录 / 对应 Flash 文件系统）
 * fs.writeFileSync("/config.json", '{"wifi": {"ssid": "mywifi"}}')
 * 
 * // 读取文件
 * const data = fs.readFileSync("/config.json")
 * const text = new TextDecoder().decode(data)
 * console.log(text)
 * 
 * // 创建目录
 * fs.mkdirSync("/data", true)  // true 表示递归创建
 * 
 * // 列出根目录内容
 * const files = fs.listDirSync("/")
 * console.log(files)
 * 
 * // 检查文件是否存在
 * if (fs.existsSync("/config.json")) {
 *     console.log("文件存在")
 * }
 * 
 * // 获取文件信息
 * const stat = fs.statSync("/config.json")
 * console.log("大小:", stat.size, "字节")
 * console.log("是目录:", stat.isDir)
 * 
 * // 删除文件
 * fs.unlinkSync("/temp.txt")
 * 
 * // 删除目录（递归删除）
 * fs.rmSync("/data", true)
 * ```
 * 
 * ### 与 SD 卡配合使用
 * ```javascript
 * import * as fs from "fs"
 * import { SDCard } from "sdcard"
 * import * as serial from "serial"
 * 
 * // 初始化 SD 卡
 * const spi = serial.spi2
 * spi.setup({ mosi: 23, miso: 19, sck: 18 })
 * 
 * const sd = new SDCard()
 * sd.setup({
 *     spi: 2,
 *     cs: 13,
 *     mount: "/sdcard"
 * })
 * 
 * // 现在可以访问 SD 卡上的文件
 * fs.writeFileSync("/sdcard/log.txt", "System started\n")
 * 
 * // 获取 SD 卡使用情况
 * const usage = fs.usage("/sdcard")
 * console.log("总容量:", usage.total)
 * console.log("已使用:", usage.used)
 * console.log("可用:", usage.free)
 * ```
 * 
 * ### 文件流操作（大文件）
 * ```javascript
 * import * as fs from "fs"
 * 
 * // 打开文件进行写入
 * const handle = fs.open("/large-file.bin", "w")
 * 
 * // 分块写入数据
 * for (let i = 0; i < 100; i++) {
 *     const buffer = new Uint8Array(1024)
 *     // 填充数据...
 *     fs.write(handle, buffer)
 * }
 * 
 * // 同步到存储介质（确保数据写入）
 * fs.sync(handle)
 * 
 * // 关闭文件
 * fs.close(handle)
 * 
 * // 读取大文件
 * const readHandle = fs.open("/large-file.bin", "r")
 * const chunk = fs.read(readHandle, 1024)  // 读取 1KB
 * fs.close(readHandle)
 * ```
 * 
 * ## 注意事项
 * 
 * 1. **Flash 寿命**：Flash 有擦写次数限制，避免频繁写入小文件
 * 2. **断电保护**：虽然 LittleFS 有断电保护，但重要数据写入后建议调用 `fs.sync()`
 * 3. **路径长度**：嵌入式系统路径长度有限，建议使用简短路径
 * 4. **文件句柄**：使用 `open()` 打开的文件必须调用 `close()` 关闭，避免资源泄漏
 * 
 * @module fs
 */

#include "FS.hpp"
#include "JSEngine.hpp"
#include "BeShell.class.hpp"
#include "FS.hpp"
#include "fs/FS.hpp"
#include "qjs_utils.h"
#include "../path.hpp"
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
        EXPORT_FUNCTION(usage) ;
        EXPORT_FUNCTION(cwd) ;
        EXPORT_FUNCTION(setCwd) ;
        EXPORT_FUNCTION(statSync) ;
        EXPORT_FUNCTION(existsSync) ;
        EXPORT_FUNCTION(isFileSync) ;
        EXPORT_FUNCTION(isDirSync) ;
        EXPORT_FUNCTION(open) ;
        EXPORT_FUNCTION(read) ;
        EXPORT_FUNCTION(write) ;
        EXPORT_FUNCTION(seek) ;
        EXPORT_FUNCTION(tell) ;
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
     * 同步创建目录。
     * 
     * 创建指定路径的目录。如果父目录不存在且 recursive 为 false，则创建失败。
     * 当 recursive 为 true 时，会自动创建所有不存在的父目录（类似 `mkdir -p`）。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 创建单个目录（父目录必须存在）
     * fs.mkdirSync("/data")
     * 
     * // 递归创建目录（自动创建所有不存在的父目录）
     * fs.mkdirSync("/data/2024/01", true)
     * 
     * ```
     *
     * @module fs
     * @function mkdirSync
     * @param path:string 要创建的目录路径
     * @param recursive:bool=false 是否递归创建父目录
     * @return bool 创建成功返回 true，失败返回 false
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
     * 同步删除目录。
     * 
     * 默认情况下，只能删除空目录。如果目录非空，返回 false。
     * 当 recursive 参数为 true 时，可以递归删除目录及其所有内容。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 删除空目录
     * fs.rmdirSync("/empty-dir")
     * 
     * // 递归删除目录及其所有内容（谨慎使用！）
     * fs.rmdirSync("/data", true)
     * ```
     *
     * @module fs
     * @function rmdirSync
     * @param path:string 要删除的目录路径
     * @param recursive:bool=false 是否递归删除非空目录
     * @return bool 删除成功返回 true，失败返回 false
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
     * 同步删除文件。
     * 
     * 删除指定路径的文件。只能删除文件，不能删除目录（删除目录请使用 rmdirSync 或 rmSync）。
     * 如果文件不存在或是目录，返回 false。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 删除单个文件
     * fs.unlinkSync("/temp.txt")
     * 
     * // 删除子目录下的文件
     * fs.unlinkSync("/config.old.json")
     * 
     * // 安全删除（先检查是否存在）
     * if (fs.existsSync("/temp.txt")) {
     *     fs.unlinkSync("/temp.txt")
     * }
     * ```
     *
     * @module fs
     * @function unlinkSync
     * @param path:string 要删除的文件路径
     * @return bool 删除成功返回 true，失败返回 false
     */
    JSValue FS:: unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return (unlink(path.c_str())>=0)? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步读取整个文件内容。
     * 
     * 读取指定文件的内容，返回 ArrayBuffer 对象。
     * 可以使用 ArrayBuffer 的方法转换为字符串或其他格式。
     * 
     * 
     * ```js
     * // 函数返回的是一个 ArrayBuffer 对象，可以使用 TextDecoder 或自定义方法转换为字符串，例如：
     * let content = new TextDecoder().decode(fs.readFileSync("/foo.bar"))
     * console.log(content)
     * 
     * // 或者
     * content = fs.readFileSync("/foo.bar").asString()
     * console.log(content)
     * ```
     * 
     * @module fs
     * @function readFileSync
     * @param path:string 要读取的文件路径
     * @param readlen:number=-1 读取长度（字节），-1 表示读取到文件末尾
     * @param offset:number=0 开始读取的位置偏移（字节）
     * @return ArrayBuffer 文件内容
     * @throws 路径是目录
     * @throws 文件不存在
     * @throws 打开文件失败
     * @throws 内存分配失败
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
     * 同步写入数据到文件。
     * 
     * 将数据写入指定文件。如果文件不存在则创建，存在则覆盖（除非 append 为 true）。
     * 如果路径是目录，将抛出异常。
     * 
     * @module fs
     * @function writeFileSync
     * @param path:string 目标文件路径
     * @param data:string|ArrayBuffer 要写入的数据内容
     * @param append:bool=false 是否在文件末尾追加写入，false 表示覆盖原有内容
     * @return number 实际写入的字节数
     * @throws 路径是目录
     * @throws 打开文件失败
     * @throws 数据类型无效或为空
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
     * 获取指定目录下的文件和子目录列表。
     * 
     * 如果 detail 参数为 false，返回文件名字符串数组。
     * 
     * 如果 detail 参数为 true，返回包含详细信息的对象数组，格式如下：
     * 
     * ```js
     * [
     *     {
     *         name: string,        // 文件名
     *         type: "file"|"dir"|"unknown",  // 类型：文件、目录或未知
     *         size: number         // 文件大小（字节），仅对文件有效
     *     } ,
     *     ...
     * ]
     * ```
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 简单列出文件名
     * const files = fs.listDirSync("/")
     * console.log("文件列表:", files)
     * 
     * // 获取详细信息
     * const details = fs.listDirSync("/", true)
     * details.forEach(item => {
     *     if (item.type === "file") {
     *         console.log(`文件: ${item.name}, 大小: ${item.size} 字节`)
     *     } else if (item.type === "dir") {
     *         console.log(`目录: ${item.name}`)
     *     }
     * })
     * 
     * // 遍历并筛选特定类型的文件
     * const allFiles = fs.listDirSync("/", true)
     * const jsonFiles = allFiles.filter(item => 
     *     item.type === "file" && item.name.endsWith(".json")
     * )
     * console.log("JSON 文件:", jsonFiles)
     * 
     * // 计算目录总大小
     * let totalSize = 0
     * const items = fs.listDirSync("/data", true)
     * items.forEach(item => {
     *     if (item.type === "file") {
     *         totalSize += item.size
     *     }
     * })
     * console.log("总大小:", totalSize, "字节")
     * ```
     *
     * @module fs
     * @function listDirSync
     * @param path:string 要读取的目录路径
     * @param detail:bool=false 是否返回详细信息，false 只返回文件名数组
     * @return string[]|object[] 文件名数组或详细信息对象数组
     * @throws 目录不存在
     * @throws 目录无法打开
     * @throws 权限不足
     */
    JSValue FS:: listDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        path_normalize(path) ;
        std::string fullpath = FS::toVFSPath(path.c_str()) ;

        DIR* dir = opendir(path.c_str());
        string childpath ;

        bool detail = false ;
        if(argc>1) {
            detail = JS_ToBool(ctx, argv[1]) ;
        }

        if(!dir) {
            JS_ThrowReferenceError(ctx, "Cound not open dir %s", fullpath.c_str());
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
                // t = tt  ;

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

        closedir(dir);

#ifdef ESP_PLATFORM
        // esp32 vfs 读取目录时，忽略了挂载点，将已知挂载点补充上
        char ppath[256] ;
        for(auto p:partitions) {
            if(p.first=="/fs") {
                continue;
            }
            path_dirname(p.first.c_str(), ppath) ;
            if(ppath==fullpath) {
                if(detail) {
                    JSValue item = JS_NewObject(ctx) ;
                    JS_SetPropertyUint32(ctx, ret, idx++, item) ;
                    JS_SetPropertyStr(ctx, item, "name", JS_NewString(ctx, path_basename(p.first.c_str()))) ;
                    JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "dir") ) ;
                } else {
                    JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, path_basename(p.first.c_str()))) ;
                }
            }
        }
#endif

        return ret ;
    }

    /**
     * 同步删除文件或目录。
     * 
     * 与 unlinkSync 不同，rmSync 可以删除目录（当 recursive 为 true 时）。
     * 默认情况下，只能删除文件和空目录。
     * 
     * @module fs
     * @function rmSync
     * @param path:string 要删除的文件或目录路径
     * @param recursive:bool=false 是否递归删除目录及其内容
     * @return bool 删除成功返回 true，失败返回 false
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
     * 同步重命名（移动）文件或目录。
     * 
     * 将文件或目录从 oldPath 移动到 newPath。如果 newPath 已存在，
     * 其行为取决于具体文件系统实现。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 重命名文件
     * fs.renameSync("/config.json", "/config.backup.json")
     * 
     * // 移动文件到另一个目录
     * fs.renameSync("/temp.txt", "/archive/temp.txt")
     * 
     * // 移动并重命名
     * fs.renameSync("/data.txt", "/backup/data-2024.txt")
     * 
     * // 重命名目录
     * fs.renameSync("/old-name", "/new-name")
     * ```
     *
     * @module fs
     * @function renameSync
     * @param oldPath:string 原文件或目录路径
     * @param newPath:string 新文件或目录路径
     * @return number 操作成功返回 0，失败返回非零错误代码（通常为 errno）
     */
    JSValue FS:: renameSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

        ASSERT_ARGC(1)
        ARGV_PATH(oldpath, 0)
        ARGV_PATH(newpath, 1)

        int ret = rename(oldpath.c_str(), newpath.c_str()) ;
        return JS_NewInt32(ctx, ret) ;
    }


    /**
     * 同步获取文件或目录的状态信息。
     * 
     * 返回指定路径的详细元数据信息。如果文件或目录不存在，返回 null。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 获取文件信息
     * const stat = fs.statSync("/config.json")
     * if (stat) {
     *     console.log("大小:", stat.size, "字节")
     *     console.log("是目录:", stat.isDir)
     *     console.log("修改时间:", new Date(stat.mtime * 1000))
     * }
     * 
     * // 检查路径是否存在
     * if (fs.statSync("/data.txt")) {
     *     console.log("文件存在")
     * } else {
     *     console.log("文件不存在")
     * }
     * 
     * // 获取目录信息
     * const dirStat = fs.statSync("/data")
     * if (dirStat && dirStat.isDir) {
     *     console.log("这是一个目录")
     * }
     * ```
     *
     * @module fs
     * @function statSync
     * @param path:string 要查询的文件或目录路径
     * @return object|null 状态信息对象，路径不存在时返回 null
     *     {
     *         dev:number,      // 设备ID
     *         ino:number,      // inode 号
     *         mode:number,     // 文件模式/权限
     *         size:number,     // 文件大小（字节）
     *         isDir:bool,      // 是否是目录
     *         atime:number,    // 最后访问时间（时间戳）
     *         mtime:number,    // 最后修改时间（时间戳）
     *         ctime:number     // 创建时间（时间戳）
     *     }
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
     * 同步检查路径是否存在。
     * 
     * 检查指定路径的文件或目录是否存在。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 检查文件是否存在
     * if (fs.existsSync("/config.json")) {
     *     console.log("配置文件存在")
     * }
     * 
     * // 检查目录是否存在
     * if (fs.existsSync("/data")) {
     *     console.log("数据目录存在")
     * }
     * 
     * // 安全写入（避免覆盖）
     * if (!fs.existsSync("/important.txt")) {
     *     fs.writeFileSync("/important.txt", "初始内容")
     * }
     * ```
     *
     * @module fs
     * @function existsSync
     * @param path:string 要检查的路径
     * @return bool 存在返回 true，不存在返回 false
     */
    JSValue FS:: existsSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::exist(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步检查路径是否是文件。
     * 
     * 如果路径存在且是常规文件，返回 true；
     * 如果路径不存在或是目录等其他类型，返回 false。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 检查是否是文件
     * if (fs.isFileSync("/config.json")) {
     *     console.log("这是一个文件")
     * }
     * 
     * // 遍历目录时筛选文件
     * const files = fs.listDirSync("/")
     * files.forEach(name => {
     *     const path = `/${name}`
     *     if (fs.isFileSync(path)) {
     *         console.log("文件:", name)
     *     }
     * })
     * ```
     *
     * @module fs
     * @function isFileSync
     * @param path:string 要检查的路径
     * @return bool 是文件返回 true，否则返回 false
     */
    JSValue FS:: isFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::isFile(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 同步检查路径是否是目录。
     * 
     * 如果路径存在且是目录，返回 true；
     * 如果路径不存在或是文件等其他类型，返回 false。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 检查是否是目录
     * if (fs.isDirSync("/data")) {
     *     console.log("这是一个目录")
     * }
     * 
     * // 遍历目录时筛选子目录
     * const files = fs.listDirSync("/")
     * files.forEach(name => {
     *     const path = `/${name}`
     *     if (fs.isDirSync(path)) {
     *         console.log("目录:", name)
     *     }
     * })
     * 
     * // 确保目录存在（不存在则创建）
     * if (!fs.isDirSync("/logs")) {
     *     fs.mkdirSync("/logs", true)
     * }
     * ```
     *
     * @module fs
     * @function isDirSync
     * @param path:string 要检查的路径
     * @return bool 是目录返回 true，否则返回 false
     */
    JSValue FS::isDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        return FS::isDir(path.c_str())? JS_TRUE: JS_FALSE ;
    }

    /**
     * 获取文件分区的存储使用情况。
     * 
     * 返回指定挂载点的存储空间信息，包括总容量、已用空间和剩余空间。
     * 
     * 返回对象格式：
     * ```js
     * {
     *     total:number,  // 分区总容量（字节）
     *     used:number,   // 已使用空间（字节）
     *     free:number    // 剩余可用空间（字节）
     * }
     * ```
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 获取 Flash 分区使用情况
     * const fsUsage = fs.usage("/")
     * console.log("Flash 总容量:", fsUsage.total, "字节")
     * console.log("已使用:", fsUsage.used, "字节")
     * console.log("可用:", fsUsage.free, "字节")
     * console.log("使用率:", (fsUsage.used / fsUsage.total * 100).toFixed(2) + "%")
     * 
     * // 获取 SD 卡使用情况
     * const sdUsage = fs.usage("/sdcard")
     * console.log("SD 卡总容量:", (sdUsage.total / 1024 / 1024).toFixed(2), "MB")
     * console.log("SD 卡可用:", (sdUsage.free / 1024 / 1024).toFixed(2), "MB")
     * 
     * // 检查空间是否充足
     * if (fsUsage.free < 1024 * 1024) {
     *     console.log("警告：Flash 空间不足 1MB")
     * }
     * ```
     *
     * @module fs
     * @function usage
     * @param path:string 分区挂载点路径（如 "/fs"、"/sdcard" 等）
     * @return object 包含存储使用信息的对象
     *     {
     *         total:number,  // 总容量（字节）
     *         used:number,   // 已使用空间（字节）
     *         free:number    // 可用空间（字节）
     *     }
     * @throws 路径不是有效的分区挂载点
     * @throws 获取分区使用情况失败
     */
    /**
     * 获取当前工作目录。
     *
     * 返回 BeShell 维护的当前工作路径（通过 `cd` 命令或 `FS.setCwd()` 设置）。
     * 注意：这个路径可能与底层操作系统的当前目录不同。
     *
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     *
     * console.log(fs.cwd())  // 例如: "/"
     * ```
     *
     * @module fs
     * @function cwd
     * @return string 当前工作目录路径
     */
    JSValue FS:: cwd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewString(ctx, FS::cwd().c_str()) ;
    }

    /**
     * 设置当前工作目录。
     *
     * 设置 BeShell 的当前工作路径（与 CLI `cd` 命令等效）。
     * 路径必须是已存在的目录，否则操作失败并返回 false。
     *
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     *
     * fs.setCwd("/app")
     * console.log(fs.cwd())  // "/app"
     * ```
     *
     * @module fs
     * @function setCwd
     * @param path:string 新的工作目录路径
     * @return bool 设置成功返回 true，失败返回 false
     */
    JSValue FS:: setCwd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_PATH(path, 0)
        struct stat st ;
        if(stat(path.c_str(), &st) < 0 || !S_ISDIR(st.st_mode)) {
            return JS_FALSE ;
        }
        return FS::setCwd(path) ? JS_TRUE : JS_FALSE ;
    }

    JSValue FS:: usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(1)
        string ARGV_TO_PATH(0, path)

        if( partitions.count(path) < 1 ){
            JSTHROW("path is not a partition mount point: %s", path.c_str()) ;
        }

        size_t total = 0 ;
        size_t used = 0 ;

        if( !partitions[path]->usage(total, used) ){
            JSTHROW("Failed to get usage of partition %s", path.c_str()) ;
        }

        size_t free_ = total - used ;


        JSValue obj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "total", JS_NewUint32(ctx, total));
        JS_SetPropertyStr(ctx, obj, "used", JS_NewUint32(ctx, used));
        JS_SetPropertyStr(ctx, obj, "free", JS_NewUint32(ctx, free_));

        return obj ;
    }

    
    /**
     * 打开文件，返回文件句柄用于后续操作。
     * 
     * mode 参数支持标准 C 库 fopen 的模式字符串：
     * - `"r"` - 只读模式，文件必须存在
     * - `"w"` - 只写模式，文件不存在则创建，存在则清空
     * - `"a"` - 追加模式，写入到文件末尾
     * - `"r+"` - 读写模式，文件必须存在
     * - `"w+"` - 读写模式，文件不存在则创建，存在则清空
     * - `"a+"` - 读写追加模式
     * 
     * 如果文件无法打开或创建，将抛出异常。
     * 
     * 示例：
     * ```javascript
     * import * as fs from "fs"
     * 
     * // 只读模式打开文件
     * const readHandle = fs.open("/config.json", "r")
     * const data = fs.read(readHandle, 1024)
     * fs.close(readHandle)
     * 
     * // 写入模式打开文件（会清空原有内容）
     * const writeHandle = fs.open("/output.txt", "w")
     * fs.write(writeHandle, "Hello World")
     * fs.close(writeHandle)
     * 
     * // 追加模式打开文件
     * const appendHandle = fs.open("/log.txt", "a")
     * fs.write(appendHandle, "New log entry\n")
     * fs.close(appendHandle)
     * 
     * // 读写模式打开文件
     * const rwHandle = fs.open("/data.bin", "r+")
     * // 读取数据
     * const chunk = fs.read(rwHandle, 100)
     * // 移动指针到开头
     * fs.seek(rwHandle, 0, 0)
     * // 写入新数据
     * fs.write(rwHandle, new Uint8Array([1, 2, 3, 4]))
     * fs.close(rwHandle)
     * ```
     *
     * @module fs
     * @function open
     * @param path:string 文件路径
     * @param mode:string="rw" 打开模式（注意：默认 "rw" 不是标准模式，建议使用 "r+" 或 "w+")
     * @return number 文件句柄（文件描述符）
     * @throws 文件不存在且模式为只读
     * @throws 权限不足
     * @throws 路径是目录
     * @throws 打开文件失败
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
     * 从已打开的文件中读取指定长度的数据。
     * 
     * 从当前文件指针位置开始读取，读取后文件指针会自动后移。
     * 实际读取的字节数可能小于请求的长度（如到达文件末尾）。
     * 
     * ```js
     * import {open,read,close} from "fs"
     * 
     * const handle = open("/sdcard/test.txt", "r") ;
     * const data = read(handle, 10) ;
     * console.log("readed bytes:", data.byteLength) ;
     * console.log("content:", data.toString()) ;
     * close(handle) ;
     * ```
     * 
     * @module fs
     * @function read
     * @param handle:number 由 open() 返回的文件句柄
     * @param length:number 要读取的字节数
     * @return ArrayBuffer 读取到的数据，长度可能小于请求值
     * @throws 参数数量不足
     * @throws 文件句柄无效
     * @throws 内存分配失败
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
     * 向已打开的文件写入数据。
     * 
     * 从当前文件指针位置开始写入，写入后文件指针会自动后移。
     * 支持写入字符串或 ArrayBuffer 类型的数据。
     * 
     * ```js
     * import {open,write,close} from "fs"
     * 
     * const handle = open("/sdcard/test.txt", "w") ;
     * let wrote = write(handle, "hello world") ;
     * console.log("wrote bytes:", wrote) ;
     * close(handle) ;
     * ```
     * 
     * @module fs
     * @function write
     * @param handle:number 由 open() 返回的文件句柄
     * @param data:ArrayBuffer|string 要写入的数据
     * @return number 实际写入的字节数
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
     * 移动文件指针到指定位置。
     * 
     * 设置文件读写的当前位置，影响后续的 read/write 操作。
     * 
     * whence 参数说明：
     * - `0` (SEEK_SET) - 从文件开头计算偏移量
     * - `1` (SEEK_CUR) - 从当前位置计算偏移量
     * - `2` (SEEK_END) - 从文件末尾计算偏移量
     * 
     * @module fs
     * @function seek
     * @param handle:number 由 open() 返回的文件句柄
     * @param offset:number 偏移量（字节）
     * @param whence:number=0 偏移基准位置：0=文件头, 1=当前位置, 2=文件尾
     * @return number 返回 0 表示成功，非零值表示错误代码
     */
    JSValue FS::seek(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_INT64(0, handle)
        ARGV_TO_UINT32(1, offset)
        ARGV_TO_INT32_OPT(2, whence, SEEK_SET)
        return JS_NewInt32(ctx, fseek((FILE*)handle, offset, whence)) ;
    }

    /**
     * 返回文件当前的读写位置。
     * 
     * 获取文件指针的当前偏移量（相对于文件开头的字节数）。
     * 
     * @module fs
     * @function tell
     * @param handle:number 由 open() 返回的文件句柄
     * @return number 当前文件指针位置（字节偏移量）
     */
    JSValue FS::tell(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        long pos = ftell((FILE*)handle) ;
        return JS_NewUint32(ctx, pos) ;
    }
    
    /**
     * 刷新文件缓冲区，将缓冲数据写入操作系统。
     * 
     * fflush() 将用户空间缓冲区数据写入内核，但不保证数据已写入物理存储设备。
     * 如需确保数据落盘，请使用 sync() 函数。
     * 
     * 注意：在 esp32 平台上效果可能受限。
     * 
     * @module fs
     * @function flush
     * @param handle:number 由 open() 返回的文件句柄
     * @return number 返回 0 表示成功，非零值（通常为 EOF/-1）表示失败
     */
    JSValue FS::flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        return JS_NewInt32(ctx, fflush((FILE*)handle)) ;
    }
    
    /**
     * 将文件数据同步写入物理存储介质。
     * 
     * fsync() 确保所有缓冲数据（包括文件元数据）已写入物理设备。
     * 这是一个相对耗时的操作，频繁调用可能影响性能。
     * 
     * 注意：在 esp32 平台上效果可能受限。
     * 
     * @module fs
     * @function sync
     * @param handle:number 由 open() 返回的文件句柄
     * @return number 返回 0 表示成功，非零值表示错误代码
     */
    JSValue FS::sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        int fd = fileno((FILE*)handle);
        return JS_NewInt32(ctx, fsync(fd)) ;
    }

    /**
     * 关闭 open 打开的文件句柄。
     * 
     * 关闭文件，释放相关资源。关闭后该句柄不可再使用。
     * 建议在完成文件操作后及时关闭，避免资源泄漏。
     * 
     * @module fs
     * @function close
     * @param handle:number 由 open() 返回的文件句柄
     * @return number 返回 0 表示成功，非零值（通常为 EOF/-1）表示失败
     */
    JSValue FS::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_INT64(0, handle)
        return JS_NewInt32(ctx, fclose((FILE*)handle)) ;
    }
}
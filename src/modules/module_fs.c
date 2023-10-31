#include "module_fs.h"
#include "logging.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "utils.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#ifndef ACCESSPERMS
    #define ACCESSPERMS 0777
#endif

#define PARTITION_LABEL_ROOT "fsroot"
#define PARTITION_LABEL_HOME "fshome"

#ifdef PLATFORM_ESP32
#include "diskio_wl.h"
#include "esp_littlefs.h"
#include "vfs_fat_internal.h"
#include <esp_heap_caps.h>
#include "rawfs.h"
#include "esp_vfs.h"
#include "esp_partition.h"
LOG_TAG("fs")
#endif

char * vfs_path_prefix = PATH_PREFIX ;
void module_fs_set_vfs_path_prefix(char * path) {
    vfs_path_prefix = path ;
}

#ifdef PLATFORM_WASM
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
void wasm_set_fs_prefix(const char * fs_path) {
    size_t len = strlen(fs_path) ;
    vfs_path_prefix = malloc(len+1) ;
    strcpy(vfs_path_prefix, fs_path) ;
}
#endif

// 返回值 由调用者 free
char * vfspath_to_fs(const char * path) {
    size_t pathlen = strlen(path) ;
    size_t prefixlen = strlen(vfs_path_prefix) ;
    char * fullpath = (void *)malloc(prefixlen+pathlen+1) ;
    if(fullpath) {
        strcpy(fullpath, vfs_path_prefix) ;
        strcpy(fullpath+prefixlen, path) ;
    }
    return fullpath ;
}

// esp32 vfs 不能挂在根目录，将整个系统挂载到 /fs 下，访问时自动加上
// 返回值 由调用者 free
char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) {
    char * jspath = JS_ToCString(ctx, argv) ;
    const char * path = vfspath_to_fs(jspath) ;
    // char * path = mallocf(PATH_PREFIX"%s", jspath) ;
    JS_FreeCString(ctx, jspath) ;
    return path ;
}


/**
 * 同步返回文件状态
 * 如果文件不存在,返回 null; 否则返回包含详细信息的对象
 * 
 * @beapi beapi.fs.statSync
 * @param path:string 文件路径
 * @return null|{dev:number,ino:number,mode:number,size:number,isDir:bool,atime:number,mtime:number,ctime:number}
 */
JSValue js_fs_stat_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) ;
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_NULL ;
    }
    free(path) ;
    
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
 * 同步返回路径是否是一个存在的目录
 * 
 * @beapi beapi.fs.isDirSync
 * @param path:string 路径
 * @return bool
 */
JSValue js_fs_is_dir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) 
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_FALSE ;
    }
    free(path) ;
    return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
}


/**
 * 同步返回路径是否是一个存在的文件
 * 
 * @beapi beapi.fs.isFileSync
 * @param path:string 路径
 * @return bool
 */
JSValue js_fs_is_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) 
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_FALSE ;
    }
    free(path) ;
    return S_ISREG(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
}

/**
 * 同步返回路径是否存在
 * 
 * @beapi beapi.fs.existsSync
 * @param path:string 路径
 * @return bool
 */
JSValue js_fs_exists_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0])
    struct stat statbuf;
    bool exists = stat(path,&statbuf)>=0 ;
    free(path) ;
    return exists? JS_TRUE: JS_FALSE ;
}


bool isDir(const char * path) {
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        return S_ISDIR(statbuf.st_mode)? true: false ;
    }
    return false ;
}
bool isFile(const char * path) {
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        return S_ISREG(statbuf.st_mode)? true: false ;
    }
    return false ;
}

// 递归创建目录
#ifdef WIN32
int mkdir_p(char* file_path) {
#else
int mkdir_p(char* file_path, mode_t mode) {
#endif
    if( !file_path || !(*file_path) )
        return -1 ;
    char* p = strchr(file_path + 1, '/') ;
    while (p!=NULL) {
        *p = '\0';
#ifdef WIN32
        if (mkdir(file_path) == -1) {
#else
        if (mkdir(file_path, mode) == -1) {
#endif
            if (errno != EEXIST) {
                *p = '/';
                return -1;
            }
        }
        *p = '/';
        p = strchr(p + 1, '/') ;
    }

    if( !isDir(file_path) ) {
#ifdef WIN32
        return mkdir(file_path) ;
#else
        return mkdir(file_path, mode) ;
#endif
    }
    else 
        return 0 ;
}

/**
 * 同步创建目录
 * 
 * @beapi beapi.fs.mkdirSync
 * @param path:string 路径
 * @param recursive:bool=false 路径
 * @return bool
 */
JSValue js_fs_mkdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        free(path) ;
        return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
    }

    bool recursive = false ;
    if(argc>1) {
        recursive = JS_ToBool(ctx, argv[1]) ;
    }

    bool ret ;
    if(recursive) {
#if defined(WIN32)
        ret = mkdir_p(path)>=0 ;
#else
        ret = mkdir_p(path, ACCESSPERMS)>=0 ;
#endif
    }
    else {
#if defined(WIN32)
        ret = mkdir(path)>=0 ;
#else
        ret = mkdir(path, ACCESSPERMS)>=0 ;
#endif
    }
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

/**
 * 同步删除目录。如果目录非空返回 false。
 * 
 * @beapi beapi.fs.rmdirSync
 * @param path:string 路径
 * @return bool
 */
JSValue js_fs_rmdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = rmdir(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

/**
 * 同步删除文件
 * 
 * @beapi beapi.fs.unlinkSync
 * @param path:string 路径
 * @return bool
 */
JSValue js_fs_unlink_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = unlink(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
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
JSValue js_fs_read_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    JS2VSFPath(path, argv[0])
    CHECK_ARGV0_NOT_DIR(path)

    int readlen = -1 ;
    if(argc>1) {
        JS_ToInt32(ctx, &readlen, argv[1]) ;
    }
    int offset = 0 ;
    if(argc>2) {
        JS_ToInt32(ctx, &offset, argv[2]) ;
    }

    if(readlen<0) {
        readlen = statbuf.st_size ;
    }

    if(readlen<1){
        return JS_NewArrayBuffer(ctx, NULL, 0, freeArrayBuffer, NULL, false) ;
    }

    char * buff = malloc(readlen) ;

    if(!buff) {
        free(path) ;
        THROW_EXCEPTION("Failed to malloc buff");
    }

	FILE * fd = fopen(path, "r");
    free(path) ;

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
JSValue js_fs_write_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(2)
    JS2VSFPath(path, argv[0])

    bool append = false ;

    // 文件已经存在
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        CHECK_ARGV0_NOT_DIR(path)
        // offset
        if(argc>2) {
            append = JS_ToBool(ctx, argv[2]) ;
        }
    }

	FILE * fd = fopen(path, append? "a+": "w");
    if(NULL==fd) {
        JS_ThrowReferenceError(ctx, "Failed to open file %s", path);
        free(path) ;
        return JS_EXCEPTION ;
    }

    char * buff = NULL ;
	uint32_t length;
    bool buffIsString = JS_IsString(argv[1]) ;

    if(buffIsString) {
        buff = JS_ToCString(ctx, argv[1]) ;
        length = strlen(buff) ;
    }

    else {
        buff = (char *)JS_GetArrayBuffer(ctx, &length, argv[1]) ;
        if(!buff || !length) {
            free(path) ;
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
    free(path) ;
    if(buffIsString) {
        JS_FreeCString(ctx, buff) ;
    }
    else {
        // @todo
        // free buff
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
JSValue js_fs_readdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)

    JS2VSFPath(path, argv[0]) ;
    DIR* dir = opendir(path);

    bool detail = false ;
    if(argc>1) {
        detail = JS_ToBool(ctx, argv[1]) ;
    }

    if(!dir) {
        JS_ThrowReferenceError(ctx, "Cound not open dir %s", path);
        free(path) ;
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

            char * childpath = mallocf("%s/%s",path,dirEnt->d_name) ;
            if(!childpath){
                // pf("could not malloc for path %s/%s ?", path, dirEnt->d_name)
                THROW_EXCEPTION("could not malloc for path %s/%s ?", path, dirEnt->d_name)
                break ;
            }

            printf("---\n") ;
            echo_time("stat",{
                if(stat(childpath,&statbuf)!=0) {
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
            
            free(childpath) ;
        } else {
            JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, dirEnt->d_name)) ;
        }
    }


#ifdef PLATFORM_ESP32
    // esp32 vfs 读取目录时，忽略了挂载点，将已知挂载点补充上
    if(strcmp(path,"/fs/")==0 || strcmp(path,"/fs")==0) {
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

    free(path) ;
    closedir(dir);
    

    return ret ;
}

bool rm(const char * path, bool recursive) {

    struct stat statbuf;

    // 文件不存在
    if(stat(path,&statbuf)<0) {
        return true ;
    }

    // 直接删除文件
    if(S_ISREG(statbuf.st_mode)) {
        // printf("unlink(%s)\n",path) ;
        return unlink(path)>-1 ;
    }

    // 目录
    else if(S_ISDIR(statbuf.st_mode)) {
        // 递归删除文件/子目录
        if(recursive) {
            DIR* dir = opendir(path);
            if(dir) {
                struct dirent *dirEnt;
                while((dirEnt = readdir(dir))) {
                    const char * childpath = mallocf("%s/%s",path, dirEnt->d_name) ;
                    if(strcmp(dirEnt->d_name, ".")==0 || strcmp(dirEnt->d_name, "..")==0) { 
                        continue ;
                    }
                    // ds(childpath)
                    if(childpath) {                        
                        rm(childpath, recursive) ;
                        free(childpath) ;
                    }
                }
            }
        }

        // printf("rmdir(%s)\n",path) ;
        return rmdir(path) > -1 ;
    }

    // unknow type? soft link ?
    return false ;
}

/**
 * 同步删除文件
 * 
 * @param path {string} 路径
 * @return bool
 */
JSValue js_fs_rm_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) ;

    bool recursive = false ;
    if(argc>1) {
        recursive = JS_ToBool(ctx, argv[1]) ;
    }

    bool ret = rm(path, recursive) ;
    free(path) ;

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
static JSValue js_fs_rename_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    JS2VSFPath(oldpath, argv[0]) ;
    JS2VSFPath(newpath, argv[1]) ;

    int ret = rename(oldpath, newpath) ;

    free(oldpath) ;
    free(newpath) ;

    return JS_NewInt32(ctx, ret) ;
}

/**
 * 文件分区的信息
 * 返回的对象包括了分区的总大小，和已用大小
 * 
 * @param path {string} 分区路径
 * @return {total:number, used:number}
 */
static JSValue js_fs_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_STRING(0, jslabel)

    size_t total = 0 ;
    size_t used = 0 ;
    size_t free_ = 0 ;

#ifdef PLATFORM_ESP32
    if(strcmp(jslabel,"/home")==0){
        // littlefs
        if( esp_littlefs_info(PARTITION_LABEL_HOME, &total, &used) != ESP_OK ) {
            JS_FreeCString(ctx, jslabel) ;
            THROW_EXCEPTION("esp_littlefs_info() bad")
        }

        // fatfs
        // size_t free_ = 0 ;
        // esp_err_t err = esp_vfs_fat_info(jslabel, &total, &free_) ;
        // if( err != ESP_OK ) {
        //     JS_ThrowReferenceError(ctx, "unknow mount point: %s, (%d)", jslabel, err) ;
        //     JS_FreeCString(ctx, jslabel) ;
        //     return JS_EXCEPTION ;
        // }
        // used = total - free_ ;
    }
    else 
    if(strcmp(jslabel,"/")==0) {
        vfs_rawfs_t * fs = be_rawfs_root() ;
        if(fs) {
            used = fs->size ;
            total = fs->partition_size ;
        }
    }
    else {
#if ESP_IDF_VERSION_MAJOR >= 5
        esp_err_t err = esp_vfs_fat_info(jslabel, &total, &free_) ;
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
    }



#endif
    JS_FreeCString(ctx, jslabel) ;

    JSValue obj = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, obj, "total", JS_NewUint32(ctx, total));
    JS_SetPropertyStr(ctx, obj, "used", JS_NewUint32(ctx, used));
    // JS_SetPropertyStr(ctx, obj, "free", JS_NewUint32(ctx, free_));

    return obj ;
}


static JSValue js_fs_opentest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * path = js_arg_to_vfspath(ctx, argv[0]) ;
    FILE * h = fopen(path, "r") ;
    dp(h)

    free(path) ;

    return JS_UNDEFINED ;
}

#ifdef PLATFORM_ESP32

char * patition_mapping(const char * label, esp_partition_type_t type, esp_partition_type_t subtype) {
    
	char * romdata = NULL ;

	const esp_partition_t * part = esp_partition_find_first(type, subtype, label);
    if(!part) {
        printf("not found partition: %s.\n", label) ;
        return NULL ;
    }

    printf("addr: 0x%x\n",(unsigned int)part->address) ;
    dn(part->size)
    ds(part->label)


    romdata = NULL ;
    
#if ESP_IDF_VERSION_MAJOR >= 5
	esp_partition_mmap_handle_t hrom ;
    esp_err_t ret = esp_partition_mmap(part, 0, part->size, ESP_PARTITION_MMAP_DATA, (const void**)&romdata, &hrom) ;
#else
	spi_flash_mmap_handle_t hrom ;
    esp_err_t ret = esp_partition_mmap(part, 0, part->size, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom) ;
#endif

	if(ret!= ESP_OK || !romdata) {
        printf("mmap failed: %d\n", ret) ;
        return NULL ;
    }

    return romdata ;
}


static wl_handle_t fs_home_handle = WL_INVALID_HANDLE;

bool be_module_fs_init() {

    // // littlefs /
    // const esp_vfs_littlefs_conf_t conf_root = {
    //     .base_path = "/fs",
    //     .partition_label = PARTITION_LABEL_ROOT,
    //     .format_if_mount_failed = true,
    //     .block_size = 4096
    // };
    // if(esp_vfs_littlefs_register(&conf_root)!=ESP_OK){
    //     printf("Failed to mount fs.\n") ;
    //     return false ;
    // }
    
    // littlefs /home
    const esp_vfs_littlefs_conf_t conf_home = {
        .base_path = "/fs/home",
        .partition_label = PARTITION_LABEL_HOME,
        .format_if_mount_failed = true ,
        .block_size = 4096
    };
    if(esp_vfs_littlefs_register(&conf_home)!=ESP_OK){
        printf("Failed to mount %s.\n", conf_home.base_path) ;
        return false ;
    }

    // char * romdata = patition_mapping("fshome", 0x01, 0x81) ;
    // if(romdata) {
    //     for(int i=0;i<10240;i++) {
    //         if(romdata[i]!=0xFF) {
    //             dn(i)
    //             romdata+= i - 8 ;
    //             break ;
    //         }
    //     }
    //     print_block((uint8_t *)romdata,8,8) ;
    // }
    
    // // fatfs /home
    // const esp_vfs_fat_mount_config_t mount_config = {
    //         .max_files = 4,
    //         .format_if_mount_failed = false,
    //         .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    // };
    // esp_err_t err;
    // err = esp_vfs_fat_spiflash_mount_rw_wl("/fs/home", PARTITION_LABEL_HOME, &mount_config, &fs_home_handle);
    // // err = esp_vfs_fat_spiflash_mount("/fs/home", PARTITION_LABEL_HOME, &mount_config, &fs_home_handle);
    // if (err != ESP_OK) {
    //     printf("Failed to mount FATFS to /fs/home: %s(%d)\n", esp_err_to_name(err), err);
    //     return false ;
    // }
    // else {
    //     printf("vvvvvvvvvvvvv") ;
    // }

    return true ;
}
#endif

void be_module_fs_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    
    JSValue fs = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "fs", fs);
    
    JS_SetPropertyStr(ctx, fs, "statSync", JS_NewCFunction(ctx, js_fs_stat_sync, "statSync", 1));
    JS_SetPropertyStr(ctx, fs, "unlinkSync", JS_NewCFunction(ctx, js_fs_unlink_sync, "unlinkSync", 1));
    JS_SetPropertyStr(ctx, fs, "readFileSync", JS_NewCFunction(ctx, js_fs_read_file_sync, "readFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "writeFileSync", JS_NewCFunction(ctx, js_fs_write_file_sync, "writeFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "readdirSync", JS_NewCFunction(ctx, js_fs_readdir_sync, "readdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "mkdirSync", JS_NewCFunction(ctx, js_fs_mkdir_sync, "mkdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmdirSync", JS_NewCFunction(ctx, js_fs_rmdir_sync, "rmdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmSync", JS_NewCFunction(ctx, js_fs_rm_sync, "rmSync", 1));
    JS_SetPropertyStr(ctx, fs, "existsSync", JS_NewCFunction(ctx, js_fs_exists_sync, "existsSync", 1));
    JS_SetPropertyStr(ctx, fs, "isDirSync", JS_NewCFunction(ctx, js_fs_is_dir_sync, "isDirSync", 1));
    JS_SetPropertyStr(ctx, fs, "isFileSync", JS_NewCFunction(ctx, js_fs_is_file_sync, "isFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "renameSync", JS_NewCFunction(ctx, js_fs_rename_sync, "renameSync", 1));
    JS_SetPropertyStr(ctx, fs, "info", JS_NewCFunction(ctx, js_fs_info, "info", 1));
    JS_SetPropertyStr(ctx, fs, "opentest", JS_NewCFunction(ctx, js_fs_opentest, "opentest", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

#include "module_utils.h"
#include "module_fs.h"
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include "logging.h"
#include "utils.h"
#include "cutils.h"
#include "module_fs.h"
#include "eventloop.h"
#include "uuid.h"
#include "module_telnet.h"
#include "module_metadata.h"

#ifdef PLATFORM_ESP32
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "soc/soc.h"
#include "esp_partition.h"
#include "esp_mac.h"

#include "untar.h"
#include "libb64/cdecode.h"
#include "libb64/cencode.h"
// #include "gbk.h"

LOG_TAG("util");


typedef struct {
	char *          levelString;
	esp_log_level_t level;
} level_t;
static level_t levels[] = {
	{"none",    ESP_LOG_NONE},
	{"error",   ESP_LOG_ERROR},
	{"warn",    ESP_LOG_WARN},
	{"info",    ESP_LOG_INFO},
	{"debug",   ESP_LOG_DEBUG},
	{"verbose", ESP_LOG_VERBOSE},
	{NULL, 0}
};

static JSValue js_utils_set_log_level(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	char * tagToChange = (char *)JS_ToCString(ctx, argv[0]);
	char * levelString = (char *)JS_ToCString(ctx, argv[1]);

	// printf("Setting a new log level to be tag: \"%s\", level: \"%s\"\n", tagToChange, levelString);

	level_t *pLevels = levels;
	while(pLevels->levelString != NULL) {
		if (strcmp(pLevels->levelString, levelString) == 0) {
			break;
		}
		pLevels++;
	}
	if (pLevels->levelString != NULL) {
		esp_log_level_set(tagToChange, pLevels->level);
	    return JS_UNDEFINED ;
	}
    else {
        JS_ThrowReferenceError(ctx, "missing param path");
        return JS_EXCEPTION ;
    }
}

#endif


static JSValue js_utils_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return JS_NewInt64(ctx, gettime()) ;
}
static JSValue js_utils_set_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_INT64(0, now)

    uint32_t sec = now/1000 ;
    uint32_t msec = now - sec*1000 ;

	struct timespec tm = {sec, msec*1000*1000} ;
    return clock_gettime(CLOCK_REALTIME, &tm)>=0? JS_TRUE: JS_FALSE ;
}

static JSValue js_utils_sleep(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
    uint32_t ms ;
	if(JS_ToUint32(ctx, &ms, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
    
    uint32_t us = 0 ;
    if(argc>1) {
        if(JS_ToUint32(ctx, &us, argv[1]) ) {
            JS_ThrowReferenceError(ctx, "Invalid param type");
            return JS_EXCEPTION ;
        }
    }

    int64_t expire = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec + ms*1000 + us;
    int64_t now = 0 ;

    while(1) {
        gettimeofday(&tv, NULL);
        now = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
        if(now >= expire ) {
            break ;
        }
#ifdef PLATFORM_ESP32
        esp_task_wdt_reset() ;
#endif
    }

    return JS_UNDEFINED ;
}

static JSValue js_utils_var_refcount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, JS_VALUE_HAS_REF_COUNT(argv[0])? VAR_REFCNT(argv[0])-1: 0) ;
}



static JSValue __js_utils_set_timer(JSContext *ctx, int argc, JSValueConst *argv, bool repeat){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t interval ;
	if(JS_ToUint32(ctx, &interval, argv[1]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	uint32_t addr = eventloop_push(ctx, (JSValue)argv[0], JS_UNDEFINED, interval, repeat) ;
	return JS_NewInt32(ctx, addr) ;
}

static JSValue js_utils_set_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_utils_set_timer(ctx, argc, argv, false) ;
}
static JSValue js_utils_set_interval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_utils_set_timer(ctx, argc, argv, true) ;
}

static JSValue js_utils_clear_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t ptr ;
	if(JS_ToUint32(ctx, &ptr, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	eventloop_remove(ctx, (eventloop_callback_t *)ptr) ;
	return JS_UNDEFINED ;
}


void hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b) {
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

static JSValue js_utils_hsv2rgb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_UINT32(0,h)
    ARGV_TO_UINT32(1,s)
    ARGV_TO_UINT32(2,v)

dn3(h,s,v)

    uint32_t r, g, b ;
    hsv2rgb(h,s,v,&r,&g,&b) ;
dn3(r,g,b)

    JSValue rgb = JS_NewArray(ctx) ;
    JS_SetPropertyUint32(ctx, rgb, 0, JS_NewUint32(ctx, r));
    JS_SetPropertyUint32(ctx, rgb, 1, JS_NewUint32(ctx, g));
    JS_SetPropertyUint32(ctx, rgb, 2, JS_NewUint32(ctx, b));
    return rgb ;
}


static JSValue js_utils_part_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewInt32(ctx, readPartId()) ;
}
static JSValue js_utils_part_version(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewInt32(ctx, readPartVersion()) ;
}

static JSValue js_utils_part_uuid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
#ifdef PLATFORM_ESP32
    uint8_t mac_addr[6] = {0};
    char mac_addr_str[13] = {0};
    esp_efuse_mac_get_default((uint8_t *)mac_addr);

    sprintf(mac_addr_str, "%02x%02x%02x%02x%02x%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]) ;
    mac_addr_str[12] = 0 ;

    return JS_NewStringLen(ctx, mac_addr_str, 12) ;
#else
    return JS_NewString(ctx, "123456789AEF") ;
#endif
}

static JSValue js_utils_generate_uuid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    uuid_t uu ;
    char uu_str[UUID_STR_LEN];
    uuid_generate(uu);
    uuid_unparse(uu, uu_str);
    return JS_NewStringLen(ctx, uu_str, UUID_STR_LEN-1) ;
}

#ifdef PLATFORM_ESP32
static JSValue js_utils_base64_encode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t srclen = 0 ;
    const char * src = JS_ToCStringLen(ctx, &srclen, argv[0]) ;
    size_t codelen = base64_encode_expected_len(srclen) ;
    char * code = (char *) malloc(codelen+1);
    if(!code) {
        JS_FreeCString(ctx, src) ;
        THROW_EXCEPTION("Cound not malloc for encode, memory low ?")
    }

    base64_encodestate _state;
    base64_init_encodestate(&_state);
    int len = base64_encode_block((const char *) &src[0], srclen, &code[0], &_state);
    len = base64_encode_blockend((code + len), &_state);

    JSValue ret = JS_NewStringLen(ctx, code, codelen) ;

    JS_FreeCString(ctx, src) ;
    free(code);

    return ret ;
}
static JSValue js_utils_base64_decode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t codelen = 0 ;
    const char * code = JS_ToCStringLen(ctx, &codelen, argv[0]) ;
    size_t srclen = base64_decode_expected_len(codelen) + 1;
    char * src = (char *) malloc(srclen);
    if(!src) {
        JS_FreeCString(ctx, src) ;
        THROW_EXCEPTION("Cound not malloc for decode, memory low ?")
    }
    
    base64_decodestate _state;
    base64_init_encodestate(&_state);

    base64_decode_block((const char *) &code[0], codelen, &src[0], &_state);

    JSValue ret = JS_NewStringLen(ctx, src, srclen) ;

    JS_FreeCString(ctx, code) ;
    free(src);

    return ret ;
}
#endif

static const uint8_t gamma8_table[256] = {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
          2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
          5,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9, 10, 10, 10,
         11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17,
         18, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26,
         27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38,
         38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51,
         52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
         69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 80, 81, 82, 84, 85, 86,
         87, 89, 90, 91, 92, 94, 95, 96, 98, 99,100,102,103,104,106,107,
        109,110,112,113,114,116,117,119,120,122,123,125,126,128,130,131,
        133,134,136,138,139,141,143,144,146,148,149,151,153,154,156,158,
        160,161,163,165,167,169,170,172,174,176,178,180,182,183,185,187,
        189,191,193,195,197,199,201,203,205,207,209,211,213,215,218,220,
        222,224,226,228,230,233,235,237,239,241,244,246,248,250,253,255,
};

uint8_t gamma8_correct(uint8_t val) {
    return gamma8_table[val] ;
}

JSValue js_utils_gamma8_correct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, val)
    return JS_NewUint32(ctx, gamma8_table[val]) ;
}

#ifdef PLATFORM_ESP32
int untar_dist_fd = NULL;
static int untar_entry_header_cb(header_translated_t *proper, int entry_index, void *context_data) {
    if(proper->type == T_NORMAL) {
        char * distpath = mallocf("%s/%s", (char *)context_data, proper->filename) ;
        printf(distpath) ;
        printf("\n") ;
        if((untar_dist_fd = fopen(distpath, "wb")) == NULL) {
            printf("Could not open [%s] for write.\n", distpath);
            free(distpath) ;
            return -1;
        }
        free(distpath) ;
    }
    else if(proper->type == T_DIRECTORY) {
        char * distpath = mallocf("%s/%s", (char *)context_data, proper->filename) ;
        if( mkdir_p(distpath, ACCESSPERMS) !=0 ){
            free(distpath) ;
            return -1 ;
        }
        free(distpath) ;
    } else {
        untar_dist_fd = NULL ;
    }

    return 0 ;
}
static int untar_entry_data_cb(header_translated_t *proper, int entry_index, void *context_data, unsigned char *block, int length) {
    if(untar_dist_fd != NULL)
        fwrite(block, length, 1, untar_dist_fd);
    return 0 ;
}
static int untar_entry_end_cb(header_translated_t *proper, int entry_index, void *context_data) {
    if(untar_dist_fd != NULL) {
        fclose(untar_dist_fd);
        untar_dist_fd = NULL;
    }
    return 0 ;
}

static JSValue js_utils_untar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    char * srcpath = js_arg_to_vfspath(ctx, argv[0])  ;
    char * distpath = js_arg_to_vfspath(ctx, argv[1])  ;

    if( mkdir_p(distpath, ACCESSPERMS) != 0 ){
        THROW_EXCEPTION("dist path invalid")
    }

    // untar
    entry_callbacks_t entry_callbacks = { untar_entry_header_cb, untar_entry_data_cb, untar_entry_end_cb };
    int res = read_tar(srcpath, &entry_callbacks, (void *)distpath)==0 ;

    free(srcpath) ;
    free(distpath) ;

    return res==0? JS_TRUE: JS_FALSE ;
}

#endif

static JSValue js_string_bytes(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_LEN(0, str, len)

    JS_FreeCString(ctx, str) ;
    return JS_NewUint32(ctx, len) ;
}

static JSValue js_pack(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    size_t totalsize = 0 ;
    char * str ;
    size_t num = 0 ;
    for(int i=0;i<argc; i++) {
        if(JS_IsString(argv[i])) {
            str = JS_ToCStringLen(ctx, &num, argv[i]) ;
            JS_FreeCString(ctx, str) ;
            str = NULL ;
            totalsize+= num + 1 ;
        }
        else if(JS_IsNumber(argv[i])) {
            totalsize+= 4 ;
        }
        else {
            THROW_EXCEPTION("Invalid data type of package")
        }
    }

    char * pkgbuff = malloc(totalsize) ;
    JSValue pkg = JS_NewArrayBuffer(ctx, (uint8_t *)pkgbuff, totalsize, freeArrayBuffer, NULL, false) ;

    size_t offset = 0 ;
    for(int i=0;i<argc; i++) {
        if(JS_IsString(argv[i])) {
            str = JS_ToCStringLen(ctx, &num, argv[i]) ;
            if(num>255)
                num = 255 ;
            * (pkgbuff+offset) = num ;
            offset+= 1 ;
            memcpy(pkgbuff+offset, str, num) ;
            offset+= num ;
            JS_FreeCString(ctx, str) ;
        }
        else if(JS_IsNumber(argv[i])) {
            if(JS_ToInt32(ctx, (int32_t*)&num, argv[i])){
                // dd
            }
            memcpy(pkgbuff+offset, (void *)&num, 4) ;
            offset+= 4 ;
        }
    }

    return pkg ;
}

static JSValue js_pack_int32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    if(offset+4>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }
    ARGV_TO_UINT32(2, num)
    memcpy(buff+offset, (void *)&num, 4) ;
    return JS_NewUint32(ctx, offset+4) ;
}
static JSValue js_unpack_int32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    if(offset+4>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }
    return JS_NewInt32(ctx, * (int32_t *)(buff+offset)) ;
}
static JSValue js_pack_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    ARGV_TO_STRING_LEN(2, str, strlen)

    if(strlen>255) {
        strlen = 255 ;
    }
    if(offset+strlen+1>bufflen) {
        strlen = bufflen - offset - 1 ;
    }

    *(buff+offset) = strlen ;

    if(strlen>0) {
        memcpy(buff+offset+1, str, strlen) ;
    }

    JS_FreeCString(ctx, str) ;
    return JS_NewUint32(ctx, offset+strlen+1) ;
}
static JSValue js_unpack_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    uint8_t strlen = *(buff+offset) ;
    if(offset+strlen+1>bufflen) {
        strlen = bufflen - offset - 1 ;
    }
    return JS_NewStringLen(ctx, (char*)(buff+offset+1), strlen) ;
}

static JSValue js_write_string_to_ArrayBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_STRING_LEN(1, str, strlen)
    size_t offset = 0 ;
    if(argc>=3) {
        if( JS_ToUint32(ctx, &offset, argv[2]) ) {
            JS_FreeCString(ctx, str) ;
            THROW_EXCEPTION("argv offset is not a number")
        }
    }
    if(offset+strlen>bufflen) {
        JS_FreeCString(ctx, str) ;
        THROW_EXCEPTION("out of buffer length")
    }
    memcpy(buff+offset, str, strlen) ;

    JS_FreeCString(ctx, str) ;
    return JS_UNDEFINED ;
}

static JSValue js_read_string_from_ArrayBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)

    size_t offset = 0 ;
    if(argc>=2) {
        if( JS_ToUint32(ctx, &offset, argv[1]) ) {
            THROW_EXCEPTION("argv offset is not a number")
        }
    }
    size_t strlen = 0 ;
    if(argc>=3) {
        if( JS_ToUint32(ctx, &strlen, argv[2]) ) {
            THROW_EXCEPTION("argv length is not a number")
        }
    }
    else {
        strlen = bufflen - offset ;
    }

    if(offset+strlen>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }

    return JS_NewStringLen(ctx, (char*)(buff+offset), strlen) ;
}

static JSValue js_arraybuffer_as_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    size_t size ;
    char * buff = (char *)JS_GetArrayBuffer(ctx, &size, this_val) ;
    if(!buff || !size) {
        return JS_NewStringLen(ctx, NULL, 0) ;
    }
    return JS_NewStringLen(ctx, buff, size) ;
}


static JSValue js_feed_watchdog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef PLATFORM_ESP32
    esp_task_wdt_reset() ;
#endif
    return JS_UNDEFINED ;
}


static JSValue js_set_timezone_offset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT32(0, minute)

    setTimezoneOffset(minute) ;

    return JS_UNDEFINED ;
}

static JSValue js_set_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT64(0, ms)
    
    struct timeval tv;
    tv.tv_sec = ms/1000;  // epoch time (seconds)
    tv.tv_usec = (ms%1000)*1000;    // microseconds

    printf("%lu.%lu\n",(long unsigned int)tv.tv_sec,(long unsigned int)tv.tv_usec);

#ifndef PLATFORM_WASM
    settimeofday(&tv, NULL);
#endif

    return JS_UNDEFINED ;
}


#ifdef PLATFORM_ESP32
static JSValue js_partition_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_STRING(0, name) ;
    ARGV_TO_INT32(1, offset) ;
    ARGV_TO_INT32(2, size) ;
    
    esp_partition_t * partition = esp_partition_find_first(0x01, 0x81, name);

    JS_FreeCString(ctx,name) ;
    // DP(partition)
    if(!partition) {
        THROW_EXCEPTION("partition not found:%s", name)
    }
    char * buffer = malloc(size) ;
    if(!buffer) {
        THROW_EXCEPTION("out of memory?")
    }
    if(esp_partition_read(partition, offset, buffer, size) != ESP_OK){
        printf("can not read flash\n") ;
        return JS_NULL ;
    }

    return JS_NewArrayBuffer(ctx, (uint8_t *)buffer, size, freeArrayBuffer, NULL, false) ;
}
#endif

// static const JSCFunctionListEntry js_utils_funcs[] = {
//     JS_CFUNC_DEF("time", 0, js_utils_time ),
// };

// static int js_utils_init(JSContext *ctx, JSModuleDef *m) {
//     printf("js_utils_init()\n") ;
//     JS_SetModuleExport(ctx, m, "time", JS_NewCFunction(ctx, js_utils_time, "time", 1));
//     JS_SetModuleExport(ctx, m, "default", JS_NewCFunction(ctx, js_utils_time, "time", 1));
//     return 0;
// }

// JSModuleDef *be_module_init_utils(JSContext *ctx) {
//     JSModuleDef *m;
//     m = JS_NewCModule(ctx, "utils", js_utils_init);
//     if (!m) {
//         return NULL;
//     }
//     // JS_AddModuleExportList(ctx, m, js_utils_funcs, countof(js_utils_funcs));
//     JS_AddModuleExport(ctx, m, "time");
//     JS_AddModuleExport(ctx, m, "default");
//     return m;
// }


// static JSValue js_utils_encoding(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(2)
//     ARGV_TO_INT32(1, code)
//     ARGV_TO_STRING(0, str)

//     if(code!=936) {
//         JS_FreeCString(ctx, str) ;
//         THROW_EXCEPTION("not support encoding code: %d", code)
//     }

//     char filename[256] ;
//     uint16_t len = GBKToStrUTF8( str, filename, 255 ) ;
//     filename[len] = 0 ;

//     JS_FreeCString(ctx, str) ;

//     return JS_NewStringLen(ctx, filename, len) ;
// }


void be_module_utils_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

	// utils 
    JSValue utils = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "utils", utils);

    JS_SetPropertyStr(ctx, utils, "hsv2rgb", JS_NewCFunction(ctx, js_utils_hsv2rgb, "hsv2rgb", 1));
#ifdef PLATFORM_ESP32
    JS_SetPropertyStr(ctx, utils, "setLogLevel", JS_NewCFunction(ctx, js_utils_set_log_level, "setLogLevel", 1));
    JS_SetPropertyStr(ctx, utils, "untar", JS_NewCFunction(ctx, js_utils_untar, "untar", 1));
    JS_SetPropertyStr(ctx, utils, "base64Encode", JS_NewCFunction(ctx, js_utils_base64_encode, "base64Encode", 1));
    JS_SetPropertyStr(ctx, utils, "base64Decode", JS_NewCFunction(ctx, js_utils_base64_decode, "base64Decode", 1));
    JS_SetPropertyStr(ctx, utils, "partitionRead", JS_NewCFunction(ctx, js_partition_read, "partitionRead", 1));
#endif
    JS_SetPropertyStr(ctx, utils, "time", JS_NewCFunction(ctx, js_utils_time, "time", 1));
    JS_SetPropertyStr(ctx, utils, "setTime", JS_NewCFunction(ctx, js_utils_set_time, "setTime", 1));
    // JS_SetPropertyStr(ctx, utils, "freeStacks", JS_NewCFunction(ctx, js_utils_free_stacks, "freeStacks", 1));
    JS_SetPropertyStr(ctx, utils, "partId", JS_NewCFunction(ctx, js_utils_part_id, "partId", 1));
    JS_SetPropertyStr(ctx, utils, "partVersion", JS_NewCFunction(ctx, js_utils_part_version, "partVersion", 1));
    JS_SetPropertyStr(ctx, utils, "partUUID", JS_NewCFunction(ctx, js_utils_part_uuid, "partUUID", 1));
    JS_SetPropertyStr(ctx, utils, "genUUID", JS_NewCFunction(ctx, js_utils_generate_uuid, "genUUID", 1));
    JS_SetPropertyStr(ctx, utils, "varRefCnt", JS_NewCFunction(ctx, js_utils_var_refcount, "varRefCnt", 1));
    JS_SetPropertyStr(ctx, utils, "gamma8Correct", JS_NewCFunction(ctx, js_utils_gamma8_correct, "gamma8Correct", 1));
    JS_SetPropertyStr(ctx, utils, "stringBytes", JS_NewCFunction(ctx, js_string_bytes, "stringBytes", 1));
    JS_SetPropertyStr(ctx, utils, "pack", JS_NewCFunction(ctx, js_pack, "pack", 1));
    JS_SetPropertyStr(ctx, utils, "packInt32", JS_NewCFunction(ctx, js_pack_int32, "packInt32", 1));
    JS_SetPropertyStr(ctx, utils, "unpackInt32", JS_NewCFunction(ctx, js_unpack_int32, "unpackInt32", 1));
    JS_SetPropertyStr(ctx, utils, "packString", JS_NewCFunction(ctx, js_pack_string, "packString", 1));
    JS_SetPropertyStr(ctx, utils, "unpackString", JS_NewCFunction(ctx, js_unpack_string, "unpackString", 1));
    JS_SetPropertyStr(ctx, utils, "writeStringToArrayBuffer", JS_NewCFunction(ctx, js_write_string_to_ArrayBuffer, "writeStringToArrayBuffer", 1));
    JS_SetPropertyStr(ctx, utils, "readStringFromArrayBuffer", JS_NewCFunction(ctx, js_read_string_from_ArrayBuffer, "readStringFromArrayBuffer", 1));
    JS_SetPropertyStr(ctx, utils, "feed", JS_NewCFunction(ctx, js_feed_watchdog, "feed", 1));
    JS_SetPropertyStr(ctx, utils, "setTimezoneOffset", JS_NewCFunction(ctx, js_set_timezone_offset, "setTimezoneOffset", 1));
    JS_SetPropertyStr(ctx, utils, "setTime", JS_NewCFunction(ctx, js_set_time, "setTime", 1));
    // JS_SetPropertyStr(ctx, utils, "toUnicode", JS_NewCFunction(ctx, js_utils_encoding, "toUnicode", 1));

	// global
    JS_SetPropertyStr(ctx, global, "sleep", JS_NewCFunction(ctx, js_utils_sleep, "sleep", 1));
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, js_utils_set_timeout, "setTimeout", 1));
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, js_utils_set_interval, "setInterval", 1));
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, js_utils_clear_timeout, "clearTimeout", 1));
    

    // ArrayBuffer.prototype.asString()
    JSValue ArrayBufferProto = js_get_glob_prop(ctx, 2, "ArrayBuffer", "prototype") ;
    JS_SetPropertyStr(ctx, ArrayBufferProto, "asString", JS_NewCFunction(ctx, js_arraybuffer_as_string, "asString", 1));
    

	JS_FreeValue(ctx, ArrayBufferProto);
	JS_FreeValue(ctx, beapi);
	JS_FreeValue(ctx, global);
}

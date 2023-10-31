#include "module_metadata.h"
#include "utils.h"
#include <string.h>

#ifdef PLATFORM_ESP32
#include "soc/soc.h"
#include "esp_efuse.h"
#endif

const char * field_names[] = {
    "",
    "PART_ID", 
    "PART_VERSION", 
} ;



int readPartId() {
#ifdef PLATFORM_LINUX
    return 255 ;
#elif defined PLATFORM_ESP32
    int value = esp_efuse_read_reg(3, 7) ; // blk3
    return (value >> 24) & 0xFF ;
#else defined PLATFORM_WASM
#endif
}

int readPartVersion() {
#ifdef PLATFORM_LINUX
    return 255 ;
#elif defined PLATFORM_ESP32
    int value = esp_efuse_read_reg(3, 7) ;
    return (value >> 16) & 0xFF ;
#else defined PLATFORM_WASM
#endif
}


#ifdef PLATFORM_ESP32


/**
 * @brief 
 * 
 * @param field 
 * @return uint32 
 */
static JSValue js_metadata_read_field(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, field)
    if(field>7) {
        THROW_EXCEPTION("metadata field must 0-7")
    }
    int value = esp_efuse_read_reg(3, field) ;

    return JS_NewInt32(ctx, value) ;
}
#endif

static int read_fields(JSContext *ctx, JSValue meta, const char * buf, int offset) {
    if(offset>29){ // 包最小长度 3
        return 0 ;
    }
    int fieldFlags = buf[offset] ;
    if(buf[offset]==0) {
        return 0 ;
    }
    int packlen = 2 ;
    uint8_t checksum = META_VERIFY_BYTE ^ fieldFlags ;

    int o=1 ;
    for(be_metafield_t field=METAFIELD_START+1 ; field<METAFIELD_END; field++, o++) {
        if(!(fieldFlags&field)) {
            continue ;
        }
        packlen ++ ;
        if(offset+packlen>32) {
            return 0;
        }
        checksum^= buf[offset+o] ;
    }

    if(checksum!=buf[offset+packlen-1]) {
        return 0 ;
    }

    o=1 ;
    for(be_metafield_t field=METAFIELD_START+1 ; field<METAFIELD_END; field++, o++) {
        if(!(fieldFlags&field)) {
            continue ;
        }
        JS_SetPropertyStr(ctx,meta,field_names[field],JS_NewUint32(ctx, buf[offset+o])) ;
    }

    return packlen ;
}

static void load_meta_from_efuse(JSContext *ctx, JSValue meta) {

    char buffer[32] ;
    memset(buffer, 0, 32) ;

#ifdef PLATFORM_LINUX
    FILE * fd = fopen("../filesystem/meta-bins/BeCamera-019-v1.bin", "r") ;
    if(NULL==fd) {
        printf("can not open meta bin file\n") ;
        return ;
    }
    int readed = fread(buffer, 1, 32, fd) ;
    fclose(fd) ;
#elif defined(PLATFORM_ESP32)
    for(int i=0; i<32; i+=4) {
        uint32_t value = REG_READ(i+0x3FF5A078) ;
        buffer[i] = value & 255 ;
        buffer[i+1] = (value>>8) & 255 ;
        buffer[i+2] = (value>>16) & 255 ;
        buffer[i+3] = (value>>24) & 255 ;
    }
#endif

    int offset = 0 ;
    while(1) {
        int len = read_fields(ctx,meta,buffer, offset) ;
        if(len==0) {
            break ;
        }
        offset+= len ;
    }
}


void module_metadata_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue metadata = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "meta", metadata);

    load_meta_from_efuse(ctx, metadata) ;

#ifdef PLATFORM_ESP32
    JS_SetPropertyStr(ctx, metadata, "readField", JS_NewCFunction(ctx, js_metadata_read_field, "readField", 1));
#endif

    JS_FreeValue(ctx, beapi);
}

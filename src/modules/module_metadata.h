#ifndef __H_MODULE_METADATA__
#define __H_MODULE_METADATA__

#include "quickjs-libc.h"

int readPartId() ;
int readPartVersion() ;

#define META_VERIFY_BYTE 55

typedef enum {
    METAFIELD_START = 0 ,
    METAFIELD_PART_ID ,
    METAFIELD_PART_VER ,

    METAFIELD_END
} be_metafield_t ;


void module_metadata_init() ;
void module_metadata_require(JSContext *ctx) ;
void module_metadata_loop(JSContext *ctx) ;
void module_metadata_reset(JSContext *ctx) ;

#endif
#ifndef ONCE_H_JSON
#define ONCE_H_JSON

#define JSONB_HEADER
#include "json-build.h"

#define JSON_BUF_MAX_SIZE 128*1024
#define JSON_BUF_BLOCK_SIZE 1024

typedef struct _json {
    jsonb b ;
    char * buf ;
    size_t buflen ;
} JSON ;

void json_init(JSON * json) ;

void json_free(JSON * json) ;

size_t remalloc (char ** buf, size_t size) ;

#define JSON_CALL(call, json)                               \
    if( call == JSONB_ERROR_NOMEM ) {                       \
        json.buflen = remalloc(&json.buf, json.buflen) ;    \
        call ;                                              \
    }

#define _JSON_ARGS_(json)                &json.b, json.buf, json.buflen
#define JSON_OBJECT(json)               JSON_CALL( jsonb_object(_JSON_ARGS_(json)), json)
#define JSON_OBJECT_POP(json)           JSON_CALL( jsonb_object_pop(_JSON_ARGS_(json)), json)
#define JSON_ARRAY(json)                JSON_CALL( jsonb_array(_JSON_ARGS_(json)), json)
#define JSON_ARRAY_POP(json)            JSON_CALL( jsonb_array_pop(_JSON_ARGS_(json)), json)
#define JSON_KEY(json, key, klen)       JSON_CALL( jsonb_key(_JSON_ARGS_(json), key, klen), json)
#define JSON_NUMBER(json, num)          JSON_CALL( jsonb_number(_JSON_ARGS_(json), num), json)
#define JSON_BOOL(json, val)            JSON_CALL( jsonb_bool(_JSON_ARGS_(json), val), json)
#define JSON_NULL(json)                 JSON_CALL( jsonb_null(_JSON_ARGS_(json)), json)
#define JSON_STRING(json, str, slen)    JSON_CALL( jsonb_string(_JSON_ARGS_(json), str, slen), json)

#endif
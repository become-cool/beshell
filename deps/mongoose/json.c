#include "json.h"
#include <stdlib.h>
#include <string.h>

void json_init(JSON * json) {
    json->buf = malloc(JSON_BUF_BLOCK_SIZE) ;
    json->buflen = JSON_BUF_BLOCK_SIZE ;
    jsonb_init(&json->b);
}

void json_free(JSON * json) {
    if(json->buf) {
        free(json->buf) ;
        json->buf = NULL ;
        json->buflen = 0 ;
    }
    
}

size_t remalloc (char ** buf, size_t size) {
    size_t newsize = size+JSON_BUF_BLOCK_SIZE ;
    char * newbuf = malloc(newsize) ;
    memcpy(newbuf, *buf, size) ;
    free(*buf) ;
    *buf = newbuf ;
    return newsize ;
}
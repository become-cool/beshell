#include "path.h"
#include <stdlib.h>

#define IS_Name_End(p) ( *(p) == '/' || *(p) == '\0' )

void path_normalize(const char * path) {

    char *src = path;
    char *dst = path;

    // 跳过开头的 ./ 和 ../
    if( *(src) == '.' && IS_Name_End(src+1) ) {
        src+= 2 ;
    }
    else if ( *(src) == '.' && *(src+1) == '.' && IS_Name_End(src+2) ) {
        src+= 3 ;
    }

    // 遍历输入路径字符串中的每个字符
    while (*src) {
        // 如果遇到连续的两个斜杠，跳过一个斜杠
        if (*src == '/' && *(src + 1) == '/') {
            src++;
        }
        // 如果遇到当前路径符号（/./），则跳过该符号
        else if (*src == '/' && *(src+1) == '.' && IS_Name_End(src+2) ) {
            src+=2 ;
        }
        // 如果遇到上级路径符号（/../），则向前删除前一个路径段
        else if ( IS_Name_End(src) && *(src+1) == '.' && *(src+2) == '.' && IS_Name_End(src+3) ) {            
            src += 3;
            dst-- ;
            // 向前查找最后一个路径段的分隔符
            while (dst > path && *dst != '/') {
                dst--;
            }
        }
        // 将字符复制到目标路径字符串
        else {
            *dst++ = *src++;
        }
    }
    // 结束目标路径字符串
    *dst = '\0';
}

char * path_extname(const char *path)  {
    size_t n = strlen(path) ;

    char * extname = path + n;
    for( ; extname>=path; extname-- ) {
        if(*extname == '.' ) {
            return extname + 1 ;
        }
    }
    return path + n ;
}

// path 末尾的 / 会被去掉
char * path_basename(const char *path)  {
    size_t n = strlen(path) ;
    if(n==0) {
        return path ;
    }

    char * basename = path + n - 1;

    // 跳过末尾的 /
    for( ; basename>path; basename-- ) {
        if(*basename != '/' ) {
            break ;
        }
        else {
            *basename = 0 ;
        }
    }

    for( ; basename>=path; basename-- ) {
        if(*basename == '/' ) {
            return basename + 1 ;
        }
    }
    return path ;
}

void path_dirname(const char *path, char * dirname)  {
    char * basename = path_basename(path) ;
    size_t n = basename - path ;
    if(n==0) {
        return ;
    }
    strncpy(dirname, path, n) ;
    dirname[n] = 0 ;
    
    // 去掉末尾的 /
    if(n>1 && dirname[n-1]=='/') {
        dirname[n-1] = 0 ;
    }
}
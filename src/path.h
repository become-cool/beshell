#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void path_normalize(const char *path) ;
char * path_extname(const char *path) ;
char * path_basename(const char *path) ;
void path_dirname(const char *path, char * dirname) ;

#ifdef __cplusplus
}
#endif
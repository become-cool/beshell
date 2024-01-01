#pragma once
#include <string>


std::string & path_normalize(std::string & path) ;
void path_normalize(char *path) ;
char * path_extname(const char *path) ;
char * path_basename(const char *path) ;
void path_dirname(const char *path, char * dirname) ;


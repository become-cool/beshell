#include "FSModule.hpp"
#include <sys/stat.h>


namespace beshell {
    FSModule::FSModule(): ESModule("fs") {

    }

    
    bool FSModule::isDir(const char * path) {
        struct stat statbuf;
        if(stat(path,&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? true: false ;
        }
        return false ;
    }
    bool FSModule::isFile(const char * path) {
        struct stat statbuf;
        if(stat(path,&statbuf)>=0) {
            return S_ISREG(statbuf.st_mode)? true: false ;
        }
        return false ;
    }

    std::vector<JSCFunctionListEntry> FSModule::exportFunctions() {
        return std::vector<JSCFunctionListEntry> () ;
    }
}
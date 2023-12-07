#pragma once
#include "ESModule.hpp"

namespace beshell {
    class FSModule: public ESModule {
    private:
    public:
        FSModule() ;
        static bool isDir(const char * path) ;
        static bool isFile(const char * path) ;

        std::vector<JSCFunctionListEntry> exportFunctions() ;
    } ;
}
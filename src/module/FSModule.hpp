#pragma once
#include "NativeModule.hpp"

namespace beshell {
    class FSModule: public NativeModule {
    private:
    public:
        FSModule() ;
        static bool isDir(const char * path) ;
        static bool isFile(const char * path) ;

        std::vector<JSCFunctionListEntry> exportFunctions() ;
    } ;
}
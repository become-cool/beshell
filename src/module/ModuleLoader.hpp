#pragma once

#include <map>
#include <string>
#include "module/ESModule.hpp"
#include "module/Path.hpp"
#include "module/FSModule.hpp"
#include "fs/FS.hpp"

namespace beshell {
    class JSEngine ;
    class ModuleLoader {
    private:
        std::map<std::string, ESModule *> modules;
        
        JSModuleDef * createModuleDef(JSContext *) ;
        void importModule(JSContext *) ;

    public:
        ModuleLoader() ;
        ~ModuleLoader() ;
        void addModule(ESModule * module) ;
        void setup(JSContext * ctx) ;

        static std::string resovleFS(FS & fs, const char * module_name, const char * base_dir) ;
        static char * normalize(JSContext *ctx, const char *module_base_name, const char * module_name, void *opaque) ;
        static JSModuleDef * load(JSContext *ctx, const char *path, void *opaque) ;

    } ;
}
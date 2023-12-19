#pragma once

#include <map>
#include <string>
#include "module/NativeModule.hpp"
#include "module/PathModule.hpp"
#include "module/FSModule.hpp"
#include "fs/FS.hpp"

namespace be {
    class JSEngine ;
    class ModuleLoader {
    private:
        std::map<std::string, NativeModule *> modules;
        
        JSModuleDef * createModuleDef(JSContext *) ;
        void importModule(JSContext *) ;

    public:
        ModuleLoader() ;
        ~ModuleLoader() ;
        void add(NativeModule * module) ;

        void init(JSRuntime * rt) ;
        void setup(JSContext * ctx) ;

        static std::string resovleFS(FS & fs, const char * module_name, const char * base_dir) ;
        static char * normalize(JSContext *ctx, const char *module_base_name, const char * module_name, void *opaque) ;
        static JSModuleDef * load(JSContext *ctx, const char *path, void *opaque) ;

    } ;
}
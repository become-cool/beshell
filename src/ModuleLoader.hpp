#pragma once

#include <map>
#include <vector>
#include <string>
#include "NativeModule.hpp"
#include "fs/FSModule.hpp"
#include "fs/FS.hpp"

namespace be {
    class JSEngine ;
    class ModuleLoader ;
    
	typedef NativeModule* (*NativeModuleFactoryFunc)(JSContext * ctx, const char * name) ;

    class ModuleLoader {
    private:
        std::map<const char *, NativeModuleFactoryFunc> factories ;
        std::map<JSContext*, std::map<std::string, NativeModule*>> modules ;

    public:
        ModuleLoader() ;
        ~ModuleLoader() ;

        void add(const char * name, NativeModuleFactoryFunc factory) ;

        // NativeModule * add(NativeModule * module) ;
        // NativeModule * added(const char *) ;

        // void init(JSRuntime * rt) ;
        void setup(JSContext * ctx) ;

        NativeModule * moduleByName(JSContext * ctx, const char * name) ;

        static std::string resovleFS(FS * fs, const char * module_name, const char * base_dir) ;
        static char * normalize(JSContext *ctx, const char *module_base_name, const char * module_name, void *opaque) ;
        static JSModuleDef * load(JSContext *ctx, const char *path, void *opaque) ;

    } ;
}
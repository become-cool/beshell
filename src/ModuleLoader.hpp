#pragma once

#include <map>
#include <vector>
#include <string>
#include "NativeModule.hpp"
#include "fs/FS.hpp"

namespace be {
    class BeShell ;
    class JSEngine ;
    class ModuleLoader ;
    
	typedef NativeModule* (*NativeModuleFactoryFunc)(JSContext * ctx, const char * name) ;

    class ModuleLoader {
    private:
        std::map<const char *, NativeModuleFactoryFunc> factories ;
        std::map<JSContext*, std::map<std::string, NativeModule*>> modules ;

    public:
        ModuleLoader(BeShell * beshell) ;
        ~ModuleLoader() ;

        template <typename M>
        static NativeModule* factory(JSContext * ctx, const char * name) {
            return new M(ctx, name) ;
        }
        template <typename M>
        void add(BeShell * beshell, const char * name=nullptr) {
            if(!name) {
                name = M::name ;
            }
            if(!name) {
                printf("ModuleLoader::add(): name is null\n") ;
                return ;
            }
            factories[name] = factory<M> ;
            M::use(beshell) ;
        }
        
        void setup(JSContext * ctx) ;

        NativeModule * moduleByName(JSContext * ctx, const char * name) ;

        static std::string resovleFS(const char * module_name, const char * base_dir) ;
        static char * normalize(JSContext *ctx, const char *module_base_name, const char * module_name, void *opaque) ;
        static JSModuleDef * load(JSContext *ctx, const char *path, void *opaque) ;
    } ;
}
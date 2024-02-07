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

        template <typename M>
        static NativeModule* factory(JSContext * ctx, const char * name) {
            return new M(ctx, name) ;
        }
        template <typename M>
        void add(const char * name=nullptr) {
            factories[name? name: M::name] = factory<M> ;
        }
        
        void setup(JSContext * ctx) ;

        NativeModule * moduleByName(JSContext * ctx, const char * name) ;

        static std::string resovleFS(FS * fs, const char * module_name, const char * base_dir) ;
        static char * normalize(JSContext *ctx, const char *module_base_name, const char * module_name, void *opaque) ;
        static JSModuleDef * load(JSContext *ctx, const char *path, void *opaque) ;

    } ;
}
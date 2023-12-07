#include "module/ModuleLoader.hpp"
#include "module/NativeModule.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include <cstring>
#include <cassert>
#include <stdlib.h>
#include "path.h"
#include "utils.h"

namespace beshell {

    class JSLoader: public NativeModule {
    public:
        JSLoader(): NativeModule("loader") {
            exportFunction("require",js_require) ;
        };

        static JSValue js_require(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            dd
            
            CHECK_ARGC(1)
dd
            ARGV_TO_STRING(0, module_name)
dd
            JSModuleDef * m = JS_RunModule(ctx, "", module_name);
            dd
            JS_FreeCString(ctx, module_name);
dd
            if (!m){
                return JS_EXCEPTION ;
            }

            /* return the module namespace */
            JSValue ns = js_get_module_ns(ctx, m);
            if (JS_IsException(ns)){
                return JS_EXCEPTION ;
            }
dd
            return ns ;
        }

        void load(JSContext *ctx) {
            JSValue global = JS_GetGlobalObject(ctx) ;
            JS_SetPropertyStr(ctx, global, "require", JS_NewCFunction(ctx, js_require, "require", 1));
            JS_FreeValue(ctx,global) ;

        }
    } ;

    ModuleLoader::ModuleLoader() {
        addModule(new JSLoader) ;
    }
    ModuleLoader::~ModuleLoader() {
    }

    void ModuleLoader::addModule(NativeModule * module) {
        modules[ module->name ] = module ;
    }

    void ModuleLoader::setup(JSContext * ctx) {
        
        JS_SetModuleLoaderFunc(JS_GetRuntime(ctx), normalize, load, NULL);

        for (const auto & pair : modules) {
            std::cout << pair.first << endl ;
            pair.second->createModule(ctx) ;
            pair.second->load(ctx) ;

            if(pair.second->replGlobal) {

            }
        }
    }

    // 返回的字符串，需要由调用者 js_free
    std::string ModuleLoader::resovleFS(FS & fs, const char * module_name, const char * base_dir) {

        std::string fullpath = base_dir ;
        fullpath+= "/" ;
        fullpath+= module_name ;

        // to vfs path
        fullpath = fs.toVFSPath(fullpath.c_str()) ;

        if(FSModule::isFile(fullpath.c_str())) {
            return fullpath ;
        }

        if(FSModule::isDir(fullpath.c_str())) {
            // @todo package

            // index.js
            fullpath+= "/index.js" ;
            if(FSModule::isFile(fullpath.c_str())) {
                return fullpath ;
            }
        }
        
        fullpath = fullpath+= ".bin" ;
        if(FSModule::isFile(fullpath.c_str())) {
            return fullpath ;
        }

        return std::string("") ;
    }

    char * ModuleLoader::normalize(JSContext *ctx, const char *module_base_name, const char *module_name, void *opaque) {
        // 内置模块 
        // -------------
        if(
            strcmp(module_name,"fs")==0
            || strcmp(module_name,"path")==0
            || strcmp(module_name,"loader")==0
            || strcmp(module_name,"wifi")==0
            || strcmp(module_name,"gpio")==0
            || strcmp(module_name,"utils")==0
            || strcmp(module_name,"telnet")==0
        ) {
            return js_strdup(ctx, module_name) ;
        }

        // resolve file 
        // -------------

        BeShell * beshell = BeShell::fromJSContext(ctx) ;
        assert(beshell) ;
        
        std::string fullpath ;
        // 绝对路
        if(module_name[0]=='/') {
            fullpath = resovleFS(beshell->fs, module_name, NULL) ;
        }

        // 相对路径
        else if(module_base_name && (strncmp(module_name,"./",2)==0 || strncmp(module_name,"../",3)==0) ) {
            char * base_dir = strdup(module_base_name) ;
            path_dirname(module_base_name, base_dir) ;
            fullpath = resovleFS(beshell->fs, module_name, base_dir) ;
            free(base_dir) ;
        }

        // 系统默认目录
        else {
            fullpath = resovleFS(beshell->fs, module_name, "/lib/local") ;
            if(!fullpath.length()) {
                fullpath = resovleFS(beshell->fs, module_name, "/opt") ;
            }
        }

        if(!fullpath.length()) {
            return NULL ;
        }

        path_normalize(fullpath.c_str()) ;

        char * jfullpath = js_strdup(ctx, fullpath.c_str()) ;

        return jfullpath ;
    }

    JSModuleDef * ModuleLoader::load(JSContext *ctx, const char *path, void *opaque) {

        JSModuleDef *m;

        size_t buf_len;
        uint8_t *buf;
        JSValue func_val;

        buf = js_load_file(ctx, &buf_len, path);
        if (!buf) {
            JS_ThrowReferenceError(ctx, "could not load module filename '%s'", path);
            return NULL;
        }

        size_t pathlen = strlen(path) ;
        bool asBin = strcmp(path+pathlen-7,".js.bin") == 0 ;
        if(asBin) {
            // 做为字节码加载
            func_val = JS_ReadObject(ctx, buf, buf_len, JS_READ_OBJ_BYTECODE);
        } else {
            // 编译
            func_val = JS_Eval(ctx, (char *)buf, buf_len, path, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
        }

        js_free(ctx, buf);
        if (JS_IsException(func_val)){
            return NULL;
        }

        js_module_set_import_meta(ctx, func_val, true, false);
        
        m = (JSModuleDef*)JS_VALUE_GET_PTR(func_val);
        JS_FreeValue(ctx, func_val);
        
        return m;
    }
}
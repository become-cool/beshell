#include "module/ModuleLoader.hpp"
#include "module/ProcessModule.hpp"
#include "module/PathModule.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include <cstring>
#include <cassert>
#include <stdlib.h>
#include "path.hpp"
#include "utils.h"
#include "quickjs_private.h"



namespace be {

    class JSLoader: public NativeModule {
    public:
        using NativeModule::NativeModule;
        
        JSLoader(JSContext * ctx, const char * name,uint8_t flagGlobal)
            : NativeModule(ctx, name, flagGlobal)
        {
            exportFunction("__filename",jsFilename) ;
            exportFunction("__dirname",jsDirname) ;
        }
        static NativeModule* factory(JSContext * ctx, const char * name) {
            return new JSLoader(ctx,name,1) ;
        }
        
        static JSValue jsFilename(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            int32_t stack = 1 ;
            if(argc>0) {
                if(JS_ToInt32(ctx, &stack, argv[0])!=0) {
                    THROW_EXCEPTION("argv stack must be a number")
                }
            }
            JSAtom atom = JS_GetScriptOrModuleName(ctx, stack) ;
            JSValue val1 = JS_AtomToString(ctx, atom);

            char * fullpath = (char *)JS_ToCString(ctx,val1) ;
            path_normalize(fullpath) ;
            JSValue val2 = JS_NewString(ctx, fullpath);

            JS_FreeAtom(ctx,atom) ;
            JS_FreeValue(ctx,val1) ;
            JS_FreeCString(ctx,fullpath) ;

            return val2 ;
        }
        static JSValue jsDirname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

            int32_t stack = 1 ;
            if(argc>0) {
                if(JS_ToInt32(ctx, &stack, argv[0])!=0) {
                    THROW_EXCEPTION("argv stack must be a number")
                }
            }

            JSAtom atom = JS_GetScriptOrModuleName(ctx, stack) ;
            JSValue val = JS_AtomToString(ctx, atom);

            char * fullpath = (char *)JS_ToCString(ctx, val) ;
            path_normalize(fullpath) ;
            // char * path = vfspath_trim_prefix(fullpath) ;
            JS_FreeAtom(ctx,atom) ;
            JS_FreeValue(ctx,val) ;

            char * dir = (char *) malloc(strlen(fullpath)+1) ;
            path_dirname(fullpath,dir) ;

            val = JS_NewString(ctx,dir) ;

            JS_FreeCString(ctx, fullpath) ;
            free(dir) ;

            return val ;
        }
    } ;

    ModuleLoader::ModuleLoader() {
        add("loader", JSLoader::factory) ;
        add("process", ProcessModule::factory) ;
    }
    
    ModuleLoader::~ModuleLoader() {
        // @todo  delete native modules
    }
    
    void ModuleLoader::add(const char * name, NativeModuleFactoryFunc factory) {
        factories[name] = factory ;
    }

    NativeModule * ModuleLoader::moduleByName(JSContext * ctx, const char * name) {
        if( modules.count(ctx)<1 ) {
            return nullptr ;
        }
        if(modules[ctx].count(name)<1) {
            return nullptr ;
        }
        return modules[ctx][name] ;
    }

    void ModuleLoader::setup(JSContext * ctx) {

        JS_SetModuleLoaderFunc(JS_GetRuntime(ctx), normalize, load, this);

        for (const auto & pair : factories) {

            NativeModule * nm = pair.second(ctx, pair.first) ;
            if(!nm) {
                printf("module %s factory return NULL\n", pair.first) ;
                continue;
            }

            modules[ctx][(const char *)pair.first] = nm ;

            if(nm->flagGlobal==1) {

                JSModuleDef * mm = JS_RunModule(ctx, "", (const char *)pair.first);
                if(mm) {
                    JSValue mi = js_get_module_ns(ctx, mm ) ;

                    if (JS_IsException(mi)){
                        // todo
                    } else {
                        JSEngine::setGlobalValue(ctx, (const char *)pair.first, mi);
                    }
                    JS_FreeValue(ctx, mi) ;
                }
            }

            nm->setup(ctx) ;
        }        
    }

    std::string ModuleLoader::resovleFS(FS * fs, const char * module_name, const char * base_dir) {

        assert(fs) ;

        std::string fullpath = base_dir ;
        fullpath+= "/" ;
        fullpath+= module_name ;

        // to vfs path
        fullpath = fs->toVFSPath(fullpath.c_str()) ;

        if(fs->isFile(fullpath.c_str())) {
            return fullpath ;
        }

        if(fs->isDir(fullpath.c_str())) {
            // @todo package

            // index.js
            fullpath+= "/index.js" ;
            if(fs->isFile(fullpath.c_str())) {
                return fullpath ;
            }
        }
        
        fullpath = fullpath+= ".bin" ;
        if(fs->isFile(fullpath.c_str())) {
            return fullpath ;
        }

        return std::string("") ;
    }

    char * ModuleLoader::normalize(JSContext *ctx, const char *module_base_name, const char *module_name, void *opaque) {
        assert(opaque) ;
        ModuleLoader * mloader = (ModuleLoader *)opaque ;

        if(mloader->modules.count(ctx)<1) {
            return nullptr ;
        }

        // 内置模块 
        // -------------
        for (const auto & pair : mloader->modules[ctx]) {
            if( pair.first==module_name ) {
                return js_strdup(ctx, module_name) ;
            }
        }

        // resolve file
        // -------------
        JSEngine * engine= JSEngine::fromJSContext(ctx) ;
        assert(engine) ;
        
        std::string fullpath ;
        // 绝对路
        if(module_name[0]=='/') {
            fullpath = resovleFS(engine->beshell->fs, module_name, NULL) ;
        }

        // 相对路径
        else if(module_base_name && (strncmp(module_name,"./",2)==0 || strncmp(module_name,"../",3)==0) ) {
            char * base_dir = strdup(module_base_name) ;
            path_dirname(module_base_name, base_dir) ;
            fullpath = resovleFS(engine->beshell->fs, module_name, base_dir) ;
            free(base_dir) ;
        }

        // 系统默认目录
        else {
            fullpath = resovleFS(engine->beshell->fs, module_name, "/lib/local") ;
            if(!fullpath.length()) {
                fullpath = resovleFS(engine->beshell->fs, module_name, "/opt") ;
            }
        }

        if(!fullpath.length()) {
            return nullptr ;
        }

        path_normalize((char *)fullpath.c_str()) ;

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
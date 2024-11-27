#include "ModuleLoader.hpp"
#include "module/GPIO.hpp"
#include "module/Process.hpp"
#include "module/Path.hpp"
#include "driver/DriverModule.hpp"
#include "telnet/TelnetModule.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "fs/FS.hpp"
#include <cstring>
#include <cassert>
#include <stdlib.h>
#include "path.hpp"
#include "mallocf.h"
#include "qjs_utils.h"
#include "quickjs_private.h"

/* insert 'el' between 'prev' and 'next' */
static inline void __list_add(struct list_head *el, 
                              struct list_head *prev, struct list_head *next)
{
    prev->next = el;
    el->prev = prev;
    el->next = next;
    next->prev = el;
}

/* add 'el' at the end of the list 'head' (= before element head) */
static inline void list_add_tail(struct list_head *el, struct list_head *head)
{
    __list_add(el, head->prev, head);
}

namespace be {

    class JSLoader: public NativeModule {
    public:
        using NativeModule::NativeModule;
        
        JSLoader(JSContext * ctx, const char * name,uint8_t flagGlobal=1)
            : NativeModule(ctx, name, flagGlobal)
        {
            exportFunction("__filename",jsFilename) ;
            exportFunction("__dirname",jsDirname) ;
            exportFunction("compile",compile) ;
            exportFunction("importSync",importSync) ;
            exportFunction("exportValue",exportValue) ;
            exportFunction("allModuleNames",allModuleNames) ;
        }
        
        static JSValue importSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            ASSERT_ARGC(1)
            const char * modulename = JS_ToCString(ctx, argv[0]) ;

            JSAtom atom = JS_GetScriptOrModuleName(ctx, 1) ;
            JSValue val = JS_AtomToString(ctx, atom);

            char * fullpath = (char *)JS_ToCString(ctx, val) ;
            path_normalize(fullpath) ;

            JS_FreeAtom(ctx,atom) ;
            JS_FreeValue(ctx,val) ;

            char * dir = (char *) malloc(strlen(fullpath)+1) ;
            path_dirname(fullpath,dir) ;

            JSModuleDef * mdef = JS_RunModule(ctx, fullpath, modulename) ;
            
            JS_FreeCString(ctx, fullpath) ;
            free(dir) ;

            if(!mdef) {
                JSValue excpt = JS_GetException(ctx) ;
                if( JS_IsNull(excpt) ) {
                    JSTHROW("Cannot find module: %s", modulename) ;
                } else {
                    JS_Throw(ctx, excpt) ;
                    return JS_EXCEPTION ;
                }
            }
            JSValue mi = js_get_module_ns(ctx, mdef ) ;

            JS_FreeCString(ctx, modulename) ;
            return mi ;
        }
        
        static JSValue jsFilename(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            int32_t stack = 1 ;
            if(argc>0) {
                if(JS_ToInt32(ctx, &stack, argv[0])!=0) {
                    JSTHROW("argv stack must be a number")
                }
            }
            JSAtom atom = JS_GetScriptOrModuleName(ctx, stack) ;
            JSValue val1 = JS_AtomToString(ctx, atom);

            char * fullpath = (char *)JS_ToCString(ctx,val1) ;
            path_normalize(fullpath) ;
            const char * fullpath2 = FS::trimVFSPath(fullpath) ;

            JSValue val2 = JS_NewString(ctx, fullpath2);

            JS_FreeAtom(ctx,atom) ;
            JS_FreeValue(ctx,val1) ;
            JS_FreeCString(ctx,fullpath) ;

            return val2 ;
        }
        static JSValue jsDirname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

            int32_t stack = 1 ;
            if(argc>0) {
                if(JS_ToInt32(ctx, &stack, argv[0])!=0) {
                    JSTHROW("argv stack must be a number")
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
        static JSValue compile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            ASSERT_ARGC(1)

            std::string ARGV_TO_STRING(0, source)
            std::string dist ;

            if(argc>1) {
                ARGV_TO_STRING(1, dist)
            } else {
                dist = source + ".bin" ;
            }

            int readed = 0 ;
            std::unique_ptr<char> content = FS::readFileSync(source.c_str(), &readed) ;
            if(readed<0) {
                JSTHROW("could not read file: %s", source.c_str())
            }

            std::string script(content.get(), readed) ;
            
            uint32_t flag = JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY ;

            JSValue func = JS_Eval(ctx, script.c_str(), readed, dist.c_str(), flag) ; 
            size_t bytecode_len;
            uint8_t * bytecode = JS_WriteObject(ctx, &bytecode_len, func, JS_WRITE_OBJ_BYTECODE);

            bool res = FS::writeFileSync(dist.c_str(), (const char *)bytecode, bytecode_len, false) ;
            free(bytecode) ;
            if(!res) {
                JSTHROW("write file failed: %s", dist.c_str())
            }

            return JS_UNDEFINED ;
        }
        static JSValue exportValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            ASSERT_ARGC(3)
            JSModuleDef * mdef = JS_FindModuleWithNS(ctx,argv[0]) ;
            if(!mdef) {
                JSTHROW("Invalid module") ;
            }
            const char * valueName = JS_ToCString(ctx, argv[1]) ;

            if( JS_SetModuleExport(ctx, mdef, valueName, JS_DupValue(ctx, argv[2])) !=0 ){
                JS_FreeCString(ctx, valueName) ;
                JSTHROW("Cannot export value: %s", valueName) ;
            }
                
            JS_FreeCString(ctx, valueName) ;
            return JS_UNDEFINED ;
        }

        static JSValue allModuleNames(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            JSValue arr = JS_NewArray(ctx) ;
            ModuleLoader * mloader = & JSEngine::fromJSContext(ctx)->mloader ;
            int i = 0 ;
            for (const auto & pair : mloader->modules[ctx]) {
                JS_SetPropertyUint32(ctx, arr, i++, JS_NewString(ctx, pair.first.c_str())) ;
            }
            return arr ;
        }
        
    } ;

    ModuleLoader::ModuleLoader(BeShell * beshell) {
        add<JSLoader>(beshell, "loader") ;
        add<Process>(beshell, "process") ;
        add<TelnetModule>(beshell, "telnet") ;
#ifdef ESP_PLATFORM
        add<driver::DriverModule>(beshell, "driver") ;
        add<GPIO>(beshell, "gpio") ;
#endif
    }
    
    ModuleLoader::~ModuleLoader() {
        // @todo  delete native modules
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
        }
    }

    std::string ModuleLoader::resovleFS(const char * module_name, const char * base_dir) {

        std::string fullpath = base_dir? base_dir: "" ;
        fullpath+= "/" ;
        fullpath+= module_name ;

        // to vfs path
        fullpath = FS::toVFSPath(fullpath.c_str()) ;
        path_normalize(fullpath) ;

        if(FS::isFile(fullpath.c_str())) {
            return fullpath ;
        }

        if(FS::isDir(fullpath.c_str())) {
            // @todo package

            // index.js
            fullpath+= "/index.js" ;
            if(FS::isFile(fullpath.c_str())) {
                return fullpath ;
            }
        }
        
        std::string trypath = fullpath + ".js" ;
        if(FS::isFile(trypath.c_str())) {
            return trypath ;
        }

        trypath = fullpath + ".bin" ;
        if(FS::isFile(trypath.c_str())) {
            return trypath ;
        }
        
        trypath = trypath + ".js.bin" ;
        if(FS::isFile(trypath.c_str())) {
            return trypath ;
        }

        return std::string("") ;
    }

    char * ModuleLoader::normalize(JSContext *ctx, const char *module_base_name, const char *module_name, void *opaque) {

        if(!opaque) {
            printf("invalid opaque in ModuleLoader::normalize()\n") ;
            return nullptr ;
        }
        
        ModuleLoader * mloader = (ModuleLoader *)opaque ;

        if(mloader->modules.count(ctx)<1) {
            printf("invalid ctx in ModuleLoader::normalize()\n") ;
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
    
        std::string fullpath ;
        // 绝对路
        if(module_name[0]=='/') {
            fullpath = resovleFS(module_name, NULL) ;
        }

        // 相对路径
        else if(module_base_name && (strncmp(module_name,"./",2)==0 || strncmp(module_name,"../",3)==0) ) {
            char * base_dir = strdup(module_base_name) ;
            path_dirname(module_base_name, base_dir) ;
            fullpath = resovleFS(module_name, base_dir) ;
            free(base_dir) ;
        }

        // 系统默认目录
        else {
            fullpath = resovleFS(module_name, "/lib/local") ;
            if(!fullpath.length()) {
                fullpath = resovleFS(module_name, "/opt") ;
            }
        }
        
        if(!fullpath.length()) {
            JS_ThrowReferenceError(ctx, " Cannot find module: %s", module_name) ;
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

        // 将 bin 文件内的路径改为实际路径, 并添加到已加载module队列
        if(asBin) {
            JS_FreeAtom(ctx, m->module_name) ;
            m->module_name = JS_NewAtom(ctx, path) ;
            list_add_tail(&m->link, &ctx->loaded_modules);
        }

        return m;
    }
}
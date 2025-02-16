#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "qjs_utils.h"
#include <sys/stat.h>
#include <string.h>
#include "debug.h"
#include "js/json.c"
#include "ModuleLoader.hpp"
#include "EventEmitter.hpp"
#include "module/Process.hpp"
#include <cassert>
#include <iostream>
#include <iomanip>

#ifdef ESP_PLATFORM
#include "malloc_funcs.h"
#endif

using namespace std ;

#define LOOPTYPE_FUNC 1
#define LOOPTYPE_OBJ 2

#define LOOPING_ADD 1
#define LOOPING_REMOVE 2

namespace be {

    static JSValue js_ArrayBuffer_asString(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, this_val) ;
        if(!buff || !size) {
            return JS_NewStringLen(ctx, NULL, 0) ;
        }
        return JS_NewStringLen(ctx, buff, size) ;
    }
    static JSValue js_ArrayBuffer_toArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        bool signed_int = false ;
        if(argc>0) {
            signed_int = JS_ToBool(ctx, argv[0]) ;
        }
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, this_val) ;
        JSValue array = JS_NewArray(ctx) ;
        if(buff) {
            for(int i=0;i<(int)size;i++) {
                if(signed_int) {
                    JS_SetPropertyUint32(ctx, array, i, JS_NewInt32(ctx, (int8_t)buff[i])) ;
                } else {
                    JS_SetPropertyUint32(ctx, array, i, JS_NewUint32(ctx, buff[i])) ;
                }
            }
        }
        return array ;
    }


    JSEngine::JSEngine(BeShell * beshell)
        : beshell(beshell)
        , mloader(beshell)
    {
    }

    void JSEngine::setup() {
        if(rt!=NULL) {
            return ;
        }
        
#ifdef ESP_PLATFORM
        
        xSemaphore = xSemaphoreCreateBinary();
        give() ;

        // // esp32 平台优先使用 PSRAM内存
        // if( getPsramTotal()>1024 ) {
        //     static const JSMallocFunctions def_malloc_funcs = {
        //         js_def_malloc,
        //         js_def_free,
        //     js_def_realloc,
        //         malloc_usable_size,
        //     };
        //     rt = JS_NewRuntime2(&def_malloc_funcs, NULL);
        // }
        // else {
        //     rt = JS_NewRuntime();
        // }
        
        rt = JS_NewRuntime();
#else
        rt = JS_NewRuntime();
#endif

        // js_std_set_worker_new_context_func(SetupContext);
        // js_std_init_handlers(rt);
        
        JS_SetRuntimeOpaque2(rt, this) ;

        // mloader.init(rt) ;
        
        ctx = SetupContext(rt);
    }

    JSContext * JSEngine::SetupContext(JSRuntime *rt) {

        JSContext *ctx;
        ctx = JS_NewContextRaw(rt);
        if (!ctx) {
            return NULL;
        }

        JS_AddIntrinsicBaseObjects(ctx);
        JS_AddIntrinsicDate(ctx);
        JS_AddIntrinsicEval(ctx);
        JS_AddIntrinsicStringNormalize(ctx);
        JS_AddIntrinsicRegExp(ctx);
        JS_AddIntrinsicJSON(ctx);
        JS_AddIntrinsicProxy(ctx);
        JS_AddIntrinsicMapSet(ctx);
        JS_AddIntrinsicTypedArrays(ctx);
        JS_AddIntrinsicPromise(ctx);
#ifdef CONFIG_BIGNUM
        JS_AddIntrinsicBigInt(ctx);
#endif


        // global 对象
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, "global", global);
        
        JSValue ArrayBufferProto = js_get_prop(ctx, global, 2, "ArrayBuffer", "prototype") ;
        JS_SetPropertyStr(ctx, ArrayBufferProto, "asString", JS_NewCFunction(ctx, js_ArrayBuffer_asString, "asString", 0));
        JS_SetPropertyStr(ctx, ArrayBufferProto, "toArray", JS_NewCFunction(ctx, js_ArrayBuffer_toArray, "toArray", 0));
	    JS_FreeValue(ctx, ArrayBufferProto);

        JSEngine * engine = JSEngine::fromJSRuntime(rt) ;
        assert(engine) ;

        engine->timer.setup(ctx) ;
        engine->mloader.setup(ctx) ;

        engine->console = new Console(ctx) ;
        setGlobalValue(ctx, "console", engine->console->jsobj) ;

        JSEngineEvalEmbeded(ctx, json)
        

        NativeClass::defineClass(ctx) ;
        setGlobalValue(ctx, "NativeClass", JS_DupValue(ctx,NativeClass::getClass(ctx,NativeClass::classID)) ) ;

        EventEmitter::defineClass(ctx) ;
        setGlobalValue(ctx, "EventEmitter", JS_DupValue(ctx,NativeClass::getClass(ctx,EventEmitter::classID)) ) ;

        return ctx;
    }

    void JSEngine::loop() {

        take() ;

        inLooping = true ;
        
        for(struct Looping& looping : lstLoopings) {
            if(looping.priorityCount) {
                looping.priorityCount -- ;
            } else {
                looping.priorityCount = looping.priority ;

                switch(looping.type) {
                    case LOOPTYPE_FUNC:
                        looping.exec.func.function(ctx, looping.exec.func.opaque) ;
                        break ;
                    case LOOPTYPE_OBJ:
                        looping.exec.obj->loop(ctx) ;
                        break ;
                    default:
                        break ;
                }
            }
        }

        timer.loop(ctx) ;
        
        js_std_loop(ctx) ;

        // gc
        static int gc_count = 0 ;
        while(gc_count++>100) {
            gc_count = 0 ;
            JS_RunGC(JS_GetRuntime(ctx)) ;
        }
        
        inLooping = false ;

        // 执行添加/移除 loopable 对象的异步操作请求
        // for(auto op : waitingLoopingOps) {
        //     if(op.op==LOOPING_ADD) {
        //         if( op.target.add.looping.type==LOOPTYPE_FUNC ) {
        //             addLoopFunction(op.target.add.looping.exec.func.function, op.target.add.looping.exec.func.opaque, op.target.add.ignoreRepeat, op.target.add.looping.priority) ;
        //         }
        //         else if( op.target.add.looping.type==LOOPTYPE_OBJ ) {
        //             addLoopObject(op.target.add.looping.exec.obj, op.target.add.ignoreRepeat, op.target.add.looping.priority) ;
        //         }
        //     }
        //     else if(op.op==LOOPING_REMOVE) {
        //         removeLooping(op.target.remove) ;
        //     }
        // }
        // waitingLoopingOps.clear() ;

        give() ;
    }

    JSEngine * JSEngine::fromJSContext(JSContext * ctx) {
        return (JSEngine *)JS_GetRuntimeOpaque2( JS_GetRuntime(ctx) ) ;
    }
    JSEngine * JSEngine::fromJSRuntime(JSRuntime * rt) {
        return (JSEngine *)JS_GetRuntimeOpaque2(rt) ;
    }

    void JSEngine::print(JSValue content, int pkgId, uint8_t cmd, TelnetChannel * ch) {
        assert(beshell) ;

        std::string str = console->stringify(ctx, content) ;

        if(!str.length()) {
            return ;
        }
        if(ch) {
            ch->send(str.c_str(), str.length(), pkgId, cmd) ;
        } else if(beshell->telnet) {
            beshell->telnet->output(str.c_str(), str.length(), pkgId, cmd) ;
        } else {
            cout << str ;
        }
    }

    void JSEngine::dumpError(int pkgId, TelnetChannel * ch) {
        JSValue excep = JS_GetException(ctx);
        if(JS_IsNull(excep) || JS_IsUndefined(excep)) {
            return ;
        }

        string str = getExceptionStr(ctx, excep) ;
        if(ch) {
            ch->send(str.c_str(), str.length(), pkgId, EXCEPTION) ;
        } else if(beshell->telnet) {
            beshell->telnet->output(str.c_str(), str.length(), pkgId, EXCEPTION) ;
        }

        JS_FreeValue(ctx, excep);
    }
    
    string JSEngine::getExceptionStr(JSContext * ctx, JSValue exception_val) {
        const char * cstr = JS_ToCString(ctx, exception_val) ;
        if(!cstr) {
            return std::string("") ;
        }
        std::string str = cstr ;
        JS_FreeCString(ctx, cstr) ;
        // std::string str = console->stringify(ctx, exception_val) ;

        bool is_error = JS_IsError(ctx, exception_val);
        if (is_error) {
            JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
            if (!JS_IsUndefined(val)) {
                cstr = JS_ToCString(ctx, val) ;
                str+= "\n";
                str+= cstr;
                JS_FreeCString(ctx,cstr) ;
            }
            JS_FreeValue(ctx, val);
        }
        return str ;
    }

    JSValue JSEngine::eval(const char * code, int code_len,const char * filename, int flags) {
        if(code_len<0) {
            code_len = strlen(code) ;
        }
        return JS_Eval(ctx, code, code_len, filename, flags) ;   // JS_EVAL_FLAG_STRIP
    }

    JSValue JSEngine::evalScript(const char * filepath, int flags, bool dumpException) {
        assert(beshell) ;
        
        int readed ;
        unique_ptr<char> content = FS::readFileSync(filepath,&readed) ;
        if(readed<0) {
            JSTHROW("read file failed")
        }

        string code(content.get(), readed) ;

        JS_Eval(ctx, code.c_str(), code.length(), filepath, flags) ;
        JSValue ret = JS_GetException(ctx) ;
        if(dumpException && !JS_IsNull(ret) && !JS_IsUndefined(ret)) {
            string str = getExceptionStr(ctx, ret) ;
            printf("%s\n", str.c_str()) ;
            JS_FreeValue(ctx, ret) ;
            ret = JS_UNDEFINED ;
        }
        return ret ;
    }

    void JSEngine::setGlobalValue(JSContext * ctx, const char * name, JSValue value) {
        JSValue global = JS_GetGlobalObject(ctx) ; 
        JS_SetPropertyStr(ctx, global, name, value) ;
        JS_FreeValue(ctx,global) ;
    }
    
    
    int32_t JSEngine::findLooping(EngineLoopFunction func, void * opaque) {
        for(struct Looping& looping : lstLoopings) {
            if( looping.type==LOOPTYPE_FUNC && looping.exec.func.function==func && looping.exec.func.opaque==opaque ) {
                return looping.id ;
            }
        }
        return -1 ;
    }
    int32_t JSEngine::findLooping(EngineLoopFunction func) {
        for(struct Looping& looping : lstLoopings) {
            if( looping.type==LOOPTYPE_FUNC && looping.exec.func.function==func ) {
                return looping.id ;
            }
        }
        return -1 ;
    }
    int32_t JSEngine::findLooping(ILoopable* obj) {
        for(struct Looping& looping : lstLoopings) {
            if( looping.type==LOOPTYPE_OBJ && looping.exec.obj==obj ) {
                return looping.id ;
            }
        }
        return -1 ;
    }

    int32_t JSEngine::addLoopFunction(EngineLoopFunction func, void * opaque, bool ignoreRepeat, uint8_t priority) {
        if(ignoreRepeat && findLooping(func, opaque)>=0)  {
            return -1 ;
        }

        struct Looping looping = {
            LOOPTYPE_FUNC,
            {
                .func = {
                    .function = func,
                    .opaque = opaque
                } ,
            } ,
            loopingAssignedId ++,
            priority,
            0
        } ;

        
        // if(inLooping) {
        //     waitingLoopingOps.push_back({
        //         .op=LOOPING_ADD, .target={ .add={ .ignoreRepeat=ignoreRepeat, .looping=looping} }
        //     }) ;
        // } else {
            lstLoopings.push_back(looping) ;
        // }

        return looping.id ;
    }

    int32_t JSEngine::addLoopObject(ILoopable* obj, bool ignoreRepeat, uint8_t priority) {
        if(ignoreRepeat) {
            for(struct Looping& looping : lstLoopings) {
                if( looping.type==LOOPTYPE_OBJ && looping.exec.obj==obj ) {
                    return -1 ;
                }
            }
        }

        struct Looping looping = {
            LOOPTYPE_OBJ,
            {
                .obj = obj
            } ,
            loopingAssignedId ++,
            priority,
            0
        } ;
        
        // if(inLooping) {
        //     waitingLoopingOps.push_back({
        //         .op=LOOPING_ADD, .target={ .add={ .ignoreRepeat=ignoreRepeat, .looping=looping} }
        //     }) ;
        // } else {
            lstLoopings.push_back(looping) ;
        // }

        return looping.id ;
    }

    void JSEngine::removeLooping(int32_t id) {
        if(id<0) {
            return ;
        }

        // if(inLooping) {
        //     waitingLoopingOps.push_back({
        //         .op=LOOPING_REMOVE, .target={ .remove=(uint16_t)id }
        //     }) ;
        // }
        // else {
            auto it = std::find_if(lstLoopings.begin(), lstLoopings.end(), [id](struct Looping& looping) {
                return looping.id==id ;
            }) ;
            if(it!=lstLoopings.end()) {
                lstLoopings.erase(it) ;
            }
        // }
    }
    
    void JSEngine::removeLooping(EngineLoopFunction func, void * opaque) {
        removeLooping(findLooping(func, opaque)) ;
    }
    void JSEngine::removeLooping(EngineLoopFunction func) {
        removeLooping(findLooping(func)) ;
    }
    void JSEngine::removeLooping(ILoopable* obj) {
        removeLooping(findLooping(obj)) ;
    }

    bool JSEngine::take(int timeout) {
        return xSemaphoreTake(xSemaphore, timeout) == pdTRUE ;
    }
    void JSEngine::give() {
        xSemaphoreGive(xSemaphore);
    }
}
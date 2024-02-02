#include "./Console.hpp"
#include "utils.h"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include <assert.h>

using namespace std ;

namespace be {

    DEFINE_NCLASS_META(Console)

    std::vector<JSCFunctionListEntry> Console::methods = {
        JS_CFUNC_DEF("write", 1, jsWrite),
    };

    Console::Console(JSContext * ctx)
        : NativeClass(ctx)
    {
        JSEngine * engine = JSEngine::fromJSContext(ctx) ;
        assert(engine) ;

        DEF_JS_FUNC(jsStringify, R"(
function (value,format) {
    let pool = []
    let str = JSON.stringify(value, (key,value)=>{
        if(value===null) {
            return value
        }
        if( typeof value=='object' || typeof value=='function' ) {
            let idx = pool.indexOf(value)
            if( idx>=0 ) {
                return '<circular reference>#'+idx
            }
            pool.push(value)
            if(typeof value=='function') {
                return (value.name||'')+'<function>'
            }
        }
        return value
    },format? 2: 0)
    pool.length = 0
    return str
}
    )", "console.js", {
            engine->dumpError() ;
            return ;
        })

    JSValue DEF_JS_FUNC(jsEmit, R"(
function() {
    console.message(JSON.stringify(Object.values(arguments)))
}
    )", "console.js", {
            engine->dumpError() ;
            return ;
        })

        JSValue DEF_JS_FUNC(jsLog, R"(
function() {
    let vals = []
    for(let v of arguments){
        switch(typeof v) {
            case 'number':
            case 'string':
            case 'boolean':
            case 'function':
                vals.push(v)
                break
            default:
                if(v instanceof Error) {
                    vals.push(v.message+'\n'+v.stack)
                }
                else {
                    vals.push(console.stringify(v))
                }
                break
        }
    }
    console.write(vals.join(' ')+'\n')
}
    )", "console.js", {
            engine->dumpError() ;
            return ;
        })

    // dref(jsStringify)
    // const char * eeee = JS_ToCString(ctx, jsStringify) ;
    // ds(eeee) ;
    // JS_FreeCString(ctx, eeee) ;
    // dref(jsStringify)

        JS_DupValue(ctx, jsStringify) ;
        JS_SetPropertyStr(ctx, jsobj, "stringify", jsStringify) ;
    // dref(jsStringify)

    // const char * ttt = JS_ToCString(ctx, jsStringify) ;
    // ds(ttt) ;
    // JS_FreeCString(ctx, ttt) ;


        JS_SetPropertyStr(ctx, jsobj, "log", jsLog) ;
        JS_SetPropertyStr(ctx, jsobj, "emit", jsEmit) ;
    }

    JSValue Console::jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
dd
        if(argc<1) {
            return JS_UNDEFINED ;
        }

        ARGV_TO_STRING(0,str)
        
        JSEngine * engine = JSEngine::fromJSContext(ctx) ;
        assert(engine) ;
        assert(engine->beshell) ;
        assert(engine->beshell->telnet) ;

        engine->beshell->telnet->output(str.c_str(), str.length()) ;

        return JS_UNDEFINED ;
    }

    #undef stringify
    string Console::stringify(JSContext *ctx, JSValue val) {
dd
        const char * cstr = nullptr ;
        // 简单调用 toString
        if( JS_IsUndefined(jsStringify) || JS_IsNull(jsStringify) ) {
            dd
            cstr = JS_ToCString(ctx,val) ;
        }
        // 深度 stringify
        else {
            dd


    const char * eeee = JS_ToCString(ctx, jsStringify) ;
    ds(eeee) ;
    JS_FreeCString(ctx, eeee) ;

    dref(jsStringify) ;


            JSValue ret = JS_Call(ctx, jsStringify, JS_UNDEFINED, 1, &val) ;
            if(JS_IsException(ret)) {
                dd
                JSEngine * engine = JSEngine::fromJSContext(ctx) ;
                assert(engine) ;
                engine->dumpError() ;
            }
            else {
                dd
                cstr = JS_ToCString(ctx,ret) ;
                JS_FreeValue(ctx, ret) ;
            }
        }

        string str = cstr? cstr: "" ;
        JS_FreeCString(ctx, cstr) ;
dd
        return str ;
    }
}

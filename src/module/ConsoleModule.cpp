#include "ConsoleModule.hpp"
#include "JSEngine.hpp"
#include "BeShell.hpp"
#include "NativeObject.hpp"
#include "debug.h"
#include "utils.h"
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>


using namespace std ;

namespace be {

    NativeModule* ConsoleModule::factory(JSContext * ctx, const char * name) {
        return new ConsoleModule(ctx,name,1) ;
    }
    ConsoleModule::ConsoleModule(JSContext * ctx, const char * name, uint8_t flagGlobal)
        : NativeModule(ctx,name,flagGlobal)
    {
        jsStringify = JS_NULL ;

        JSValue jsconsole = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, jsconsole, "write", JS_NewCFunction(ctx, jsWrite, "write", 1)) ;
    }
    
    void ConsoleModule::setup(JSContext * ctx) {

        return ;
        // JSEngine::setGlobalValue(ctx, "console", jsconsole) ;

        JSEngine * engine = JSEngine::fromJSContext(ctx) ;
        assert(engine) ;
        
        const char * code = R"(
console.stringify = function (value,format) {
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
    )" ;
        jsStringify = JS_Eval(ctx, code, strlen(code), "console.js", JS_EVAL_TYPE_GLOBAL) ;
        if(JS_IsException(jsStringify)) {
            engine->dumpError() ;
            return ;
        }
        assert( JS_IsFunction(ctx, jsStringify) ) ;

        
        code = R"(
console.emit = function() {
    console.message(JSON.stringify(Object.values(arguments)))
}
console.log = function() {
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
    console.write(vals.join(' ')+'\n
}
console.error = console.log
        )" ;
        JSValue ret = JS_Eval(ctx, code, strlen(code), "console.js", JS_EVAL_TYPE_GLOBAL) ;
        if(JS_IsException(ret)) {
            engine->dumpError() ;
            return ;
        }
    }
    
    JSValue ConsoleModule::jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
    string ConsoleModule::stringify(JSContext *ctx, JSValue val) {

        const char * cstr ;
        // 简单调用 toString
        if( JS_IsNull(jsStringify) ) {
            cstr = JS_ToCString(ctx,val) ;
        }
        // 深度 stringify
        else {
            JSValue ret = JS_Call(ctx, jsStringify, JS_UNDEFINED, 1, &val) ;
            if(JS_IsException(ret)) {
                JSEngine * engine = JSEngine::fromJSContext(ctx) ;
                assert(engine) ;
                engine->dumpError() ;
            }
            else {
                cstr = JS_ToCString(ctx,ret) ;
                JS_FreeValue(ctx, ret) ;
            }
        }

        string str = cstr? cstr: "" ;
        JS_FreeCString(ctx, cstr) ;

        return str ;
    }
}
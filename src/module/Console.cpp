#include "Console.hpp"
#include "JSEngine.hpp"
#include "debug.h"
#include "utils.h"
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>


using namespace std ;

namespace be {
    Console::Console(): NativeModule("console") {
        isGlobal = true ;
        isReplGlobal = true ;

        jsStringify = JS_NULL ;

        exportFunction("write",write) ;
        exportFunction("print",write) ;
    }
    Console::~Console() {

    }
    
    JSValue Console::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(argc<1) {
            return JS_UNDEFINED ;
        }

        JSEngine * engine = JSEngine::fromJSContext(ctx) ;
        assert(engine) ;

        engine->print(argv[0]) ;

        return JS_UNDEFINED ;
    }
    // JSValue Console::log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    //     return JS_UNDEFINED ;
    // }
    
    void Console::onAdded(JSContext *ctx) {
        JSEngine * engine = JSEngine::fromJSContext(ctx) ;
        assert(engine) ;

        const char * code = R"(
console.stringify = function(value,format) {
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
    console.write(vals.join(' ')+'\n')
}
console.error = console.log
        )" ;
        size_t codelen = strlen(code) ;
        JSValue ret = JS_Eval(ctx, code, codelen, "console.js", JS_EVAL_TYPE_GLOBAL) ;
        if(JS_IsException(ret)) {
            engine->dumpError() ;
            return ;
        }

        jsStringify = js_get_glob_prop(ctx, 2, "console", "stringify") ;
        JS_FreeValue(ctx, jsStringify) ;
    }

    #undef stringify
    string Console::stringify(JSContext *ctx, JSValue val) {
        
        const char * cstr ;
        // 简单调用 toString
        if( JS_IsNull(jsStringify) ) {
            cstr = JS_ToCString(ctx,val) ;
        }
        // 深度 stringify
        else {
            JSValue ret = JS_Call(ctx, jsStringify, JS_UNDEFINED, 1, &val) ;
            cstr = JS_ToCString(ctx,ret) ;
            JS_FreeValue(ctx, ret) ;
        }
        
        string str = cstr ;
        JS_FreeCString(ctx, cstr) ;

        return str ;
    }
}
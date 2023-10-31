/**
 * @file runtime.c
 * @copyright become.cool
 * 
 */

/*********************
 *      INCLUDES
 *********************/
#include "runtime.h"
#include "utils.h"
#include <sys/stat.h>
#include "module_fs.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


void evalScript(JSContext *ctx, const char * path, bool asBin, bool es6Module) {

    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        printf("path not exist: %s\n", path); ;
        return ;
    }
    if(S_ISDIR(statbuf.st_mode)){
        return ;
    }
    char * buff = malloc(statbuf.st_size+1) ;
    if(!buff) {
        return ;
    }
	FILE * fd = fopen(path, "r") ;
    if(fd==NULL) {
        printf("open file failed: %s\n", path); ;
        return ;
    }

    int readedBytes = fread(buff, 1, statbuf.st_size, fd) ;
    fclose(fd) ;
    buff[readedBytes] = 0 ;

    if(asBin) {
        JSValue func = JS_ReadObject(ctx, (uint8_t*)buff, readedBytes, JS_READ_OBJ_BYTECODE);
        JSValue ret = JS_EvalFunction(ctx, func);
        if(JS_IsException(ret)) {
            echo_error(ctx) ;
        }
        JS_FreeValue(ctx, ret) ;
    }
    else {
        eval_code_len(ctx, buff, readedBytes, path, es6Module) ;
    }

    free(buff) ;
}

static JSValue js_eval_script(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    JS2VSFPath(path, argv[0])
    CHECK_ARGV0_NOT_DIR(path)
    evalScript(ctx, path, false, false) ;
    free(path) ;
	return JS_UNDEFINED ;
}


static JSValue js_eval_as_file(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    ARGV_TO_STRING_LEN_E(0, code, codelen, "argv code must be a string")
    ARGV_TO_STRING_E(1, path, "argv path must be a string")

    bool es6Module = false ;
    if(argc>2) {
        es6Module = JS_ToBool(ctx, argv[2]) ;
    }
    
    JSValue ret = JS_Eval(ctx, code, codelen, path, es6Module?JS_EVAL_TYPE_MODULE:JS_EVAL_TYPE_GLOBAL) ; // JS_EVAL_TYPE_GLOBAL, JS_EVAL_TYPE_MODULE, JS_EVAL_FLAG_STRIP

    JS_FreeCString(ctx, code) ;
	JS_FreeCString(ctx, path) ;

    // JS_DupValue(ctx, ret) ;
    return ret ;
}

static JSValue js_compile_script(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    ARGV_TO_STRING_LEN_E(0, code, codelen, "argv code must be a string")
    ARGV_TO_STRING_E(1, path, "argv path must be a string")

    uint32_t flag = JS_EVAL_TYPE_GLOBAL|JS_EVAL_FLAG_COMPILE_ONLY ;
    if(argc>2 && JS_ToBool(ctx, argv[2])) {
        flag|= JS_EVAL_FLAG_STRIP ;
    }

    JSValue func = JS_Eval(ctx, code, codelen, path, flag) ; 

	JS_FreeCString(ctx, code) ;
	JS_FreeCString(ctx, path) ;

    if( JS_IsException(func) ) {
        JS_FreeValue(ctx,func) ;
        return JS_EXCEPTION ;
    }

    size_t bytecode_len;
    uint8_t * bytecode = JS_WriteObject(ctx, &bytecode_len, func, JS_WRITE_OBJ_BYTECODE);
    JS_FreeValue(ctx,func) ;

    return JS_NewArrayBuffer(ctx, bytecode, bytecode_len, freeArrayBuffer, NULL, false ) ; ;
}

static JSValue js_eval_bin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_ARRAYBUFFER(0, bytecodes, bytelen)

    JSValue func = JS_ReadObject(ctx, bytecodes, bytelen, JS_READ_OBJ_BYTECODE);
    JSValue ret = JS_EvalFunction(ctx, func);

    return ret ;
}


/**********************
 *   REGISTER
 **********************/

void be_runtime_init() {}

void be_runtime_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);

    JS_SetPropertyStr(ctx, global, "evalScript", JS_NewCFunction(ctx, js_eval_script, "evalScript", 1));
    JS_SetPropertyStr(ctx, global, "evalAsFile", JS_NewCFunction(ctx, js_eval_as_file, "evalAsFile", 1));
    JS_SetPropertyStr(ctx, global, "compileScript", JS_NewCFunction(ctx, js_compile_script, "compileScript", 1));
    JS_SetPropertyStr(ctx, global, "evalBin", JS_NewCFunction(ctx, js_eval_bin, "evalBin", 1));

	JS_FreeValue(ctx, global);
}

void be_runtime_loop(JSContext *ctx) {}

void be_runtime_reset(JSContext *ctx) {}


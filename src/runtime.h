/**
 * @file runtime.h
 * @copyright become.cool
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "quickjs-libc.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void evalScript(JSContext *ctx, const char * path, bool asBin, bool es6Module) ;

/**********************
 *      REGISTER
 **********************/
void be_runtime_init() ;
void be_runtime_require(JSContext *ctx) ;
void be_runtime_loop(JSContext *ctx) ;
void be_runtime_reset(JSContext *ctx) ;

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

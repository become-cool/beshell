#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <iostream>

namespace be {
    class NativeObject ;
    class NativeClass {
    private:
    public:

        JSClassID & classID ;

        JSContext * ctx ;
        JSValue constructor = JS_NULL ;
        JSValue proto = JS_NULL ;
        JSValue parentProto = JS_NULL ;

        bool defined = false ;

        NativeClass(
            JSContext * ctx
            , const char * name
            , JSClassID & classID
            , JSClassFinalizer * finalizer
        ) ;
        ~NativeClass() ;
        
        void setParent(JSContext * ctx, NativeClass * parent) ;

        static NativeClass * fromClassID(JSContext * ctx, JSClassID & classID) {
            JS_NewClassID(&classID);

            if (mapClasses.count(ctx) < 1) {
                std::cout << "new ctx nclass pool" << std::endl ;
                mapClasses[ctx] = std::map<JSClassID, NativeClass *>() ;
            }

            if (mapClasses.count(ctx)<0) {
                return nullptr ;
            }

            return mapClasses[ctx][classID] ;
        }

        JSValue newJSObject(JSContext * ctx) {
            return  JS_NewObjectClass(ctx, classID) ;
        }

        friend class NativeObject ;
    private:
        static std::map<JSContext*, std::map<JSClassID, NativeClass *>> mapClasses ;

    } ;


	typedef void (*NativeClassDefineFunc)(NativeClass *) ;

    class NativeObject {
    private:
    protected:
        NativeClass * nclass;
        JSContext * ctx ;
        JSValue jsobj ;
    public:
        NativeObject(
            JSContext * _ctx
            , JSClassID classID
            , const char * name
            , NativeClassDefineFunc funcDefineClass=nullptr
            , NativeObject * parent=nullptr
        )
            : nclass( NativeClass::fromClassID(_ctx, classID) )
            , ctx(_ctx)
        {
            if(!nclass) {

                nclass = new NativeClass(_ctx, name, classID, jsFinalizer) ;

                JSValue jscotr = JS_NewCFunction2(_ctx, jsConstructor, name, 1, JS_CFUNC_constructor, 0) ;
                JS_SetConstructor(_ctx, jscotr, nclass->proto) ;

                if(parent) {
                    nclass->setParent(ctx,parent->nclass) ;
                }
                
                if(funcDefineClass) {
                    funcDefineClass(nclass) ;
                }
            }

            jsobj = nclass->newJSObject(ctx) ;
            JS_DupValue(ctx, jsobj) ;
        }
        
        ~NativeObject() {
            JS_FreeValue(ctx, jsobj) ;
        }
        
        virtual void defineClass(int argc, JSValueConst * argv) ;
        virtual void constructor(int argc, JSValueConst * argv) ;
        virtual void finalizer() ;

        private:
            static JSValue jsConstructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
                return JS_UNDEFINED ;
            }
            static void jsFinalizer(JSRuntime *rt, JSValue val) {

            }

    } ;


    class I2C: public NativeObject {
    private:
        uint8_t busnum = 0 ;
    public:
        I2C(JSContext * ctx, uint8_t _busnum=0)
          : NativeObject(ctx, classID, "I2C", defineClass)
          , busnum(_busnum)
        {
        }

        static void defineClass(NativeClass *) {

        }

        static JSClassID classID ;
    } ;
}
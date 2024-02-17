#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Table: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Table(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Table(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getRowCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getColumnCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setColumnCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsAddCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}

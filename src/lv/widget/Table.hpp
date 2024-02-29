#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Table: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Table(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Table(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getRowCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getColumnCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setColumnCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setCellValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCellValueFmt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setColumnWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clearCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCellUserData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getCellValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getColumnWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSelectedCell(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getCellUserData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}

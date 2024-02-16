#include "Table.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Table, Obj)
    std::vector<JSCFunctionListEntry> Table::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("rowCount",Table::getRowCount,Table::setRowCount) ,
        JS_CGETSET_DEF("columnCount",Table::getColumnCount,Table::setColumnCount) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addCellCtrl", 3, Table::jsAddCellCtrl),
        JS_CFUNC_DEF("clearCellCtrl", 3, Table::jsClearCellCtrl),
        JS_CFUNC_DEF("hasCellCtrl", 3, Table::jsHasCellCtrl),

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Table::Table(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Table::Table(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_table_create(parent))
    {}
        
    JSValue Table::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Table * widget = new Table(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Table::getRowCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Table,thisobj)
        uint32_t value = lv_table_get_row_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Table::setRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Table,thisobj)
        uint32_t rowCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &rowCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rowCount","Table","setRowCount","number")
        }
        lv_table_set_row_count(thisobj->lvobj(), rowCount) ;
        return JS_UNDEFINED ;
    }
    JSValue Table::getColumnCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Table,thisobj)
        uint32_t value = lv_table_get_column_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Table::setColumnCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Table,thisobj)
        uint32_t columnCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &columnCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","columnCount","Table","setColumnCount","number")
        }
        lv_table_set_column_count(thisobj->lvobj(), columnCount) ;
        return JS_UNDEFINED ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue Table::jsAddCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            uint32_t row ;
            if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","row","Table","addCellCtrl","number")
            }
            uint32_t col ;
            if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","col","Table","addCellCtrl","number")
            }
            uint8_t ctrl ;
            if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","ctrl","Table","addCellCtrl","number")
            }
            lv_table_add_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
            return JS_UNDEFINED ;
        }

        JSValue Table::jsClearCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            uint32_t row ;
            if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","row","Table","clearCellCtrl","number")
            }
            uint32_t col ;
            if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","col","Table","clearCellCtrl","number")
            }
            uint8_t ctrl ;
            if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","ctrl","Table","clearCellCtrl","number")
            }
            lv_table_clear_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
            return JS_UNDEFINED ;
        }

        JSValue Table::jsHasCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            uint32_t row ;
            if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","row","Table","hasCellCtrl","number")
            }
            uint32_t col ;
            if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","col","Table","hasCellCtrl","number")
            }
            uint8_t ctrl ;
            if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","ctrl","Table","hasCellCtrl","number")
            }
            bool retval = lv_table_has_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

// AUTO GENERATE CODE END [METHODS] --------

}

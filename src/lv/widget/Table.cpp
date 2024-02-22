#include "Table.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Table, Obj)
    std::vector<JSCFunctionListEntry> Table::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("rowCount",Table::getRowCount,Table::setRowCount) ,
        JS_CGETSET_DEF("columnCount",Table::getColumnCount,Table::setColumnCount) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setCellValue", 3, Table::setCellValue),
        JS_CFUNC_DEF("setColumnWidth", 2, Table::setColumnWidth),
        JS_CFUNC_DEF("addCellCtrl", 3, Table::addCellCtrl),
        JS_CFUNC_DEF("clearCellCtrl", 3, Table::clearCellCtrl),
        JS_CFUNC_DEF("getColumnWidth", 1, Table::getColumnWidth),
        JS_CFUNC_DEF("hasCellCtrl", 3, Table::hasCellCtrl),
        // Unsupported arg type:
        // void lv_table_set_cell_value_fmt(lv_obj_t * obj, uint32_t row, uint32_t col, const char * fmt, ...)
        // void lv_table_set_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col, void * user_data)
        // const char * lv_table_get_cell_value(lv_obj_t * obj, uint32_t row, uint32_t col)
        // void lv_table_get_selected_cell(lv_obj_t * obj, uint32_t * row, uint32_t * col)
        // void * lv_table_get_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Table::Table(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Table::build(ctx,jsobj), lvobj)
    {}

    Table::Table(JSContext * ctx, lv_obj_t * parent)
        : Table(ctx, JS_NULL, lv_table_create(parent))
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
    #ifndef GETTER_Table_RowCount
    JSValue Table::getRowCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Table,thisobj)
        uint32_t value = lv_table_get_row_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Table_RowCount
    JSValue Table::setRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Table,thisobj)
        uint32_t rowCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &rowCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rowCount","Table","setRowCount","number")
        }
        lv_table_set_row_count(thisobj->lvobj(), rowCount) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Table_ColumnCount
    JSValue Table::getColumnCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Table,thisobj)
        uint32_t value = lv_table_get_column_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Table_ColumnCount
    JSValue Table::setColumnCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Table,thisobj)
        uint32_t columnCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &columnCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","columnCount","Table","setColumnCount","number")
        }
        lv_table_set_column_count(thisobj->lvobj(), columnCount) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Table_setCellValue
    JSValue Table::setCellValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        uint32_t row ;
        if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","row","Table","setCellValue","number")
        }
        uint32_t col ;
        if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","col","Table","setCellValue","number")
        }
        char * txt = (char *)JS_ToCString(ctx, argv[2]) ;
        lv_table_set_cell_value( thisobj->lvobj(), row, col, txt ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: 
    // void lv_table_set_cell_value_fmt(lv_obj_t * obj, uint32_t row, uint32_t col, const char * fmt, ...)

    #ifndef METHOD_Table_setColumnWidth
    JSValue Table::setColumnWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t col_id ;
        if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","col_id","Table","setColumnWidth","number")
        }
        int32_t w ;
        if(JS_ToInt32(ctx, (int32_t *) &w, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","w","Table","setColumnWidth","number")
        }
        lv_table_set_column_width( thisobj->lvobj(), col_id, w ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Table_addCellCtrl
    JSValue Table::addCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
        // argv ctrl
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_table_cell_ctrl_t ctrl;
        if(!lv_table_cell_ctrl_str_to_const(cstr_argv_2_,&ctrl)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_table_cell_ctrl_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_table_add_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Table_clearCellCtrl
    JSValue Table::clearCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
        // argv ctrl
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_table_cell_ctrl_t ctrl;
        if(!lv_table_cell_ctrl_str_to_const(cstr_argv_2_,&ctrl)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_table_cell_ctrl_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_table_clear_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: void *
    // void lv_table_set_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col, void * user_data)

        // Unsupported return type: const char *
        // const char * lv_table_get_cell_value(lv_obj_t * obj, uint32_t row, uint32_t col)

    #ifndef METHOD_Table_getColumnWidth
    JSValue Table::getColumnWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t col ;
        if(JS_ToUint32(ctx, (uint32_t *) &col, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","col","Table","getColumnWidth","number")
        }
        int32_t retval = lv_table_get_column_width( thisobj->lvobj(), col ) ;
        JSValue jsretval = JS_NewInt32(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Table_hasCellCtrl
    JSValue Table::hasCellCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
        // argv ctrl
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_table_cell_ctrl_t ctrl;
        if(!lv_table_cell_ctrl_str_to_const(cstr_argv_2_,&ctrl)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_table_cell_ctrl_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        bool retval = lv_table_has_cell_ctrl( thisobj->lvobj(), row, col, ctrl ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: uint32_t *
    // void lv_table_get_selected_cell(lv_obj_t * obj, uint32_t * row, uint32_t * col)

        // Unsupported return type: void *
        // void * lv_table_get_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col)
// AUTO GENERATE CODE END [METHODS] --------

}

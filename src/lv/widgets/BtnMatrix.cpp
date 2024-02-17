#include "BtnMatrix.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(BtnMatrix, Obj)
    std::vector<JSCFunctionListEntry> BtnMatrix::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("selectedButton",BtnMatrix::getSelectedButton,BtnMatrix::setSelectedButton) ,
        JS_CGETSET_DEF("buttonCtrlAll",be::lv::Obj::invalidGetter,BtnMatrix::setButtonCtrlAll) ,
        JS_CGETSET_DEF("oneChecked",BtnMatrix::getOneChecked,BtnMatrix::setOneChecked) ,
        JS_CGETSET_DEF("popovers",BtnMatrix::getPopovers,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("clearButtonCtrl", 2, BtnMatrix::jsClearButtonCtrl),
        JS_CFUNC_DEF("clearButtonCtrlAll", 1, BtnMatrix::jsClearButtonCtrlAll),
        JS_CFUNC_DEF("hasButtonCtrl", 2, BtnMatrix::jsHasButtonCtrl),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    BtnMatrix::BtnMatrix(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, BtnMatrix::build(ctx,jsobj), lvobj)
    {}

    BtnMatrix::BtnMatrix(JSContext * ctx, lv_obj_t * parent)
        : BtnMatrix(ctx, JS_NULL, lv_buttonmatrix_create(parent))
    {}
        
    JSValue BtnMatrix::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        BtnMatrix * widget = new BtnMatrix(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const char **
    // JSValue BtnMatrix::getMap(JSContext *ctx, JSValueConst this_val){}
    // const char ** lv_buttonmatrix_get_map(const lv_obj_t * obj)
    // unspported type: const char **
    // JSValue BtnMatrix::setMap(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_buttonmatrix_set_map(lv_obj_t * obj, const char * map[])
    // unspported type: const lv_buttonmatrix_ctrl_t*
    // JSValue BtnMatrix::setCtrlMap(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_buttonmatrix_set_ctrl_map(lv_obj_t * obj, const lv_buttonmatrix_ctrl_t ctrl_map[])
    JSValue BtnMatrix::getSelectedButton(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(BtnMatrix,thisobj)
        uint32_t value = lv_buttonmatrix_get_selected_button(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue BtnMatrix::setSelectedButton(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(BtnMatrix,thisobj)
        uint32_t selectedButton ;
        if(JS_ToUint32(ctx, (uint32_t *) &selectedButton, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","selectedButton","BtnMatrix","setSelectedButton","number")
        }
        lv_buttonmatrix_set_selected_button(thisobj->lvobj(), selectedButton) ;
        return JS_UNDEFINED ;
    }
    JSValue BtnMatrix::setButtonCtrlAll(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(BtnMatrix,thisobj)
        // argv buttonCtrlAll
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_buttonmatrix_ctrl_t buttonCtrlAll;
        if(lv_buttonmatrix_ctrl_str_to_const(cstr_val,&buttonCtrlAll)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_buttonmatrix_ctrl_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_buttonmatrix_set_button_ctrl_all(thisobj->lvobj(), buttonCtrlAll) ;
        return JS_UNDEFINED ;
    }
    JSValue BtnMatrix::getOneChecked(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(BtnMatrix,thisobj)
        bool value = lv_buttonmatrix_get_one_checked(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    JSValue BtnMatrix::setOneChecked(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(BtnMatrix,thisobj)
        bool oneChecked = JS_ToBool(ctx, val) ;
        lv_buttonmatrix_set_one_checked(thisobj->lvobj(), oneChecked) ;
        return JS_UNDEFINED ;
    }
    JSValue BtnMatrix::getPopovers(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(BtnMatrix,thisobj)
        bool value = lv_buttonmatrix_get_popovers(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue BtnMatrix::jsClearButtonCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            uint32_t btn_id ;
            if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","btn_id","BtnMatrix","clearButtonCtrl","number")
            }
            // argv ctrl
            const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
            lv_buttonmatrix_ctrl_t ctrl;
            if(lv_buttonmatrix_ctrl_str_to_const(cstr_argv_1_,&ctrl)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_buttonmatrix_ctrl_t",cstr_argv_1_) ;
                JS_FreeCString(ctx, cstr_argv_1_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_1_) ;
            lv_buttonmatrix_clear_button_ctrl( thisobj->lvobj(), btn_id, ctrl ) ;
            return JS_UNDEFINED ;
        }

        JSValue BtnMatrix::jsClearButtonCtrlAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv ctrl
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_buttonmatrix_ctrl_t ctrl;
            if(lv_buttonmatrix_ctrl_str_to_const(cstr_argv_0_,&ctrl)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_buttonmatrix_ctrl_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_buttonmatrix_clear_button_ctrl_all( thisobj->lvobj(), ctrl ) ;
            return JS_UNDEFINED ;
        }

        JSValue BtnMatrix::jsHasButtonCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            uint32_t btn_id ;
            if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","btn_id","BtnMatrix","hasButtonCtrl","number")
            }
            // argv ctrl
            const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
            lv_buttonmatrix_ctrl_t ctrl;
            if(lv_buttonmatrix_ctrl_str_to_const(cstr_argv_1_,&ctrl)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_buttonmatrix_ctrl_t",cstr_argv_1_) ;
                JS_FreeCString(ctx, cstr_argv_1_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_1_) ;
            bool retval = lv_buttonmatrix_has_button_ctrl( thisobj->lvobj(), btn_id, ctrl ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }
// AUTO GENERATE CODE END [METHODS] --------

}

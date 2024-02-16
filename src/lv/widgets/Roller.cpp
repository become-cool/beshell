#include "Roller.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Roller, Obj)
    std::vector<JSCFunctionListEntry> Roller::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("visibleRowCount",be::lv::Obj::invalidGetter,Roller::setVisibleRowCount) ,
        JS_CGETSET_DEF("selected",Roller::getSelected,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("optionCount",Roller::getOptionCount,be::lv::Obj::invalidSetter) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Roller::Roller(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Roller::Roller(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_roller_create(parent))
    {}
        
    JSValue Roller::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Roller * widget = new Roller(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Roller::setVisibleRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t visibleRowCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &visibleRowCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","visibleRowCount","Roller","setVisibleRowCount","number")
        }
        lv_roller_set_visible_row_count(thisobj->lvobj(), visibleRowCount) ;
        return JS_UNDEFINED ;
    }
    JSValue Roller::getSelected(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t value = lv_roller_get_selected(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    // unspported type: const char *
    // JSValue Roller::getOptions(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_roller_get_options(const lv_obj_t * obj)
    JSValue Roller::getOptionCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t value = lv_roller_get_option_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject)

// AUTO GENERATE CODE END [METHODS] --------

}

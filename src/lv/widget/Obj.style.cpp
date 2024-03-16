#include "Obj.hpp"
#include "../const.hpp"
#include "../Style.hpp"

using namespace std ;

namespace be::lv {


    JSValue Obj::style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        if(!JS_IsString(argv[0])) {
            JSTHROW("arg %s of method %s.%s() must be a %s","styleName","Obj","style","string")
        }
        lv_style_prop_t prop ;
        char * jsStyleName = (char *)JS_ToCString(ctx, argv[0]) ;
        if(!lv_style_prop_str_to_const(jsStyleName, &prop)) {
            JS_ThrowReferenceError(ctx, "unknow style name: %s", jsStyleName) ;
            JS_FreeCString(ctx, jsStyleName) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, jsStyleName) ;
        lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
        if(argc>1) {
            if(JS_ToUint32(ctx, &selector, argv[1])!=0) {
                JSTHROW("invalid arg part")
            }
        }

        JSVALUE_TO_LVOBJ(this_val,lvobj)

        lv_style_value_t value = lv_obj_get_style_prop(lvobj, selector, prop) ;

        return Style::propToJS(ctx, prop, value) ;
    }

    
    static JSValue set_style(JSContext *ctx, lv_obj_t * lvobj, const char * jsStyleName, JSValue value, lv_style_selector_t selector) {
        lv_style_value_t jsStyleValue ;
        
        if(strcmp(jsStyleName,"pad")==0) {
            if(!Style::propToValue(ctx, LV_STYLE_PAD_TOP, value, &jsStyleValue)){
                JSTHROW("style value invalid")
            }
            lv_obj_set_local_style_prop(lvobj, LV_STYLE_PAD_TOP, jsStyleValue, selector) ;
            lv_obj_set_local_style_prop(lvobj, LV_STYLE_PAD_BOTTOM, jsStyleValue, selector) ;
            lv_obj_set_local_style_prop(lvobj, LV_STYLE_PAD_LEFT, jsStyleValue, selector) ;
            lv_obj_set_local_style_prop(lvobj, LV_STYLE_PAD_RIGHT, jsStyleValue, selector) ;
        }
        else {
            lv_style_prop_t prop ;
            if(!lv_style_prop_str_to_const(jsStyleName, &prop)) {
                JS_ThrowReferenceError(ctx, "unknow style name: %s", jsStyleName) ;
                JS_FreeCString(ctx, jsStyleName) ;
                return JS_EXCEPTION ;
            }
            if(!Style::propToValue(ctx, prop, value, &jsStyleValue)){
                JSTHROW("style value invalid")
            }

            lv_style_prop_str_to_const(jsStyleName, &prop) ;
            lv_obj_set_local_style_prop(lvobj, prop, jsStyleValue, selector) ;
        }
        return JS_UNDEFINED ;
    }
    
    JSValue Obj::setStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSVALUE_TO_LVOBJ(this_val,lvobj)
        CHECK_ARGC(1)

        if(JS_IsObject(argv[0])) {
            ARGV_TO_UINT32_OPT(1, selector, LV_PART_MAIN | LV_STATE_DEFAULT)
            
            JSPropertyEnum *tab;
            uint32_t len = 0;
            JS_GetOwnPropertyNames(ctx, &tab, &len, argv[0], JS_GPN_STRING_MASK);
            for (uint32_t i = 0; i < len; i++) {
                JSPropertyEnum e = tab[i];
                JSAtom prop = e.atom;
                const char * propName = JS_AtomToCString(ctx, prop) ;
                JSValue val = JS_GetProperty(ctx, argv[0], prop);
                JSValue ret = set_style(ctx, lvobj, propName, val, selector) ;

                JS_FreeCString(ctx, propName);
                JS_FreeValue(ctx, val);

                if(JS_IsException(ret)) {
                    return ret ;
                }
            }

            return JS_UNDEFINED ;

        } else {
            ARGV_TO_UINT32_OPT(2, selector, LV_PART_MAIN | LV_STATE_DEFAULT)
            
            if(!JS_IsString(argv[0])) {
                JSTHROW("arg %s of method %s.%s() must be a %s","styleName","Obj","setStyle","string")
            }
            const char * jsStyleName = (char *)JS_ToCString(ctx, argv[0]) ;
            JSValue ret = set_style(ctx, lvobj, jsStyleName, argv[1], selector) ;
            JS_FreeCString(ctx, jsStyleName) ;
            return ret ;
        }
    }

    JSValue Obj::allStyleProps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSVALUE_TO_LVOBJ(this_val,lvobj)

        JSTHROW("not implements")
        // JSValue jsProps = JS_NewObject(ctx) ;
        // for(int i = 0; i < lvobj->style_cnt; i++) {

        //     const lv_style_t * style = lvobj->styles[i].style ;

        //     lv_style_value_t * values = (lv_style_value_t *)style->v_p.values_and_props;
        //     uint8_t * tmp = style->v_p.values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
        //     uint16_t * props = (uint16_t *)tmp;

        //     for(int j=0;j<style->prop_cnt;j++) {
        //         const char * propName = lv_style_prop_const_to_str(props[j]) ;
        //         JSValue jsval = lv_style_value_to_js(ctx, props[j], values[j]) ;
        //         JS_SetPropertyStr(ctx, jsProps, propName, jsval) ;
        //         // JS_FreeValue(ctx,jsval) ;
        //     }

        // }

        // return jsProps ;

    }

    JSValue Obj::center(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSVALUE_TO_LVOBJ(this_val,lvobj)
        lv_obj_align(lvobj, LV_ALIGN_CENTER, 0, 0);
        return JS_UNDEFINED ;
    }

    static bool sizeValue(JSContext *ctx, JSValueConst var, int32_t & value) {
        bool success = true ;
        const char * cvalue = JS_ToCString(ctx, var) ;
        char * percent = strchr(cvalue,'%') ;
        if(percent) {
            *percent = 0 ;
            sscanf(cvalue, "%d", &value);
            value = LV_PCT(value) ;
        } else {
            if(JS_ToInt32(ctx, (int32_t *) &value, var)!=0){
                success = false ;
            }
        }

        JS_FreeCString(ctx, cvalue) ;
        return success ;
    }
    JSValue Obj::setWidth(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,lvobj)
        int32_t value ;
        if(!sizeValue(ctx,val,value)) {
            JSTHROW("arg %s of method %s.%s() must be a %s","width","Obj","setWidth","number")
        }
        lv_obj_set_width(lvobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::setHeight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,lvobj)
        int32_t value ;
        if(!sizeValue(ctx,val,value)) {
            JSTHROW("arg %s of method %s.%s() must be a %s","height","Obj","setHeight","number")
        }
        lv_obj_set_height(lvobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
}
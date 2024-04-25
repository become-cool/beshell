#include "LVModule.hpp"
#include "qjs_utils.h"
#include "widget/Label.hpp"
#include <map>

using namespace std ;

namespace be::lv {
    
    static map<string, lv_font_t *> loadedFonts ;
    
    JSValue LVModule::loadFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2) 
        ARGV_TO_CSTRING(0, name) 
        ARGV_TO_CSTRING(1, cpath)

        string path = string("C:") + FS::toVFSPath(ctx, cpath) ;
        JS_FreeCString(ctx, cpath) ;

        lv_font_t * font = lv_binfont_create(path.c_str());
        if(font) {
            loadedFonts[name] = font ;
            JS_FreeCString(ctx, name) ;
            return JS_UNDEFINED ;
        } else {
            JS_FreeCString(ctx, name) ;
            return JSTHROW("load font failed") ;
        }
    }

    #define GET_FONT_MONTSERRAT(s)                  \
        else if(font == &lv_font_montserrat_##s) {  \
            return JS_NewString(ctx, "m"#s) ;       \
        }
    JSValue Label::getFont(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Label,thisobj)
        const lv_font_t * font = lv_obj_get_style_text_font(thisobj->lvobj(),LV_PART_MAIN | LV_STATE_DEFAULT) ;

        if(false) {}
        #if LV_FONT_MONTSERRAT_8
            GET_FONT_MONTSERRAT(8)
        #endif
        #if LV_FONT_MONTSERRAT_10
            GET_FONT_MONTSERRAT(10)
        #endif
        #if LV_FONT_MONTSERRAT_12
            GET_FONT_MONTSERRAT(12)
        #endif
        #if LV_FONT_MONTSERRAT_14
            GET_FONT_MONTSERRAT(14)
        #endif
        #if LV_FONT_MONTSERRAT_16
            GET_FONT_MONTSERRAT(16)
        #endif
        #if LV_FONT_MONTSERRAT_18
            GET_FONT_MONTSERRAT(18)
        #endif
        #if LV_FONT_MONTSERRAT_20
            GET_FONT_MONTSERRAT(20)
        #endif
        #if LV_FONT_MONTSERRAT_22
            GET_FONT_MONTSERRAT(22)
        #endif
        #if LV_FONT_MONTSERRAT_24
            GET_FONT_MONTSERRAT(24)
        #endif
        #if LV_FONT_MONTSERRAT_26
            GET_FONT_MONTSERRAT(26)
        #endif
        #if LV_FONT_MONTSERRAT_28
            GET_FONT_MONTSERRAT(28)
        #endif
        #if LV_FONT_MONTSERRAT_30
            GET_FONT_MONTSERRAT(30)
        #endif
        #if LV_FONT_MONTSERRAT_32
            GET_FONT_MONTSERRAT(32)
        #endif
        #if LV_FONT_MONTSERRAT_34
            GET_FONT_MONTSERRAT(34)
        #endif
        #if LV_FONT_MONTSERRAT_36
            GET_FONT_MONTSERRAT(36)
        #endif
        #if LV_FONT_MONTSERRAT_38
            GET_FONT_MONTSERRAT(38)
        #endif
        #if LV_FONT_MONTSERRAT_40
            GET_FONT_MONTSERRAT(40)
        #endif
        #if LV_FONT_MONTSERRAT_42
            GET_FONT_MONTSERRAT(42)
        #endif
        #if LV_FONT_MONTSERRAT_44
            GET_FONT_MONTSERRAT(44)
        #endif
        #if LV_FONT_MONTSERRAT_46
            GET_FONT_MONTSERRAT(46)
        #endif
        #if LV_FONT_MONTSERRAT_48
            GET_FONT_MONTSERRAT(48)
        #endif

        return JS_NULL ;
    }

    #define SET_FONT_MONTSERRAT(s)                          \
        else if( strcmp("m"#s, fontname)==0 ) {             \
            font = (lv_font_t *)&lv_font_montserrat_##s;    \
        }
    JSValue Label::setFont(JSContext *ctx, JSValueConst this_val, JSValueConst val) {
        THIS_NCLASS(Label,thisobj)

        const char * fontname = JS_ToCString(ctx,val) ;
        lv_font_t * font = NULL ;
        
        if(false) {}
        #if LV_FONT_MONTSERRAT_8
            SET_FONT_MONTSERRAT(8)
        #endif
        #if LV_FONT_MONTSERRAT_10
            SET_FONT_MONTSERRAT(10)
        #endif
        #if LV_FONT_MONTSERRAT_12
            SET_FONT_MONTSERRAT(12)
        #endif
        #if LV_FONT_MONTSERRAT_14
            SET_FONT_MONTSERRAT(14)
        #endif
        #if LV_FONT_MONTSERRAT_16
            SET_FONT_MONTSERRAT(16)
        #endif
        #if LV_FONT_MONTSERRAT_18
            SET_FONT_MONTSERRAT(18)
        #endif
        #if LV_FONT_MONTSERRAT_20
            SET_FONT_MONTSERRAT(20)
        #endif
        #if LV_FONT_MONTSERRAT_22
            SET_FONT_MONTSERRAT(22)
        #endif
        #if LV_FONT_MONTSERRAT_24
            SET_FONT_MONTSERRAT(24)
        #endif
        #if LV_FONT_MONTSERRAT_26
            SET_FONT_MONTSERRAT(26)
        #endif
        #if LV_FONT_MONTSERRAT_28
            SET_FONT_MONTSERRAT(28)
        #endif
        #if LV_FONT_MONTSERRAT_30
            SET_FONT_MONTSERRAT(30)
        #endif
        #if LV_FONT_MONTSERRAT_32
            SET_FONT_MONTSERRAT(32)
        #endif
        #if LV_FONT_MONTSERRAT_34
            SET_FONT_MONTSERRAT(34)
        #endif
        #if LV_FONT_MONTSERRAT_36
            SET_FONT_MONTSERRAT(36)
        #endif
        #if LV_FONT_MONTSERRAT_38
            SET_FONT_MONTSERRAT(38)
        #endif
        #if LV_FONT_MONTSERRAT_40
            SET_FONT_MONTSERRAT(40)
        #endif
        #if LV_FONT_MONTSERRAT_42
            SET_FONT_MONTSERRAT(42)
        #endif
        #if LV_FONT_MONTSERRAT_44
            SET_FONT_MONTSERRAT(44)
        #endif
        #if LV_FONT_MONTSERRAT_46
            SET_FONT_MONTSERRAT(46)
        #endif
        #if LV_FONT_MONTSERRAT_48
            SET_FONT_MONTSERRAT(48)
        #endif

        else if(loadedFonts.count(fontname)>0) {
            font = loadedFonts[fontname] ;
        }
    
        JSValue ret = JS_UNDEFINED ;

        if(!font) {
            JS_ThrowReferenceError(ctx, "unknow font name: %s", fontname) ;
            ret = JS_EXCEPTION ;
        }
        else {
            lv_obj_set_style_text_font(thisobj->lvobj(), font, LV_PART_MAIN | LV_STATE_DEFAULT ) ;
        }

        JS_FreeCString(ctx, fontname) ;
        return ret ;
    }


}

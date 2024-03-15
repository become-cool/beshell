#include "Obj.hpp"
#include "../const.hpp"

using namespace std ;

namespace be::lv {

    void Obj::eventCallback(lv_event_t * e) {
        if(!e->user_data) {
            printf("event->user_data==NULL, no ctx.") ;
            return ;
        }
        JSContext *ctx = (JSContext *)e->user_data ;
        if(!e->current_target) {
            return ;
        }
        Obj * obj = (Obj *)lv_obj_get_user_data((lv_obj_t*)e->current_target) ;
        if(!obj) {
            return ;
        }
        
        JSValue jsname = lv_event_code_const_to_jsstr(ctx, e->code) ;

        // param
        JSValue param = JS_UNDEFINED ;
        if(e->code==LV_EVENT_GESTURE && e->param) {
            // printf("--------- gesture\n") ;
            lv_dir_t dir = lv_indev_get_gesture_dir((const lv_indev_t *)e->param) ;
            param = lv_dir_const_to_jsstr(ctx, dir) ;
        }

        obj->emit(ctx, jsname, {jsname, obj->jsobj, param}) ;

        JS_FreeValue(ctx,jsname) ;

        // @todo 线程安全
        if(e->code==LV_EVENT_DELETE){
            JS_SetOpaque(obj->jsobj, nullptr) ;
            JS_FreeValue(ctx, obj->jsobj) ; // js  counter -1
            obj->jsobj = JS_NULL ;
            obj->_lvobj = nullptr ;
            obj->self = nullptr ;           // c++ counter -1
        }
    }

    lv_event_dsc_t * Obj::findEventDsc(lv_obj_t* obj, lv_event_code_t e) {
        uint32_t count = lv_obj_get_event_count(obj) ;
        for(int i=0; i<count; i++) {
            lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i) ;
            
            if(dsc->cb!=eventCallback)
                continue ;
            if(dsc->filter==LV_EVENT_ALL || dsc->filter==e) {
                return dsc ;
            }
        }
        return NULL ;
    }
    JSValue Obj::enableEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        lv_event_code_t event ;
        if(!lv_event_code_jsstr_to_const(ctx, argv[0], &event)) {
            return JS_UNDEFINED ;
        }
        if(findEventDsc(thisobj->_lvobj, event)==NULL) {
            lv_obj_add_event_cb(thisobj->_lvobj, eventCallback, event, ctx) ;
        }
        return JS_UNDEFINED ;
    }


}
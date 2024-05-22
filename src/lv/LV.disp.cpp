#include "LV.hpp"
#include "deps/lvgl/lvgl.h"
#include "driver/disp/Display.hpp"

using namespace std ;
using namespace be::driver::disp ;

namespace be::lv {
    
    
    typedef struct {
        shared_ptr<be::NativeClass> display ;
    } disp_drv_opa_t ;

    void disp_flush_cb(lv_display_t * lvdisp, const lv_area_t * area, unsigned char * color_p) {
        disp_drv_opa_t* opa = (disp_drv_opa_t*)lv_display_get_driver_data(lvdisp) ;
        if(opa) {
            ((Display*)(opa->display.get()))->drawRect(area->x1,area->y1,area->x2,area->y2, (color_t*)color_p) ;
        }
        lv_display_flush_ready(lvdisp);
    }
    
    /**
     * 将传入的屏幕驱动对象注册为 lv 的屏幕
     * 
     * @module lv
     * @function registerDisplay
     * @param driver 屏幕驱动对象
     * @return bool 是否注册成功
     */
    JSValue LV::registerDisplay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        JSVALUE_TO_NCLASS(be::driver::disp::Display, argv[0], display)

        if( display->_usedForLVGL ) {
            JSTHROW("This display has already been registered to lvgl.")
        }
        
        lv_display_render_mode_t render_mode = LV_DISPLAY_RENDER_MODE_PARTIAL ;
        if(argc>1) {
            GET_UINT32_PROP_OPT(argv[1], "render_mode", render_mode, LV_DISPLAY_RENDER_MODE_PARTIAL )
        }
        if( render_mode!=LV_DISPLAY_RENDER_MODE_PARTIAL
            && render_mode!=LV_DISPLAY_RENDER_MODE_FULL
            && render_mode!=LV_DISPLAY_RENDER_MODE_DIRECT
        ) {
            JSTHROW("Invalid render mode.")
        }

        lv_display_t * lvdisp = lv_display_create(display->width(), display->height()) ;

        if(!display->createBuff()) {
            lv_display_delete(lvdisp) ;
            lvdisp = nullptr ;
            JSTHROW("Failed to create buffer for display.")
        }

        lv_display_set_buffers(lvdisp, display->buff1(), display->buff2(), display->buffSize(), render_mode);

        lv_display_set_antialiasing(lvdisp, true) ;

        lv_display_set_flush_cb(lvdisp, disp_flush_cb);
        
        // @todo
        // unregister 时销毁这个 shared_ptr 指针
        disp_drv_opa_t * opa = new disp_drv_opa_t ;
        opa->display = display->shared() ;

        lv_display_set_driver_data(lvdisp,opa) ;

        display->_usedForLVGL = true ;

        dispReady = true ;

        return JS_UNDEFINED ;
    }

}
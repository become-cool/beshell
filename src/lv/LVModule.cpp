#include "LVModule.hpp"
#include "all-widgets.hpp"
#include <cassert>
#include "Style.hpp"
#include "driver/input/InDevPointer.hpp"

using namespace std ;

namespace be::lv {

    LVModule::LVModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("screen",screen,0) ;
        exportFunction("load",loadScreen,0) ;
        exportFunction("pct",pct,0) ;
        exportFunction("registerDisplay",registerDisplay,0) ;
        exportFunction("registerInputDevice",registerInputDevice,0) ;
        exportFunction("loadFont",loadFont,2) ;

        exportClass<Style>() ;
        exportClass<Row>() ;
        exportClass<Column>() ;
        exportClass<Rect>() ;


// AUTO GENERATE CODE START [EXPORT WIDGETS] --------
        exportClass<Obj>() ;
        exportClass<AnimImg>() ;
        exportClass<Btn>() ;
        exportClass<Canvas>() ;
        exportClass<Dropdown>() ;
        exportClass<Keyboard>() ;
        exportClass<Line>() ;
        exportClass<MsgBox>() ;
        exportClass<Scale>() ;
        exportClass<SpinBox>() ;
        exportClass<Table>() ;
        exportClass<TileView>() ;
        exportClass<Arc>() ;
        exportClass<BtnMatrix>() ;
        exportClass<Chart>() ;
        exportClass<Img>() ;
        exportClass<Label>() ;
        exportClass<List>() ;
        exportClass<Slider>() ;
        exportClass<Spinner>() ;
        exportClass<TabView>() ;
        exportClass<Win>() ;
        exportClass<Bar>() ;
        exportClass<Calendar>() ;
        exportClass<Checkbox>() ;
        exportClass<ImgBtn>() ;
        exportClass<Led>() ;
        exportClass<Menu>() ;
        exportClass<Roller>() ;
        exportClass<Span>() ;
        exportClass<Switch>() ;
        exportClass<TextArea>() ;
        exportClass<Gif>() ;
// AUTO GENERATE CODE END [EXPORT WIDGETS] --------
    }

    // void LVModule::import() {
    // }

    
    void LVModule::use(be::BeShell & beshell) {
        driver::display::use(beshell) ;
        beshell.addModule<LVModule>("lv") ;
    }

    JSValue LVModule::screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvobj = lv_screen_active() ;
        if(!lvobj) {
            return JS_NULL ;
        }
        return JS_DupValue(ctx,Obj::wrap(ctx,lvobj)->jsobj) ;
    }
    JSValue LVModule::loadScreen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        JSVALUE_TO_NCLASS(Obj, argv[0], obj)
        lv_screen_load(obj->lvobj()) ;
        return JS_UNDEFINED ;
    }
    
    JSValue LVModule::pct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0,val)
        return JS_NewUint32(ctx,LV_PCT(val)) ;
    }

    JSValue LVModule::registerDisplay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        dd
        return JS_UNDEFINED ;
    }

    typedef struct {
        shared_ptr<be::driver::InDevPointer> ptr ;
        uint16_t x;
        uint16_t y;
    } indev_pointer_data_t ;

    static void pointer_read(lv_indev_t * indev, lv_indev_data_t * data) {

        indev_pointer_data_t * indev_opa = (indev_pointer_data_t*)lv_indev_get_user_data(indev) ;
        assert(indev_opa) ;

        if( indev_opa->ptr->readPointCount()>0 ) {
            indev_opa->ptr->readPos(0, indev_opa->x, indev_opa->y);
            // dn2(indev_opa->x, indev_opa->y)
            data->state = LV_INDEV_STATE_PR;
        } else {
            data->state = LV_INDEV_STATE_REL;
        }

        data->point.x = (lv_coord_t)indev_opa->x;
        data->point.y = (lv_coord_t)indev_opa->y;
    }

    JSValue LVModule::registerInputDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        dd
        JSVALUE_TO_NCLASS(be::driver::InDevPointer, argv[0], indev)

        // @todo
        // unregister 的时候回收 indev_pointer_data_t
        auto data = new indev_pointer_data_t() ;
        data->x = 0 ;
        data->y = 0 ;

        data->ptr = static_pointer_cast<be::driver::InDevPointer>(indev->shared());

        lv_indev_t * lvindev = lv_indev_create();
        dp(lvindev)
        lv_indev_set_user_data(lvindev, (void *)data) ;
        lv_indev_set_type(lvindev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(lvindev, pointer_read);

        return JS_UNDEFINED ;
    }
    

}
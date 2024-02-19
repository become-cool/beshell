#include "LVModule.hpp"
#include "all-widgets.hpp"
#include "Style.hpp"

namespace be::lv {

    LVModule::LVModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("screen",screen,0) ;
        exportFunction("load",loadScreen,0) ;
        exportFunction("pct",pct,0) ;
        exportFunction("test",test,0) ;

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

    JSValue LVModule::test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * parent = lv_screen_active() ;
        dp(parent)
        dn(lv_obj_get_child_count(parent))

        lv_obj_t * btn = lv_button_create(parent) ;

        return JS_UNDEFINED ;
    }

}
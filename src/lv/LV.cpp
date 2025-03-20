#include "LV.hpp"
#include "Animation.hpp"
#include "all-widgets.hpp"
#include <cassert>
#include "Style.hpp"

using namespace std ;
using namespace be::driver::disp ;

namespace be::lv {

    char const * const LV::name = "lv" ;
    esp_timer_handle_t LV::tickTimer = nullptr ;
    bool LV::used = false ;
    bool LV::dispReady = false ;
    bool LV::useFont = true ;
    bool LV::useImg = true ;
    
    std::map<std::string, const lv_image_dsc_t  *> LV::embededImages ;

    LV::LV(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        EXPORT_FUNCTION(screen) ;
        EXPORT_FUNCTION(load) ;
        EXPORT_FUNCTION(pct) ;
        EXPORT_FUNCTION(registerDisplay) ;
        EXPORT_FUNCTION(registerInputDevice) ;
        EXPORT_FUNCTION(loadFont) ;
        EXPORT_FUNCTION(unuseFont) ;
        EXPORT_FUNCTION(unuseImg) ;
        EXPORT_FUNCTION(disableAllInDev) ;
        EXPORT_FUNCTION(enableAllInDev) ;
        
        exportFunction("RGB", Display::RGB) ;
        exportFunction("RGB565", Display::RGB565) ;
        exportFunction("toRGB", Display::toRGB) ;
        exportFunction("toRGB565", Display::toRGB565) ;

        exportClass<Animation>() ;
        exportClass<Style>() ;
        exportClass<Row>() ;
        exportClass<Column>() ;
        exportClass<Rect>() ;
        exportClass<ColorPicker>() ;


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

    // void LV::exports() {
    // }

    static void lv_tick_inc_cb(void *data) {
        lv_tick_inc(*((uint32_t *) data));
    }

    void LV::initTick() {
        static const uint32_t tick_inc_period_ms = 20;
        const esp_timer_create_args_t periodic_timer_args = {
                .callback = lv_tick_inc_cb,
                .arg = (void*)&tick_inc_period_ms,
                .dispatch_method = ESP_TIMER_TASK,            
                .skip_unhandled_events = true,
        };
        ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &tickTimer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(tickTimer, tick_inc_period_ms * 1000));
    }
    
    void LV::loop(const BeShell &, void *) {
        if(dispReady) {
            lv_task_handler() ;
        }
    }

    void LV::use(be::BeShell * beshell) {
        if(used) {
            return ;
        }

        initTick() ;
        beshell->addLoopFunction(loop,nullptr) ;
        lv_init() ;

        used = true ;
    }


    /**
     * 返回当前激活的 `屏幕对象`
     * 
     * `屏幕对象` 是指当前显示在屏幕上的对象，是所有可见组件的根节点，自身没有父节对象。
     * 
     * @module lv
     * @function screen
     * @return Obj[widget/Obj] 当前激活的屏幕对象
     */
    JSValue LV::screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvobj = lv_screen_active() ;
        if(!lvobj) {
            return JS_NULL ;
        }
        return JS_DupValue(ctx,Obj::wrap(ctx,lvobj)->jsobj) ;
    }

    
    /**
     * 将参数指定的对象作为 `屏幕对象` 加载到当前显示器上
     * 
     * 加载成功后，传入对象的父对象会被设置为 null
     * 
     * @module lv
     * @function load
     * @param obj:Obj[widget/Obj] 要加载的屏幕对象
     * @return undefined
     */
    JSValue LV::load(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        JSVALUE_TO_NCLASS(Obj, argv[0], obj)
        lv_screen_load(obj->lvobj()) ;
        return JS_UNDEFINED ;
    }
    
    /**
     * 将 0-100 范围的整数转换为能够表示百分比的 16位整数值
     * 
     * 改数值是 lvgl 内部使用的格式
     * 
     * @module lv
     * @function pct
     * @param value:number 0-100范围的整数
     * @return number
     */
    JSValue LV::pct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT16(0,val)
        return JS_NewUint32(ctx,LV_PCT(val)) ;
    }

    JSValue LV::unuseFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        LV::useFont = false ;
        return JS_UNDEFINED ;
    }

    JSValue LV::unuseImg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        string ARGV_TO_STRING_OPT(0, name, "*")

        if(name=="*") {
            LV::useImg = false ;
        } else {
            LV::embededImages.erase(name) ;
        }

        return JS_UNDEFINED ;
    }

    void LV::addImageDsc(const char * name, const lv_image_dsc_t  * imgDsc) {
        LV::embededImages[name] = imgDsc ;
    }
    
}
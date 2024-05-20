#include "LV.hpp"
#include "Animation.hpp"
#include "all-widgets.hpp"
#include <cassert>
#include "Style.hpp"
#include "driver/input/InDevPointer.hpp"
#include "driver/disp/Display.hpp"

using namespace std ;
using namespace be::driver::disp ;

namespace be::lv {

    const char * const LV::name = "lv" ;
    esp_timer_handle_t LV::tickTimer = nullptr ;
    bool LV::used = false ;
    
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

    // void LV::import() {
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
        lv_task_handler() ;
    }

    void LV::use(be::BeShell * beshell) {
        if(used) {
            return ;
        }

        initTick() ;
        beshell->addLoopFunction(loop) ;
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
        return JS_NewBool(ctx, display->registerToLV()) ;
    }

    typedef struct {
        shared_ptr<be::driver::InDevPointer> ptr ;
        uint16_t x;
        uint16_t y;
        bool enabled;
    } indev_pointer_data_t ;

    static void pointer_read(lv_indev_t * indev, lv_indev_data_t * data) {

        indev_pointer_data_t * indev_opa = (indev_pointer_data_t*)lv_indev_get_user_data(indev) ;
        assert(indev_opa) ;

        indev_opa->ptr->read() ;
        if(!indev_opa->enabled) {
            return ;
        }

        indev_opa->x = indev_opa->ptr->lastX() ;
        indev_opa->y = indev_opa->ptr->lastY() ;

        // if( indev_opa->ptr->readPointCount()>0 ) {
        //     indev_opa->ptr->readPos(0, indev_opa->x, indev_opa->y);
        //     dn2(indev_opa->x, indev_opa->y)
        //     data->state = LV_INDEV_STATE_PR;
        // } else {
        //     data->state = LV_INDEV_STATE_REL;
        // }

        data->point.x = (lv_coord_t)indev_opa->x;
        data->point.y = (lv_coord_t)indev_opa->y;
        data->state = indev_opa->ptr->pointCount()==0? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR ;
    }


    typedef struct  {
        /**< Input device type*/
        lv_indev_type_t type;

        /**< Function pointer to read input device data.*/
        lv_indev_read_cb_t read_cb;

        lv_indev_state_t state; /**< Current state of the input device.*/
        lv_indev_mode_t mode;

        /*Flags*/
        uint8_t long_pr_sent : 1;
        uint8_t reset_query : 1;
        uint8_t disabled : 1;
        uint8_t wait_until_release : 1;

        uint32_t pr_timestamp;         /**< Pressed time stamp*/
        uint32_t longpr_rep_timestamp; /**< Long press repeat time stamp*/

        void * driver_data;
        void * user_data;

        /**< Pointer to the assigned display*/
        lv_display_t * disp;

        /**< Timer to periodically read the input device*/
        lv_timer_t * read_timer;

        /**< Number of pixels to slide before actually drag the object*/
        uint8_t scroll_limit;

        /**< Drag throw slow-down in [%]. Greater value means faster slow-down*/
        uint8_t scroll_throw;

        /**< At least this difference should be between two points to evaluate as gesture*/
        uint8_t gesture_min_velocity;

        /**< At least this difference should be to send a gesture*/
        uint8_t gesture_limit;

        /**< Long press time in milliseconds*/
        uint16_t long_press_time;

        /**< Repeated trigger period in long press [ms]*/
        uint16_t long_press_repeat_time;

        struct {
            /*Pointer and button data*/
            lv_point_t act_point; /**< Current point of input device.*/
            lv_point_t last_point; /**< Last point of input device.*/
            lv_point_t last_raw_point; /**< Last point read from read_cb. */
            lv_point_t vect; /**< Difference between `act_point` and `last_point`.*/
            lv_point_t scroll_sum; /*Count the dragged pixels to check LV_INDEV_DEF_SCROLL_LIMIT*/
            lv_point_t scroll_throw_vect;
            lv_point_t scroll_throw_vect_ori;
            lv_obj_t * act_obj;      /*The object being pressed*/
            lv_obj_t * last_obj;     /*The last object which was pressed*/
            lv_obj_t * scroll_obj;   /*The object being scrolled*/
            lv_obj_t * last_pressed; /*The lastly pressed object*/
            lv_area_t scroll_area;
            lv_point_t gesture_sum; /*Count the gesture pixels to check LV_INDEV_DEF_GESTURE_LIMIT*/

            /*Flags*/
            lv_dir_t scroll_dir : 4;
            lv_dir_t gesture_dir : 4;
            uint8_t gesture_sent : 1;
        } pointer;
        struct {
            /*Keypad data*/
            lv_indev_state_t last_state;
            uint32_t last_key;
        } keypad;

        lv_obj_t * cursor;     /**< Cursor for LV_INPUT_TYPE_POINTER*/
        lv_group_t * group;   /**< Keypad destination group*/
        const lv_point_t * btn_points; /**< Array points assigned to the button ()screen will be pressed
                                        here by the buttons*/

        lv_event_list_t event_list;
        lv_anim_t * scroll_throw_anim;
    } _lv_indev_t ;

    /**
     * 将一个输入设备的驱动对象注册为 lv 的输入设备
     * 
     * @module lv
     * @function registerInputDevice
     * @param driver 输入设备驱动对象
     * @return bool 是否注册成功
     */
    JSValue LV::registerInputDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        JSVALUE_TO_NCLASS(be::driver::InDevPointer, argv[0], indev)

        int gesture_limit = 20 ;
        int gesture_min_velocity = 3 ;
        if(argc>1) {
            GET_UINT32_PROP_OPT(argv[1], "gesture_limit", gesture_limit, 20 )
            GET_UINT32_PROP_OPT(argv[1], "gesture_min_velocity", gesture_min_velocity, 3 )
            dn2(gesture_min_velocity, gesture_limit)
        }

        // @todo
        // unregister 的时候回收 indev_pointer_data_t
        auto data = new indev_pointer_data_t() ;
        data->x = 0 ;
        data->y = 0 ;
        data->enabled = true ;

        data->ptr = static_pointer_cast<be::driver::InDevPointer>(indev->shared());

        lv_indev_t * lvindev = lv_indev_create();
        lv_indev_set_user_data(lvindev, (void *)data) ;
        lv_indev_set_type(lvindev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(lvindev, pointer_read);
        dn(((_lv_indev_t*)lvindev)->gesture_min_velocity)
        ((_lv_indev_t*)lvindev)->gesture_limit = gesture_limit ;
        ((_lv_indev_t*)lvindev)->gesture_min_velocity = gesture_min_velocity ;

        return JS_UNDEFINED ;
    }
    
    /**
     * 禁用所有的输入设备，可用于锁定系统
     * 
     * @module lv
     * @function disableAllInDev
     * @return undefined
     */
    JSValue LV::disableAllInDev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_indev_t * indev = NULL;
        for(;;) {
            indev = lv_indev_get_next(indev);
            if(!indev) {
                break;
            }
            indev_pointer_data_t * indev_opa = (indev_pointer_data_t*)lv_indev_get_user_data(indev) ;
            if(indev_opa) {
                indev_opa->enabled = false ;
            }
        }
        return JS_UNDEFINED ;
    }
    
    /**
     * 恢复输入设备
     * 
     * @module lv
     * @function disableAllInDev
     * @return undefined
     */
    JSValue LV::enableAllInDev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_indev_t * indev = NULL;
        for(;;) {
            indev = lv_indev_get_next(indev);
            if(!indev) {
                break;
            }
            indev_pointer_data_t * indev_opa = (indev_pointer_data_t*)lv_indev_get_user_data(indev) ;
            if(indev_opa) {
                indev_opa->enabled = true ;
            }
        }
        return JS_UNDEFINED ;
    }

    void addImageDsc(const char * name, const lv_image_dsc_t  * imgDsc) {
        LV::embededImages[name] = imgDsc ;
    }
}
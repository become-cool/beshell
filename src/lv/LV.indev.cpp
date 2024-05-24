#include "LV.hpp"
#include "deps/lvgl/lvgl.h"
#include "driver/input/InDevPointer.hpp"

using namespace std ;

namespace be::lv {
    
    typedef struct {
        shared_ptr<be::driver::input::InDevPointer> ptr ;
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
        JSVALUE_TO_NCLASS(be::driver::input::InDevPointer, argv[0], indev)

        if(indev->_usedForLVGL) {
            JSTHROW("Input device has already been registered as lv input device")
        }

        int gesture_limit = 20 ;
        int gesture_min_velocity = 3 ;
        if(argc>1) {
            GET_UINT32_PROP_OPT(argv[1], "gesture_limit", gesture_limit, 20 )
            GET_UINT32_PROP_OPT(argv[1], "gesture_min_velocity", gesture_min_velocity, 3 )
        }

        // @todo
        // unregister 的时候回收 indev_pointer_data_t
        auto data = new indev_pointer_data_t() ;
        data->x = 0 ;
        data->y = 0 ;
        data->enabled = true ;

        data->ptr = static_pointer_cast<be::driver::input::InDevPointer>(indev->shared());

        lv_indev_t * lvindev = lv_indev_create();
        lv_indev_set_user_data(lvindev, (void *)data) ;
        lv_indev_set_type(lvindev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(lvindev, pointer_read);
        ((_lv_indev_t*)lvindev)->gesture_limit = gesture_limit ;
        ((_lv_indev_t*)lvindev)->gesture_min_velocity = gesture_min_velocity ;

        indev->_usedForLVGL = true ;

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

}
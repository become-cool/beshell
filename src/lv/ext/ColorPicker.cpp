#include "ColorPicker.hpp"
#include "../../driver/disp/Display.hpp"

using namespace std ;


// const BLOCKS = [
//   [
//     [253, 204, 208],
//     [245, 186, 206],
//     [220, 188, 229],
//     [204, 192, 228],
//     [198, 202, 231],
//     [187, 223, 247],
//     [175, 228, 244],
//     [175, 234, 238],
//     [182, 222, 221],
//     [194, 228, 201],
//     [219, 236, 204],
//     [241, 242, 198],
//     [254, 249, 194],
//     [253, 234, 176],
//     [249, 225, 177],
//     [250, 202, 190],
//   ],
//   [
//     [236, 156, 157],
//     [247, 142, 175],
//     [206, 146, 218],
//     [178, 158, 217],
//     [156, 166, 215],
//     [142, 203, 248],
//     [128, 212, 248],
//     [124, 222, 233],
//     [126, 203, 195],
//     [168, 211, 168],
//     [198, 224, 163],
//     [232, 237, 155],
//     [252, 246, 152],
//     [254, 224, 128],
//     [252, 204, 122],
//     [251, 171, 146],
//   ],
//   [
//     [227, 113, 112],
//     [240, 96, 145],
//     [188, 105, 197],
//     [149, 116, 203],
//     [121, 130, 195],
//     [99, 178, 245],
//     [71, 192, 239],
//     [71, 210, 231],
//     [74, 181, 165],
//     [124, 200, 125],
//     [171, 213, 131],
//     [219, 230, 110],
//     [252, 241, 117],
//     [255, 212, 84],
//     [253, 179, 80],
//     [251, 139, 99],
//   ],
//   [
//     [238, 78, 78],
//     [230, 65, 125],
//     [165, 68, 181],
//     [123, 87, 195],
//     [87, 102, 185],
//     [67, 163, 240],
//     [42, 182, 241],
//     [35, 196, 216],
//     [34, 168, 143],
//     [101, 186, 101],
//     [154, 202, 102],
//     [210, 220, 85],
//     [252, 237, 82],
//     [251, 201, 40],
//     [255, 167, 34],
//     [250, 111, 72],
//   ],
//   [
//     [240, 63, 53],
//     [229, 31, 90],
//     [150, 41, 170],
//     [98, 58, 181],
//     [66, 77, 182],
//     [31, 148, 243],
//     [4, 167, 244],
//     [0, 186, 212],
//     [0, 146, 135],
//     [77, 171, 74],
//     [137, 193, 66],
//     [201, 219, 55],
//     [250, 234, 60],
//     [252, 192, 6],
//     [253, 149, 2],
//     [251, 86, 32],
//   ],
//   [
//     [228, 55, 51],
//     [212, 26, 97],
//     [136, 37, 166],
//     [92, 52, 175],
//     [57, 70, 172],
//     [25, 136, 225],
//     [0, 155, 229],
//     [0, 172, 188],
//     [2, 132, 120],
//     [64, 159, 65],
//     [124, 178, 64],
//     [192, 201, 48],
//     [252, 212, 52],
//     [250, 179, 0],
//     [248, 135, 5],
//     [243, 78, 32],
//   ],
//   [
//     [210, 46, 47],
//     [189, 26, 83],
//     [118, 29, 159],
//     [79, 39, 162],
//     [47, 59, 157],
//     [27, 116, 208],
//     [3, 135, 207],
//     [0, 152, 165],
//     [0, 121, 100],
//     [54, 141, 63],
//     [108, 154, 56],
//     [173, 177, 38],
//     [246, 191, 46],
//     [253, 160, 0],
//     [246, 122, 0],
//     [230, 76, 22],
//   ],
//   [
//     [195, 37, 38],
//     [167, 22, 81],
//     [106, 24, 152],
//     [65, 40, 158],
//     [39, 52, 146],
//     [3, 109, 187],
//     [0, 120, 183],
//     [0, 128, 135],
//     [12, 102, 77],
//     [44, 124, 55],
//     [91, 135, 46],
//     [155, 158, 29],
//     [247, 167, 34],
//     [255, 138, 2],
//     [236, 108, 0],
//     [208, 66, 28],
//   ],
//   [
//     [180, 26, 27],
//     [137, 12, 85],
//     [72, 17, 136],
//     [46, 25, 141],
//     [23, 30, 118],
//     [9, 70, 163],
//     [0, 85, 151],
//     [3, 93, 101],
//     [1, 75, 60],
//     [26, 95, 30],
//     [56, 102, 30],
//     [127, 120, 16],
//     [238, 125, 21],
//     [246, 107, 0],
//     [228, 81, 0],
//     [190, 54, 14],
//   ],
// ];

namespace be::lv {

    DEFINE_NCLASS_META(ColorPicker, Obj)

    std::vector<JSCFunctionListEntry> ColorPicker::methods = {
    } ;

    ColorPicker::ColorPicker(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, ColorPicker::build(ctx,jsobj), lvobj)
    {
        if(lvobj) {
            lv_obj_remove_style_all(lvobj);
            lv_obj_set_size(lvobj, 300, 300);
            // lv_obj_set_style_bg_color(obj, lv_color_hex3(0xff0), 0);
            // lv_obj_set_style_opa(obj, opa_saved, 0);
            lv_obj_add_event_cb(lvobj, draw, LV_EVENT_DRAW_MAIN, (void *)this);
        }
    }

    
    void ColorPicker::draw(lv_event_t * e) {
        
        lv_obj_t * obj = (lv_obj_t *) lv_event_get_target(e);
        int32_t w = lv_obj_get_style_width(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_width(obj, LV_PART_MAIN);

        dn2(w, h)
        
        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);

        // lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

        
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_area_t coords;
        coords.x1 = 10;
        coords.y1 = 10;
        coords.x2 = 20;
        coords.y2 = 20;
        //draw_dsc()


        draw_dsc.bg_color = (lv_color_t) be::driver::disp::RGB(0xff, 0xff, 0xff);
        // printf("%d\n",(int)draw_dsc.bg_color)

        draw_dsc.bg_opa = 255 ;

        lv_draw_rect(layer, &draw_dsc, &coords);


        // lv_area_increase(&coords, w, h);


        // lv_layer_t * layer = lv_event_get_layer(e);
        // lv_draw_rect_dsc_t draw_dsc;
        // lv_draw_rect_dsc_init(&draw_dsc);

        // lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        // /*If the border is drawn later disable loading its properties*/
        // if(lv_obj_get_style_border_post(obj, LV_PART_MAIN)) {
        //     draw_dsc.border_post = 1;
        // }

        // int32_t w = lv_obj_get_style_transform_width(obj, LV_PART_MAIN);
        // int32_t h = lv_obj_get_style_transform_height(obj, LV_PART_MAIN);
        // lv_area_t coords;
        // lv_area_copy(&coords, &obj->coords);
        // lv_area_increase(&coords, w, h);

        // lv_draw_rect(layer, &draw_dsc, &coords);




        
        // lv_draw_triangle_dsc_t dsc;
        // lv_draw_triangle_dsc_init(&dsc);

        // lv_obj_t * obj = lv_event_get_target(e);

        // lv_point_t * p_rel = lv_event_get_user_data(e);

        // dsc.p[0].x = p_rel[0].x + obj->coords.x1 + 8;
        // dsc.p[0].y = p_rel[0].y + obj->coords.y1 + 2;
        // dsc.p[1].x = p_rel[1].x + obj->coords.x1 + 8;
        // dsc.p[1].y = p_rel[1].y + obj->coords.y1 + 2;
        // dsc.p[2].x = p_rel[2].x + obj->coords.x1 + 8;
        // dsc.p[2].y = p_rel[2].y + obj->coords.y1 + 2;

        // lv_opa_t opa = lv_obj_get_style_opa(obj, 0);
        // dsc.bg_grad.dir = lv_obj_get_style_bg_grad_dir(obj, 0);
        // dsc.bg_grad.stops[0].color = lv_obj_get_style_bg_color(obj, 0);
        // dsc.bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop(obj, 0);
        // dsc.bg_grad.stops[0].opa = LV_OPA_MIX2(lv_obj_get_style_bg_main_opa(obj, 0), opa);
        // dsc.bg_grad.stops[1].color = lv_obj_get_style_bg_grad_color(obj, 0);
        // dsc.bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop(obj, 0);
        // dsc.bg_grad.stops[1].opa = LV_OPA_MIX2(lv_obj_get_style_bg_grad_opa(obj, 0), opa);
        // dsc.bg_grad.stops_count = 2;

        // dsc.bg_color = dsc.bg_grad.stops[0].color;
        // dsc.bg_opa = dsc.bg_grad.stops[0].opa;

        // lv_draw_triangle(lv_event_get_layer(e), &dsc);
    }

    ColorPicker::ColorPicker(JSContext * ctx, lv_obj_t * parent)
        : ColorPicker(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue ColorPicker::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        ColorPicker * widget = new ColorPicker(ctx, obj, lv_obj_create(lvparent)) ;
        return widget->jsobj ;
    }

}
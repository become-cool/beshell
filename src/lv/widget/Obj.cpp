#include "Obj.hpp"
#include "../const.hpp"
#include "../Style.hpp"
#include "../all-widgets.hpp"
#include <stdlib.h>

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Obj, EventEmitter)
    
    JSValue dbginfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        return JS_UNDEFINED ;
    }
    
    std::vector<JSCFunctionListEntry> Obj::methods = {
        JS_CFUNC_DEF("dbginfo", 0, dbginfo),
        JS_CFUNC_DEF("child", 1, Obj::getChild),
        JS_CFUNC_DEF("style", 1, Obj::style),
        JS_CFUNC_DEF("setStyle", 2, Obj::setStyle),
        JS_CFUNC_DEF("center", 2, Obj::center),
        JS_CFUNC_DEF("enableEvent", 1, Obj::enableEvent),

// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("state",Obj::getState,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("eventCount",Obj::getEventCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("x",Obj::getX,Obj::setX) ,
        JS_CGETSET_DEF("y",Obj::getY,Obj::setY) ,
        JS_CGETSET_DEF("width",Obj::getWidth,Obj::setWidth) ,
        JS_CGETSET_DEF("height",Obj::getHeight,Obj::setHeight) ,
        JS_CGETSET_DEF("contentWidth",Obj::getContentWidth,Obj::setContentWidth) ,
        JS_CGETSET_DEF("contentHeight",Obj::getContentHeight,Obj::setContentHeight) ,
        JS_CGETSET_DEF("layout",be::lv::Obj::invalidGetter,Obj::setLayout) ,
        JS_CGETSET_DEF("align",be::lv::Obj::invalidGetter,Obj::setAlign) ,
        JS_CGETSET_DEF("xAligned",Obj::getXAligned,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("yAligned",Obj::getYAligned,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("selfWidth",Obj::getSelfWidth,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("selfHeight",Obj::getSelfHeight,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("extClickArea",be::lv::Obj::invalidGetter,Obj::setExtClickArea) ,
        JS_CGETSET_DEF("scrollbarMode",Obj::getScrollbarMode,Obj::setScrollbarMode) ,
        JS_CGETSET_DEF("scrollDir",Obj::getScrollDir,Obj::setScrollDir) ,
        JS_CGETSET_DEF("scrollSnapX",Obj::getScrollSnapX,Obj::setScrollSnapX) ,
        JS_CGETSET_DEF("scrollSnapY",Obj::getScrollSnapY,Obj::setScrollSnapY) ,
        JS_CGETSET_DEF("scrollX",Obj::getScrollX,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollY",Obj::getScrollY,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollTop",Obj::getScrollTop,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollBottom",Obj::getScrollBottom,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollLeft",Obj::getScrollLeft,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollRight",Obj::getScrollRight,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("parent",Obj::getParent,Obj::setParent) ,
        JS_CGETSET_DEF("screen",Obj::getScreen,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("childCount",Obj::getChildCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("index",Obj::getIndex,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("flexFlow",be::lv::Obj::invalidGetter,Obj::setFlexFlow) ,
        JS_CGETSET_DEF("flexGrow",be::lv::Obj::invalidGetter,Obj::setFlexGrow) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addFlag", 1, Obj::addFlag),
        JS_CFUNC_DEF("removeFlag", 1, Obj::removeFlag),
        JS_CFUNC_DEF("updateFlag", 2, Obj::updateFlag),
        JS_CFUNC_DEF("addState", 1, Obj::addState),
        JS_CFUNC_DEF("removeState", 1, Obj::removeState),
        JS_CFUNC_DEF("setState", 2, Obj::setState),
        JS_CFUNC_DEF("hasFlag", 1, Obj::hasFlag),
        JS_CFUNC_DEF("hasFlagAny", 1, Obj::hasFlagAny),
        JS_CFUNC_DEF("hasState", 1, Obj::hasState),
        JS_CFUNC_DEF("allocateSpecAttr", 0, Obj::allocateSpecAttr),
        JS_CFUNC_DEF("isValid", 0, Obj::isValid),
        JS_CFUNC_DEF("classInitObj", 0, Obj::classInitObj),
        JS_CFUNC_DEF("isEditable", 0, Obj::isEditable),
        JS_CFUNC_DEF("isGroupDef", 0, Obj::isGroupDef),
        JS_CFUNC_DEF("calculateExtDrawSize", 1, Obj::calculateExtDrawSize),
        JS_CFUNC_DEF("refreshExtDrawSize", 0, Obj::refreshExtDrawSize),
        JS_CFUNC_DEF("removeEvent", 1, Obj::removeEvent),
        JS_CFUNC_DEF("setPos", 2, Obj::setPos),
        JS_CFUNC_DEF("setSize", 2, Obj::setSize),
        JS_CFUNC_DEF("refrSize", 0, Obj::refrSize),
        JS_CFUNC_DEF("isLayoutPositioned", 0, Obj::isLayoutPositioned),
        JS_CFUNC_DEF("markLayoutAsDirty", 0, Obj::markLayoutAsDirty),
        JS_CFUNC_DEF("updateLayout", 0, Obj::updateLayout),
        JS_CFUNC_DEF("alignTo", 4, Obj::alignTo),
        JS_CFUNC_DEF("refreshSelfSize", 0, Obj::refreshSelfSize),
        JS_CFUNC_DEF("refrPos", 0, Obj::refrPos),
        JS_CFUNC_DEF("moveTo", 2, Obj::moveTo),
        JS_CFUNC_DEF("moveChildrenBy", 3, Obj::moveChildrenBy),
        JS_CFUNC_DEF("invalidate", 0, Obj::invalidate),
        JS_CFUNC_DEF("isVisible", 0, Obj::isVisible),
        JS_CFUNC_DEF("scrollBy", 3, Obj::scrollBy),
        JS_CFUNC_DEF("scrollByBounded", 3, Obj::scrollByBounded),
        JS_CFUNC_DEF("scrollTo", 3, Obj::scrollTo),
        JS_CFUNC_DEF("scrollToX", 2, Obj::scrollToX),
        JS_CFUNC_DEF("scrollToY", 2, Obj::scrollToY),
        JS_CFUNC_DEF("scrollToView", 1, Obj::scrollToView),
        JS_CFUNC_DEF("scrollToViewRecursive", 1, Obj::scrollToViewRecursive),
        JS_CFUNC_DEF("isScrolling", 0, Obj::isScrolling),
        JS_CFUNC_DEF("updateSnap", 1, Obj::updateSnap),
        JS_CFUNC_DEF("scrollbarInvalidate", 0, Obj::scrollbarInvalidate),
        JS_CFUNC_DEF("readjustScroll", 1, Obj::readjustScroll),
        JS_CFUNC_DEF("removeStyleAll", 0, Obj::removeStyleAll),
        JS_CFUNC_DEF("fadeIn", 2, Obj::fadeIn),
        JS_CFUNC_DEF("fadeOut", 2, Obj::fadeOut),
        JS_CFUNC_DEF("delete", 0, Obj::_delete),
        JS_CFUNC_DEF("clean", 0, Obj::clean),
        JS_CFUNC_DEF("deleteDelayed", 1, Obj::deleteDelayed),
        JS_CFUNC_DEF("deleteAsync", 0, Obj::deleteAsync),
        JS_CFUNC_DEF("swap", 1, Obj::swap),
        JS_CFUNC_DEF("moveToIndex", 1, Obj::moveToIndex),
        JS_CFUNC_DEF("getChild", 1, Obj::getChild),
        JS_CFUNC_DEF("getSibling", 1, Obj::getSibling),
        JS_CFUNC_DEF("dumpTree", 0, Obj::dumpTree),
        JS_CFUNC_DEF("setFlexAlign", 3, Obj::setFlexAlign),
        JS_CFUNC_DEF("setGridAlign", 2, Obj::setGridAlign),
        JS_CFUNC_DEF("setGridCell", 6, Obj::setGridCell),
        // Unsupported arg type:
        // bool lv_obj_check_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // bool lv_obj_has_class(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj_t * parent)
        // void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint32_t part, lv_draw_rect_dsc_t * draw_dsc)
        // void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint32_t part, lv_draw_label_dsc_t * draw_dsc)
        // void lv_obj_init_draw_image_dsc(lv_obj_t * obj, uint32_t part, lv_draw_image_dsc_t * draw_dsc)
        // void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint32_t part, lv_draw_line_dsc_t * draw_dsc)
        // void lv_obj_init_draw_arc_dsc(lv_obj_t * obj, uint32_t part, lv_draw_arc_dsc_t * draw_dsc)
        // lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)
        // lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)
        // void lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
        // lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
        // bool lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
        // uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)
        // void lv_obj_get_coords(const lv_obj_t * obj, lv_area_t * coords)
        // void lv_obj_get_content_coords(const lv_obj_t * obj, lv_area_t * area)
        // void lv_obj_transform_point(const lv_obj_t * obj, lv_point_t * p, bool recursive, bool inv)
        // void lv_obj_get_transformed_area(const lv_obj_t * obj, lv_area_t * area, bool recursive, bool inv)
        // void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area)
        // bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area)
        // void lv_obj_get_click_area(const lv_obj_t * obj, lv_area_t * area)
        // bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point)
        // lv_result_t lv_obj_set_property(lv_obj_t * obj, const lv_property_t * value)
        // lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)
        // lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
        // void lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
        // void lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor, lv_area_t * ver)
        // void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
        // bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style, lv_style_selector_t selector)
        // void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
        // void lv_obj_report_style_change(lv_style_t * style)
        // void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
        // void lv_obj_enable_style_refresh(bool en)
        // lv_style_value_t lv_obj_get_style_prop(const lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
        // bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
        // void lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t value, lv_style_selector_t selector)
        // lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t * value, lv_style_selector_t selector)
        // bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
        // lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt)
        // lv_opa_t lv_obj_get_style_opa_recursive(const lv_obj_t * obj, lv_part_t part)
        // void lv_obj_set_style_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_min_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_max_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_min_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_max_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_length(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_align(lv_obj_t * obj, lv_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_translate_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_translate_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_scale_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_scale_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_rotation(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_pivot_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_pivot_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_skew_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transform_skew_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_row(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_pad_column(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_margin_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_margin_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_margin_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_margin_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_grad_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_grad_dir(lv_obj_t * obj, lv_grad_dir_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_main_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_grad_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_main_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_grad_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_grad(lv_obj_t * obj, const lv_grad_dsc_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_bg_image_tiled(lv_obj_t * obj, bool value, lv_style_selector_t selector)
        // void lv_obj_set_style_border_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_border_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_border_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_border_side(lv_obj_t * obj, lv_border_side_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_border_post(lv_obj_t * obj, bool value, lv_style_selector_t selector)
        // void lv_obj_set_style_outline_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_outline_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_outline_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_outline_pad(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_offset_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_offset_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_spread(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_shadow_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_dash_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_dash_gap(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_line_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_arc_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_arc_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector)
        // void lv_obj_set_style_arc_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_arc_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_arc_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_font(lv_obj_t * obj, const lv_font_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_letter_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_line_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_decor(lv_obj_t * obj, lv_text_decor_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_text_align(lv_obj_t * obj, lv_text_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_radius(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_clip_corner(lv_obj_t * obj, bool value, lv_style_selector_t selector)
        // void lv_obj_set_style_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_opa_layered(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_color_filter_dsc(lv_obj_t * obj, const lv_color_filter_dsc_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_color_filter_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_anim(lv_obj_t * obj, const lv_anim_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_anim_duration(lv_obj_t * obj, uint32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_transition(lv_obj_t * obj, const lv_style_transition_dsc_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_blend_mode(lv_obj_t * obj, lv_blend_mode_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_layout(lv_obj_t * obj, uint16_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_base_dir(lv_obj_t * obj, lv_base_dir_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_flex_flow(lv_obj_t * obj, lv_flex_flow_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_flex_main_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_flex_cross_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_flex_track_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_flex_grow(lv_obj_t * obj, uint8_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_column_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_column_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_row_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_row_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_column_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_x_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_column_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_row_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_y_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)
        // void lv_obj_set_style_grid_cell_row_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
        // void lv_obj_delete_anim_completed_cb(lv_anim_t * a)
        // lv_obj_t * lv_obj_get_child_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)
        // lv_obj_t * lv_obj_get_sibling_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)
        // uint32_t lv_obj_get_child_count_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // int32_t lv_obj_get_index_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data)
        // void lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
        // void lv_obj_set_grid_dsc_array(lv_obj_t * obj, const int32_t col_dsc[], const int32_t row_dsc[])
        // lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    
    Obj::Obj(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : EventEmitter(ctx, build(ctx, jsobj))
        , _lvobj(lvobj)
    {
        if(_lvobj) {
            if(!lv_obj_get_user_data(_lvobj)) {
                lv_obj_set_user_data(_lvobj, (void *)this) ;
            }

            lv_obj_add_event_cb(_lvobj, eventCallback, LV_EVENT_DELETE, ctx) ;

            // Obj C++/js 都增加一个引用计数，直到 lv_obj_t 销毁的时候再回收
            // 如果由 js 创建，但 js 没有保存变量，同时 js 还无法通过 lv 的对象树访问到这个对象，则无法销毁
            // c++/js counter 在 eventCallback() 中 -1
            // @todo 后续可优化
            shared() ;                  // c++ counter +1
            JS_DupValue(ctx, jsobj) ;   // js counter +1
        }
    }

    Obj::Obj(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Obj::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        if(!lv_display_get_default()) {
            JSTHROW("not create display")
        }

        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }

        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Obj * widget = new Obj(ctx, obj, lv_obj_create(lvparent)) ;
        return widget->jsobj ;
    }

    Obj::~Obj() {
        if(_lvobj) {
            lv_obj_set_user_data(_lvobj, nullptr) ;
            _lvobj = nullptr ;
        }
    }

    lv_obj_t * Obj::lvobj() const {
        return _lvobj ;
    }
    
    #define CREATE_WIDGET(typename, classname)                                      \
        if(lv_obj_check_type(lvobj, &lv_##typename##_class)) {                      \
            if(!widget) {                                                           \
                return new classname(ctx, classname::build(ctx, JS_NULL), lvobj) ;  \
            } else if(JS_IsNone(widget->jsobj)) {                                   \
                widget->jsobj = classname::build(ctx) ;                             \
                JS_SetOpaque(widget->jsobj, widget) ;                               \
            }                                                                       \
        }

    Obj * Obj::wrap(JSContext * ctx, lv_obj_t * lvobj) {
        Obj * widget = (Obj *)lv_obj_get_user_data(lvobj) ;

// AUTO GENERATE CODE START [WRAP WIDGETS] --------
        CREATE_WIDGET(obj, Obj)
        else CREATE_WIDGET(animimg, AnimImg)
        else CREATE_WIDGET(button, Btn)
        else CREATE_WIDGET(canvas, Canvas)
        else CREATE_WIDGET(dropdown, Dropdown)
        else CREATE_WIDGET(keyboard, Keyboard)
        else CREATE_WIDGET(line, Line)
        else CREATE_WIDGET(msgbox, MsgBox)
        else CREATE_WIDGET(scale, Scale)
        else CREATE_WIDGET(spinbox, SpinBox)
        else CREATE_WIDGET(table, Table)
        else CREATE_WIDGET(tileview, TileView)
        else CREATE_WIDGET(arc, Arc)
        else CREATE_WIDGET(buttonmatrix, BtnMatrix)
        else CREATE_WIDGET(chart, Chart)
        else CREATE_WIDGET(image, Img)
        else CREATE_WIDGET(label, Label)
        else CREATE_WIDGET(list, List)
        else CREATE_WIDGET(slider, Slider)
        else CREATE_WIDGET(spinner, Spinner)
        else CREATE_WIDGET(tabview, TabView)
        else CREATE_WIDGET(win, Win)
        else CREATE_WIDGET(bar, Bar)
        else CREATE_WIDGET(calendar, Calendar)
        else CREATE_WIDGET(checkbox, Checkbox)
        else CREATE_WIDGET(imagebutton, ImgBtn)
        else CREATE_WIDGET(led, Led)
        else CREATE_WIDGET(menu, Menu)
        else CREATE_WIDGET(roller, Roller)
        else CREATE_WIDGET(spangroup, Span)
        else CREATE_WIDGET(switch, Switch)
        else CREATE_WIDGET(textarea, TextArea)
        else CREATE_WIDGET(gif, Gif)
// AUTO GENERATE CODE END [WRAP WIDGETS] --------
        else {
            return nullptr ;
        }

        return widget ;
    }

    JSValue Obj::invalidGetter(JSContext *ctx, JSValueConst this_val) {
        return JS_UNDEFINED ;
    }
    JSValue Obj::invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
        return JS_UNDEFINED ;
    }

    #define SETTER_Obj_Width
    #define SETTER_Obj_Height
    
// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Obj_State
    JSValue Obj::getState(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_state_t value = lv_obj_get_state(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    // unspported type: lv_group_t *
    // JSValue Obj::getGroup(JSContext *ctx, JSValueConst this_val){}
    // lv_group_t * lv_obj_get_group(const lv_obj_t * obj)
    // unspported type: const lv_obj_class_t *
    // JSValue Obj::getClass(JSContext *ctx, JSValueConst this_val){}
    // const lv_obj_class_t * lv_obj_get_class(const lv_obj_t * obj)
    #ifndef GETTER_Obj_EventCount
    JSValue Obj::getEventCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t value = lv_obj_get_event_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_X
    JSValue Obj::getX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_X
    JSValue Obj::setX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","setX","number")
        }
        lv_obj_set_x(thisobj->lvobj(), x) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_Y
    JSValue Obj::getY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_Y
    JSValue Obj::setY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","setY","number")
        }
        lv_obj_set_y(thisobj->lvobj(), y) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_Width
    JSValue Obj::getWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_Width
    JSValue Obj::setWidth(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t width ;
        if(JS_ToInt32(ctx, (int32_t *) &width, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","width","Obj","setWidth","number")
        }
        lv_obj_set_width(thisobj->lvobj(), width) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_Height
    JSValue Obj::getHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_Height
    JSValue Obj::setHeight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t height ;
        if(JS_ToInt32(ctx, (int32_t *) &height, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","height","Obj","setHeight","number")
        }
        lv_obj_set_height(thisobj->lvobj(), height) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ContentWidth
    JSValue Obj::getContentWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_content_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ContentWidth
    JSValue Obj::setContentWidth(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t contentWidth ;
        if(JS_ToInt32(ctx, (int32_t *) &contentWidth, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","contentWidth","Obj","setContentWidth","number")
        }
        lv_obj_set_content_width(thisobj->lvobj(), contentWidth) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ContentHeight
    JSValue Obj::getContentHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_content_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ContentHeight
    JSValue Obj::setContentHeight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t contentHeight ;
        if(JS_ToInt32(ctx, (int32_t *) &contentHeight, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","contentHeight","Obj","setContentHeight","number")
        }
        lv_obj_set_content_height(thisobj->lvobj(), contentHeight) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Obj_Layout
    JSValue Obj::setLayout(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t layout ;
        if(JS_ToUint32(ctx, (uint32_t *) &layout, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","layout","Obj","setLayout","number")
        }
        lv_obj_set_layout(thisobj->lvobj(), layout) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Obj_Align
    JSValue Obj::setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv align
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_align_t align;
        if(!lv_align_str_to_const(cstr_val,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_align_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_align(thisobj->lvobj(), align) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_XAligned
    JSValue Obj::getXAligned(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_x_aligned(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_YAligned
    JSValue Obj::getYAligned(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_y_aligned(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_SelfWidth
    JSValue Obj::getSelfWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_self_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_SelfHeight
    JSValue Obj::getSelfHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_self_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ExtClickArea
    JSValue Obj::setExtClickArea(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t extClickArea ;
        if(JS_ToInt32(ctx, (int32_t *) &extClickArea, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","extClickArea","Obj","setExtClickArea","number")
        }
        lv_obj_set_ext_click_area(thisobj->lvobj(), extClickArea) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollbarMode
    JSValue Obj::getScrollbarMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scrollbar_mode_t value = lv_obj_get_scrollbar_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ScrollbarMode
    JSValue Obj::setScrollbarMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollbarMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scrollbar_mode_t scrollbarMode;
        if(!lv_scrollbar_mode_str_to_const(cstr_val,&scrollbarMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scrollbar_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scrollbar_mode(thisobj->lvobj(), scrollbarMode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollDir
    JSValue Obj::getScrollDir(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_dir_t value = lv_obj_get_scroll_dir(thisobj->lvobj()) ;
        JSValue retval = lv_dir_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ScrollDir
    JSValue Obj::setScrollDir(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollDir
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t scrollDir;
        if(!lv_dir_str_to_const(cstr_val,&scrollDir)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_dir(thisobj->lvobj(), scrollDir) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollSnapX
    JSValue Obj::getScrollSnapX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scroll_snap_t value = lv_obj_get_scroll_snap_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ScrollSnapX
    JSValue Obj::setScrollSnapX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollSnapX
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scroll_snap_t scrollSnapX;
        if(!lv_scroll_snap_str_to_const(cstr_val,&scrollSnapX)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scroll_snap_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_snap_x(thisobj->lvobj(), scrollSnapX) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollSnapY
    JSValue Obj::getScrollSnapY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scroll_snap_t value = lv_obj_get_scroll_snap_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_ScrollSnapY
    JSValue Obj::setScrollSnapY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollSnapY
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scroll_snap_t scrollSnapY;
        if(!lv_scroll_snap_str_to_const(cstr_val,&scrollSnapY)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scroll_snap_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_snap_y(thisobj->lvobj(), scrollSnapY) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollX
    JSValue Obj::getScrollX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollY
    JSValue Obj::getScrollY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollTop
    JSValue Obj::getScrollTop(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_top(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollBottom
    JSValue Obj::getScrollBottom(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_bottom(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollLeft
    JSValue Obj::getScrollLeft(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_left(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_ScrollRight
    JSValue Obj::getScrollRight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_right(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_Parent
    JSValue Obj::getParent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * value = lv_obj_get_parent(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_Parent
    JSValue Obj::setParent(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        JSVALUE_TO_LVOBJ(val,parent)
        lv_obj_set_parent(thisobj->lvobj(), parent) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Obj_Screen
    JSValue Obj::getScreen(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * value = lv_obj_get_screen(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    // unspported type: lv_display_t *
    // JSValue Obj::getDisplay(JSContext *ctx, JSValueConst this_val){}
    // lv_display_t * lv_obj_get_display(const lv_obj_t * obj)
    #ifndef GETTER_Obj_ChildCount
    JSValue Obj::getChildCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t value = lv_obj_get_child_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Obj_Index
    JSValue Obj::getIndex(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_index(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Obj_FlexFlow
    JSValue Obj::setFlexFlow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv flexFlow
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_flex_flow_t flexFlow;
        if(!lv_flex_flow_str_to_const(cstr_val,&flexFlow)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_flex_flow_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_flex_flow(thisobj->lvobj(), flexFlow) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Obj_FlexGrow
    JSValue Obj::setFlexGrow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        uint8_t flexGrow ;
        if(JS_ToUint32(ctx, (uint32_t *) &flexGrow, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","flexGrow","Obj","setFlexGrow","number")
        }
        lv_obj_set_flex_grow(thisobj->lvobj(), flexGrow) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------


// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Obj_addFlag
    JSValue Obj::addFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv f
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_obj_flag_t f;
        if(!lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_add_flag( thisobj->lvobj(), f ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_removeFlag
    JSValue Obj::removeFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv f
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_obj_flag_t f;
        if(!lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_remove_flag( thisobj->lvobj(), f ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_updateFlag
    JSValue Obj::updateFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        // argv f
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_obj_flag_t f;
        if(!lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        bool v = JS_ToBool(ctx, argv[1]) ;
        lv_obj_update_flag( thisobj->lvobj(), f, v ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_addState
    JSValue Obj::addState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv state
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_state_t state;
        if(!lv_state_str_to_const(cstr_argv_0_,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_add_state( thisobj->lvobj(), state ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_removeState
    JSValue Obj::removeState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv state
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_state_t state;
        if(!lv_state_str_to_const(cstr_argv_0_,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_remove_state( thisobj->lvobj(), state ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_setState
    JSValue Obj::setState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        // argv state
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_state_t state;
        if(!lv_state_str_to_const(cstr_argv_0_,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        bool v = JS_ToBool(ctx, argv[1]) ;
        lv_obj_set_state( thisobj->lvobj(), state, v ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_hasFlag
    JSValue Obj::hasFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv f
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_obj_flag_t f;
        if(!lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        bool retval = lv_obj_has_flag( thisobj->lvobj(), f ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_hasFlagAny
    JSValue Obj::hasFlagAny(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv f
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_obj_flag_t f;
        if(!lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        bool retval = lv_obj_has_flag_any( thisobj->lvobj(), f ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_hasState
    JSValue Obj::hasState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv state
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_state_t state;
        if(!lv_state_str_to_const(cstr_argv_0_,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        bool retval = lv_obj_has_state( thisobj->lvobj(), state ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_allocateSpecAttr
    JSValue Obj::allocateSpecAttr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_allocate_spec_attr( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: const lv_obj_class_t *
    // bool lv_obj_check_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)

    // Unsupported arg type: const lv_obj_class_t *
    // bool lv_obj_has_class(const lv_obj_t * obj, const lv_obj_class_t * class_p)

    #ifndef METHOD_Obj_isValid
    JSValue Obj::isValid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_valid( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const lv_obj_class_t *
    // lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj_t * parent)

    #ifndef METHOD_Obj_classInitObj
    JSValue Obj::classInitObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_class_init_obj( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_isEditable
    JSValue Obj::isEditable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_editable( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_isGroupDef
    JSValue Obj::isGroupDef(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_group_def( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: lv_draw_rect_dsc_t *
    // void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint32_t part, lv_draw_rect_dsc_t * draw_dsc)

    // Unsupported arg type: lv_draw_label_dsc_t *
    // void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint32_t part, lv_draw_label_dsc_t * draw_dsc)

    // Unsupported arg type: lv_draw_image_dsc_t *
    // void lv_obj_init_draw_image_dsc(lv_obj_t * obj, uint32_t part, lv_draw_image_dsc_t * draw_dsc)

    // Unsupported arg type: lv_draw_line_dsc_t *
    // void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint32_t part, lv_draw_line_dsc_t * draw_dsc)

    // Unsupported arg type: lv_draw_arc_dsc_t *
    // void lv_obj_init_draw_arc_dsc(lv_obj_t * obj, uint32_t part, lv_draw_arc_dsc_t * draw_dsc)

    #ifndef METHOD_Obj_calculateExtDrawSize
    JSValue Obj::calculateExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t part ;
        if(JS_ToUint32(ctx, (uint32_t *) &part, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","part","Obj","calculateExtDrawSize","number")
        }
        int32_t retval = lv_obj_calculate_ext_draw_size( thisobj->lvobj(), part ) ;
        JSValue jsretval = JS_NewInt32(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_refreshExtDrawSize
    JSValue Obj::refreshExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_refresh_ext_draw_size( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: void *
    // lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)

    // Unsupported arg type: const lv_obj_class_t *
    // lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)

    // Unsupported arg type: lv_event_cb_t
    // void lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)

        // Unsupported return type: lv_event_dsc_t *
        // lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)

    #ifndef METHOD_Obj_removeEvent
    JSValue Obj::removeEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t index ;
        if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","index","Obj","removeEvent","number")
        }
        bool retval = lv_obj_remove_event( thisobj->lvobj(), index ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: lv_event_cb_t
    // bool lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)

    // Unsupported arg type: lv_event_cb_t
    // uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)

    #ifndef METHOD_Obj_setPos
    JSValue Obj::setPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","setPos","number")
        }
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","setPos","number")
        }
        lv_obj_set_pos( thisobj->lvobj(), x, y ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_setSize
    JSValue Obj::setSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t w ;
        if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","w","Obj","setSize","number")
        }
        int32_t h ;
        if(JS_ToInt32(ctx, (int32_t *) &h, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","h","Obj","setSize","number")
        }
        lv_obj_set_size( thisobj->lvobj(), w, h ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_refrSize
    JSValue Obj::refrSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_refr_size( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_isLayoutPositioned
    JSValue Obj::isLayoutPositioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_layout_positioned( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_markLayoutAsDirty
    JSValue Obj::markLayoutAsDirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_mark_layout_as_dirty( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_updateLayout
    JSValue Obj::updateLayout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_update_layout( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_alignTo
    JSValue Obj::alignTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(4)
        JSVALUE_TO_LVOBJ(argv[0],base)
        // argv align
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_align_t align;
        if(!lv_align_str_to_const(cstr_argv_1_,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_align_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        int32_t x_ofs ;
        if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[2])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x_ofs","Obj","alignTo","number")
        }
        int32_t y_ofs ;
        if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[3])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y_ofs","Obj","alignTo","number")
        }
        lv_obj_align_to( thisobj->lvobj(), base, align, x_ofs, y_ofs ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_area_t *
    // void lv_obj_get_coords(const lv_obj_t * obj, lv_area_t * coords)

    // Unsupported arg type: lv_area_t *
    // void lv_obj_get_content_coords(const lv_obj_t * obj, lv_area_t * area)

    #ifndef METHOD_Obj_refreshSelfSize
    JSValue Obj::refreshSelfSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_refresh_self_size( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_refrPos
    JSValue Obj::refrPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_refr_pos( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_moveTo
    JSValue Obj::moveTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","moveTo","number")
        }
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","moveTo","number")
        }
        lv_obj_move_to( thisobj->lvobj(), x, y ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_moveChildrenBy
    JSValue Obj::moveChildrenBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        int32_t x_diff ;
        if(JS_ToInt32(ctx, (int32_t *) &x_diff, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x_diff","Obj","moveChildrenBy","number")
        }
        int32_t y_diff ;
        if(JS_ToInt32(ctx, (int32_t *) &y_diff, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y_diff","Obj","moveChildrenBy","number")
        }
        bool ignore_floating = JS_ToBool(ctx, argv[2]) ;
        lv_obj_move_children_by( thisobj->lvobj(), x_diff, y_diff, ignore_floating ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_point_t *
    // void lv_obj_transform_point(const lv_obj_t * obj, lv_point_t * p, bool recursive, bool inv)

    // Unsupported arg type: lv_area_t *
    // void lv_obj_get_transformed_area(const lv_obj_t * obj, lv_area_t * area, bool recursive, bool inv)

    // Unsupported arg type: const lv_area_t *
    // void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area)

    #ifndef METHOD_Obj_invalidate
    JSValue Obj::invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_invalidate( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_area_t *
    // bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area)

    #ifndef METHOD_Obj_isVisible
    JSValue Obj::isVisible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_visible( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: lv_area_t *
    // void lv_obj_get_click_area(const lv_obj_t * obj, lv_area_t * area)

    // Unsupported arg type: const lv_point_t *
    // bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point)

    // Unsupported arg type: const lv_property_t *
    // lv_result_t lv_obj_set_property(lv_obj_t * obj, const lv_property_t * value)

    // Unsupported arg type: const lv_property_t *
    // lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)

    // Unsupported arg type: lv_prop_id_t
    // lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)

    // Unsupported arg type: lv_point_t *
    // void lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)

    #ifndef METHOD_Obj_scrollBy
    JSValue Obj::scrollBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollBy","number")
        }
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollBy","number")
        }
        // argv anim_en
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_obj_scroll_by( thisobj->lvobj(), x, y, anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollByBounded
    JSValue Obj::scrollByBounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        int32_t dx ;
        if(JS_ToInt32(ctx, (int32_t *) &dx, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","dx","Obj","scrollByBounded","number")
        }
        int32_t dy ;
        if(JS_ToInt32(ctx, (int32_t *) &dy, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","dy","Obj","scrollByBounded","number")
        }
        // argv anim_en
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_obj_scroll_by_bounded( thisobj->lvobj(), dx, dy, anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollTo
    JSValue Obj::scrollTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollTo","number")
        }
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollTo","number")
        }
        // argv anim_en
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_obj_scroll_to( thisobj->lvobj(), x, y, anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollToX
    JSValue Obj::scrollToX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollToX","number")
        }
        // argv anim_en
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_1_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_obj_scroll_to_x( thisobj->lvobj(), x, anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollToY
    JSValue Obj::scrollToY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollToY","number")
        }
        // argv anim_en
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_1_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_obj_scroll_to_y( thisobj->lvobj(), y, anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollToView
    JSValue Obj::scrollToView(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv anim_en
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_scroll_to_view( thisobj->lvobj(), anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_scrollToViewRecursive
    JSValue Obj::scrollToViewRecursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv anim_en
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_scroll_to_view_recursive( thisobj->lvobj(), anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_isScrolling
    JSValue Obj::isScrolling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_obj_is_scrolling( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Obj_updateSnap
    JSValue Obj::updateSnap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv anim_en
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_update_snap( thisobj->lvobj(), anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_area_t *
    // void lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor, lv_area_t * ver)

    #ifndef METHOD_Obj_scrollbarInvalidate
    JSValue Obj::scrollbarInvalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_scrollbar_invalidate( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_readjustScroll
    JSValue Obj::readjustScroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        // argv anim_en
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_anim_enable_t anim_en;
        if(!lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        lv_obj_readjust_scroll( thisobj->lvobj(), anim_en ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: const lv_style_t *
    // void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)

    // Unsupported arg type: const lv_style_t *
    // bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style, lv_style_selector_t selector)

    // Unsupported arg type: const lv_style_t *
    // void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)

    #ifndef METHOD_Obj_removeStyleAll
    JSValue Obj::removeStyleAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_remove_style_all( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_style_t *
    // void lv_obj_report_style_change(lv_style_t * style)

    // Unsupported arg type: lv_part_t
    // void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)

    // Unsupported arg type: bool
    // void lv_obj_enable_style_refresh(bool en)

    // Unsupported arg type: lv_part_t
    // lv_style_value_t lv_obj_get_style_prop(const lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)

    // Unsupported arg type: lv_style_selector_t
    // bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)

    // Unsupported arg type: lv_style_value_t
    // void lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_value_t *
    // lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)

    #ifndef METHOD_Obj_fadeIn
    JSValue Obj::fadeIn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t time ;
        if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","time","Obj","fadeIn","number")
        }
        uint32_t delay ;
        if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","delay","Obj","fadeIn","number")
        }
        lv_obj_fade_in( thisobj->lvobj(), time, delay ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_fadeOut
    JSValue Obj::fadeOut(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t time ;
        if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","time","Obj","fadeOut","number")
        }
        uint32_t delay ;
        if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","delay","Obj","fadeOut","number")
        }
        lv_obj_fade_out( thisobj->lvobj(), time, delay ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_part_t
    // lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt)

    // Unsupported arg type: lv_part_t
    // lv_opa_t lv_obj_get_style_opa_recursive(const lv_obj_t * obj, lv_part_t part)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_min_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_max_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_min_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_max_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_length(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_align(lv_obj_t * obj, lv_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_translate_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_translate_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_scale_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_scale_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_rotation(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_pivot_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_pivot_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_skew_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_transform_skew_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_row(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_pad_column(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_margin_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_margin_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_margin_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_margin_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_grad_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_grad_dir(lv_obj_t * obj, lv_grad_dir_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_main_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_grad_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_main_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_grad_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: const lv_grad_dsc_t *
    // void lv_obj_set_style_bg_grad(lv_obj_t * obj, const lv_grad_dsc_t * value, lv_style_selector_t selector)

    // Unsupported arg type: const void *
    // void lv_obj_set_style_bg_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_bg_image_tiled(lv_obj_t * obj, bool value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_border_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_border_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_border_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_border_side(lv_obj_t * obj, lv_border_side_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_border_post(lv_obj_t * obj, bool value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_outline_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_outline_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_outline_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_outline_pad(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_offset_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_offset_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_spread(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_shadow_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_dash_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_dash_gap(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_line_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_arc_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_arc_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_arc_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_arc_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: const void *
    // void lv_obj_set_style_arc_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: const lv_font_t *
    // void lv_obj_set_style_text_font(lv_obj_t * obj, const lv_font_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_letter_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_line_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_decor(lv_obj_t * obj, lv_text_decor_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_text_align(lv_obj_t * obj, lv_text_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_radius(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_clip_corner(lv_obj_t * obj, bool value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_opa_layered(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: const lv_color_filter_dsc_t *
    // void lv_obj_set_style_color_filter_dsc(lv_obj_t * obj, const lv_color_filter_dsc_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_color_filter_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector)

    // Unsupported arg type: const lv_anim_t *
    // void lv_obj_set_style_anim(lv_obj_t * obj, const lv_anim_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_anim_duration(lv_obj_t * obj, uint32_t value, lv_style_selector_t selector)

    // Unsupported arg type: const lv_style_transition_dsc_t *
    // void lv_obj_set_style_transition(lv_obj_t * obj, const lv_style_transition_dsc_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_blend_mode(lv_obj_t * obj, lv_blend_mode_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_layout(lv_obj_t * obj, uint16_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_base_dir(lv_obj_t * obj, lv_base_dir_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_flex_flow(lv_obj_t * obj, lv_flex_flow_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_flex_main_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_flex_cross_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_flex_track_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_flex_grow(lv_obj_t * obj, uint8_t value, lv_style_selector_t selector)

    // Unsupported arg type: const int32_t *
    // void lv_obj_set_style_grid_column_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_column_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: const int32_t *
    // void lv_obj_set_style_grid_row_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_row_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_column_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_x_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_column_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_row_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_y_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector)

    // Unsupported arg type: lv_style_selector_t
    // void lv_obj_set_style_grid_cell_row_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)

    #ifndef METHOD_Obj__delete
    JSValue Obj::_delete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_delete( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_clean
    JSValue Obj::clean(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_clean( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_deleteDelayed
    JSValue Obj::deleteDelayed(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t delay_ms ;
        if(JS_ToUint32(ctx, (uint32_t *) &delay_ms, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","delay_ms","Obj","deleteDelayed","number")
        }
        lv_obj_delete_delayed( thisobj->lvobj(), delay_ms ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_anim_t *
    // void lv_obj_delete_anim_completed_cb(lv_anim_t * a)

    #ifndef METHOD_Obj_deleteAsync
    JSValue Obj::deleteAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_delete_async( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_swap
    JSValue Obj::swap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        JSVALUE_TO_LVOBJ(argv[0],obj2)
        lv_obj_swap( thisobj->lvobj(), obj2 ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_moveToIndex
    JSValue Obj::moveToIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        int32_t index ;
        if(JS_ToInt32(ctx, (int32_t *) &index, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","index","Obj","moveToIndex","number")
        }
        lv_obj_move_to_index( thisobj->lvobj(), index ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_getChild
    JSValue Obj::getChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        int32_t idx ;
        if(JS_ToInt32(ctx, (int32_t *) &idx, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","idx","Obj","getChild","number")
        }
        lv_obj_t * retval = lv_obj_get_child( thisobj->lvobj(), idx ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const lv_obj_class_t *
    // lv_obj_t * lv_obj_get_child_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)

    #ifndef METHOD_Obj_getSibling
    JSValue Obj::getSibling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        int32_t idx ;
        if(JS_ToInt32(ctx, (int32_t *) &idx, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","idx","Obj","getSibling","number")
        }
        lv_obj_t * retval = lv_obj_get_sibling( thisobj->lvobj(), idx ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const lv_obj_class_t *
    // lv_obj_t * lv_obj_get_sibling_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)

    // Unsupported arg type: const lv_obj_class_t *
    // uint32_t lv_obj_get_child_count_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)

    // Unsupported arg type: const lv_obj_class_t *
    // int32_t lv_obj_get_index_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)

    // Unsupported arg type: lv_obj_tree_walk_cb_t
    // void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data)

    #ifndef METHOD_Obj_dumpTree
    JSValue Obj::dumpTree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_dump_tree( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_layer_t *
    // void lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)

    #ifndef METHOD_Obj_setFlexAlign
    JSValue Obj::setFlexAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        // argv main_place
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_flex_align_t main_place;
        if(!lv_flex_align_str_to_const(cstr_argv_0_,&main_place)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_flex_align_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        // argv cross_place
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_flex_align_t cross_place;
        if(!lv_flex_align_str_to_const(cstr_argv_1_,&cross_place)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_flex_align_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        // argv track_cross_place
        const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
        lv_flex_align_t track_cross_place;
        if(!lv_flex_align_str_to_const(cstr_argv_2_,&track_cross_place)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_flex_align_t",cstr_argv_2_) ;
            JS_FreeCString(ctx, cstr_argv_2_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_2_) ;
        lv_obj_set_flex_align( thisobj->lvobj(), main_place, cross_place, track_cross_place ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: const int32_t*
    // void lv_obj_set_grid_dsc_array(lv_obj_t * obj, const int32_t col_dsc[], const int32_t row_dsc[])

    #ifndef METHOD_Obj_setGridAlign
    JSValue Obj::setGridAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        // argv column_align
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_grid_align_t column_align;
        if(!lv_grid_align_str_to_const(cstr_argv_0_,&column_align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_grid_align_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        // argv row_align
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_grid_align_t row_align;
        if(!lv_grid_align_str_to_const(cstr_argv_1_,&row_align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_grid_align_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_obj_set_grid_align( thisobj->lvobj(), column_align, row_align ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Obj_setGridCell
    JSValue Obj::setGridCell(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(6)
        // argv column_align
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_grid_align_t column_align;
        if(!lv_grid_align_str_to_const(cstr_argv_0_,&column_align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_grid_align_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        int32_t col_pos ;
        if(JS_ToInt32(ctx, (int32_t *) &col_pos, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","col_pos","Obj","setGridCell","number")
        }
        int32_t col_span ;
        if(JS_ToInt32(ctx, (int32_t *) &col_span, argv[2])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","col_span","Obj","setGridCell","number")
        }
        // argv row_align
        const char * cstr_argv_3_ = JS_ToCString(ctx, argv[3]) ;
        lv_grid_align_t row_align;
        if(!lv_grid_align_str_to_const(cstr_argv_3_,&row_align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_grid_align_t",cstr_argv_3_) ;
            JS_FreeCString(ctx, cstr_argv_3_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_3_) ;
        int32_t row_pos ;
        if(JS_ToInt32(ctx, (int32_t *) &row_pos, argv[4])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","row_pos","Obj","setGridCell","number")
        }
        int32_t row_span ;
        if(JS_ToInt32(ctx, (int32_t *) &row_span, argv[5])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","row_span","Obj","setGridCell","number")
        }
        lv_obj_set_grid_cell( thisobj->lvobj(), column_align, col_pos, col_span, row_align, row_pos, row_span ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)

    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)

    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)

    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
// AUTO GENERATE CODE END [METHODS] --------
}

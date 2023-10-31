class WASMObject {
    ptr = 0
    static wrap(ptr, clz) {
        if(!ptr) {
            return null
        }
        if(!WASMObject.isPrototypeOf(clz)) {
            throw new Error("arg clz must be a child of WASMObject")
        }
        return {
            ptr, 
            __proto__: clz.prototype
        }
    }
}
class EventEmitter extends WASMObject {
    _handlers = {}
    /**
     * 
     * @param {string|string[]} event 
     * @param {function} handle 
     * @param {bool} norepeat 
     */
    on(event, handle, norepeat) {
        if(typeof(event)=="string") {
            if(!this._handlers[event]){
                this._handlers[event] = []
                if(event!="#EVENT.ADD#"&&event!="#EVENT.CLEAR#")
                    this.emit("#EVENT.ADD#",event)
            }
            if(!norepeat || !this.isListening(event, handle)) {
                this._handlers[event].push(handle)
            }
        }
        else if(event instanceof Array) {
            for(var eventName of event) {
                this.on(eventName, handle, repeat)
            }
        }
        return
    }
    /**
     * 
     * @param {string} eventName 
     * @param {function} handle 
     */
    once(eventName, handle, norepeat) {
        var wrapper =  (...args) => {
            this.off(eventName, wrapper)
            handle.apply(this, args)
        }
        wrapper.__origin = handle
        this.on(eventName, wrapper, norepeat)
        return
    }
    off(eventName, handle, all) {
        if(!this._handlers[eventName]) {
            return
        }
        for(let h=this._handlers[eventName].length-1; h>=0; h--) {
            let func = this.originHanlde(this._handlers[eventName][h])
            if(!handle || handle==func) {
                this._handlers[eventName][h].__origin = null
                this._handlers[eventName].splice(h,1)
                if(handle && !all) {
                    break
                }
            }
        }
        if(!this._handlers[eventName].length) {
            delete this._handlers[eventName]
            if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.CLEAR#")
                this.emit("#EVENT.CLEAR#",eventName)
        }
        return
    }
    originHanlde(handle) {
        for(var h=handle; h.__origin; h=h.__origin){}
        return h
    }
    isListening(event, handle) {
        if(!this._handlers[event])
            return false
        for(let cb of this._handlers[event]) {
            if( this.originHanlde(cb)==handle )
                return true
        }
        return false
    }
    emit(eventName, ...args) {
        if(eventName!='*'&&this._handlers[eventName]) {
            for(let handle of this._handlers[eventName]) {
                handle.apply(this, args)
            }
        }
        if(this._handlers["*"]) {
            for(let handle of this._handlers["*"]) {
                handle.apply(this, [eventName, ...args])
            }
        }
        return
    }
    destroy() {
        for(let eventName in this._handlers) {
            this._handlers[eventName].forEach(cb=>{
                if(cb.__origin) {
                    delete cb.__origin
                }
            })
            this._handlers[eventName].splice(0)
            delete this._handlers[eventName]
        }
    }
}


// AUTO GENERATE CODE START [LVGL.JS WIDGETS] --------
class Obj extends EventEmitter {
    constructor(parent) {
        super()
        this._createWidget(parent)
    }
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_obj_create(parent?parent.ptr:null)
    }
    addFlag(f) {
        Module._lv_obj_add_flag(this.ptr, constMapping.OBJ_FLAG.value(f))
    }
    clearFlag(f) {
        Module._lv_obj_clear_flag(this.ptr, constMapping.OBJ_FLAG.value(f))
    }
    addState(state) {
        Module._lv_obj_add_state(this.ptr, constMapping.STATE.value(state))
    }
    clearState(state) {
        Module._lv_obj_clear_state(this.ptr, constMapping.STATE.value(state))
    }
    hasFlag(f) {
        return Module.ccall("lv_obj_has_flag", "bool", ["number", "number"], [this.ptr, constMapping.OBJ_FLAG.value(f)])
    }
    hasFlagAny(f) {
        return Module.ccall("lv_obj_has_flag_any", "bool", ["number", "number"], [this.ptr, constMapping.OBJ_FLAG.value(f)])
    }
    state() {
        return constMapping.STATE.name(Module._lv_obj_get_state(this.ptr))
    }
    hasState(state) {
        return Module.ccall("lv_obj_has_state", "bool", ["number", "number"], [this.ptr, constMapping.STATE.value(state)])
    }
    allocateSpecAttr() {
        Module._lv_obj_allocate_spec_attr(this.ptr)
    }
    isValid() {
        return Module.ccall("lv_obj_is_valid", "bool", ["number"], [this.ptr])
    }
    dpx(n) {
        return Module._lv_obj_dpx(this.ptr, n)
    }
    isEditable() {
        return Module.ccall("lv_obj_is_editable", "bool", ["number"], [this.ptr])
    }
    setPos(x, y) {
        Module._lv_obj_set_pos(this.ptr, x, y)
    }
    setX(x) {
        Module._lv_obj_set_x(this.ptr, x)
    }
    setY(y) {
        Module._lv_obj_set_y(this.ptr, y)
    }
    setSize(w, h) {
        Module._lv_obj_set_size(this.ptr, w, h)
    }
    refrSize() {
        return Module.ccall("lv_obj_refr_size", "bool", ["number"], [this.ptr])
    }
    setWidth(width) {
        Module._lv_obj_set_width(this.ptr, size(width))
    }
    setHeight(height) {
        Module._lv_obj_set_height(this.ptr, size(height))
    }
    setContentWidth(w) {
        Module._lv_obj_set_content_width(this.ptr, w)
    }
    setContentHeight(h) {
        Module._lv_obj_set_content_height(this.ptr, h)
    }
    setLayout(layout) {
        Module._lv_obj_set_layout(this.ptr, layout)
    }
    isLayoutPositioned() {
        return Module.ccall("lv_obj_is_layout_positioned", "bool", ["number"], [this.ptr])
    }
    markLayoutAsDirty() {
        Module._lv_obj_mark_layout_as_dirty(this.ptr)
    }
    updateLayout() {
        Module._lv_obj_update_layout(this.ptr)
    }
    setAlign(align) {
        Module._lv_obj_set_align(this.ptr, constMapping.ALIGN.value(align))
    }
    align(align, x_ofs, y_ofs) {
        Module._lv_obj_align(this.ptr, constMapping.ALIGN.value(align), x_ofs, y_ofs)
    }
    alignTo(base, align, x_ofs, y_ofs) {
        Module._lv_obj_align_to(this.ptr, base.ptr, constMapping.ALIGN.value(align), x_ofs, y_ofs)
    }
    center() {
        Module._lv_obj_center(this.ptr)
    }
    x() {
        return Module._lv_obj_get_x(this.ptr)
    }
    y() {
        return Module._lv_obj_get_y(this.ptr)
    }
    xAligned() {
        return Module._lv_obj_get_x_aligned(this.ptr)
    }
    yAligned() {
        return Module._lv_obj_get_y_aligned(this.ptr)
    }
    width() {
        return Module._lv_obj_get_width(this.ptr)
    }
    height() {
        return Module._lv_obj_get_height(this.ptr)
    }
    contentWidth() {
        return Module._lv_obj_get_content_width(this.ptr)
    }
    contentHeight() {
        return Module._lv_obj_get_content_height(this.ptr)
    }
    contentCoords(area) {
        Module._lv_obj_get_content_coords(this.ptr, area)
    }
    selfWidth() {
        return Module._lv_obj_get_self_width(this.ptr)
    }
    selfHeight() {
        return Module._lv_obj_get_self_height(this.ptr)
    }
    refreshSelfSize() {
        return Module.ccall("lv_obj_refresh_self_size", "bool", ["number"], [this.ptr])
    }
    refrPos() {
        Module._lv_obj_refr_pos(this.ptr)
    }
    moveTo(x, y) {
        Module._lv_obj_move_to(this.ptr, x, y)
    }
    moveChildrenBy(x_diff, y_diff, ignore_floating) {
        Module._lv_obj_move_children_by(this.ptr, x_diff, y_diff, ignore_floating)
    }
    invalidateArea(area) {
        Module._lv_obj_invalidate_area(this.ptr, area)
    }
    invalidate() {
        Module._lv_obj_invalidate(this.ptr)
    }
    areaIsVisible(area) {
        return Module.ccall("lv_obj_area_is_visible", "bool", ["number", "number"], [this.ptr, area])
    }
    isVisible() {
        return Module.ccall("lv_obj_is_visible", "bool", ["number"], [this.ptr])
    }
    setExtClickArea(size) {
        Module._lv_obj_set_ext_click_area(this.ptr, size)
    }
    clickArea(area) {
        Module._lv_obj_get_click_area(this.ptr, area)
    }
    hitTest(point) {
        return Module.ccall("lv_obj_hit_test", "bool", ["number", "number"], [this.ptr, point])
    }
    setScrollbarMode(mode) {
        Module._lv_obj_set_scrollbar_mode(this.ptr, constMapping.SCROLLBAR_MODE.value(mode))
    }
    setScrollDir(dir) {
        Module._lv_obj_set_scroll_dir(this.ptr, constMapping.DIR.value(dir))
    }
    setScrollSnapX(align) {
        Module._lv_obj_set_scroll_snap_x(this.ptr, constMapping.SCROLL_SNAP.value(align))
    }
    setScrollSnapY(align) {
        Module._lv_obj_set_scroll_snap_y(this.ptr, constMapping.SCROLL_SNAP.value(align))
    }
    scrollbarMode() {
        return constMapping.SCROLLBAR_MODE.name(Module._lv_obj_get_scrollbar_mode(this.ptr))
    }
    scrollDir() {
        return constMapping.DIR.name(Module._lv_obj_get_scroll_dir(this.ptr))
    }
    scrollSnapX() {
        return constMapping.SCROLL_SNAP.name(Module._lv_obj_get_scroll_snap_x(this.ptr))
    }
    scrollSnapY() {
        return constMapping.SCROLL_SNAP.name(Module._lv_obj_get_scroll_snap_y(this.ptr))
    }
    scrollX() {
        return Module._lv_obj_get_scroll_x(this.ptr)
    }
    scrollY() {
        return Module._lv_obj_get_scroll_y(this.ptr)
    }
    scrollTop() {
        return Module._lv_obj_get_scroll_top(this.ptr)
    }
    scrollBottom() {
        return Module._lv_obj_get_scroll_bottom(this.ptr)
    }
    scrollLeft() {
        return Module._lv_obj_get_scroll_left(this.ptr)
    }
    scrollRight() {
        return Module._lv_obj_get_scroll_right(this.ptr)
    }
    scrollEnd(end) {
        Module._lv_obj_get_scroll_end(this.ptr, end)
    }
    scrollBy(x, y, anim_en) {
        Module._lv_obj_scroll_by(this.ptr, x, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollTo(x, y, anim_en) {
        Module._lv_obj_scroll_to(this.ptr, x, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollToX(x, anim_en) {
        Module._lv_obj_scroll_to_x(this.ptr, x, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollToY(y, anim_en) {
        Module._lv_obj_scroll_to_y(this.ptr, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollToView(anim_en) {
        Module._lv_obj_scroll_to_view(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollToViewRecursive(anim_en) {
        Module._lv_obj_scroll_to_view_recursive(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    isScrolling() {
        return Module.ccall("lv_obj_is_scrolling", "bool", ["number"], [this.ptr])
    }
    updateSnap(anim_en) {
        Module._lv_obj_update_snap(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    scrollbarArea(hor, ver) {
        Module._lv_obj_get_scrollbar_area(this.ptr, hor, ver)
    }
    scrollbarInvalidate() {
        Module._lv_obj_scrollbar_invalidate(this.ptr)
    }
    readjustScroll(anim_en) {
        Module._lv_obj_readjust_scroll(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    addStyle(style, selector) {
        Module._lv_obj_add_style(this.ptr, style, selector)
    }
    removeStyle(style, selector) {
        Module._lv_obj_remove_style(this.ptr, style, selector)
    }
    removeStyleAll() {
        Module._lv_obj_remove_style_all(this.ptr)
    }
    fadeIn(time, delay) {
        Module._lv_obj_fade_in(this.ptr, time, delay)
    }
    fadeOut(time, delay) {
        Module._lv_obj_fade_out(this.ptr, time, delay)
    }
    del() {
        Module._lv_obj_del(this.ptr)
    }
    clean() {
        Module._lv_obj_clean(this.ptr)
    }
    delAsync() {
        Module._lv_obj_del_async(this.ptr)
    }
    setParent(parent) {
        Module._lv_obj_set_parent(this.ptr, parent.ptr)
    }
    moveToIndex(index) {
        Module._lv_obj_move_to_index(this.ptr, index)
    }
    screen() {
        return Module._lv_obj_get_screen(this.ptr)
    }
    disp() {
        return WASMObject.wrap(Disp, Module._lv_obj_get_disp(this.ptr))
    }
    parent() {
        return Module._lv_obj_get_parent(this.ptr)
    }
    child(id) {
        return Module._lv_obj_get_child(this.ptr, id)
    }
    childCnt() {
        return Module._lv_obj_get_child_cnt(this.ptr)
    }
    index() {
        return Module._lv_obj_get_index(this.ptr)
    }
    setFlexFlow(flow) {
        Module._lv_obj_set_flex_flow(this.ptr, constMapping.FLEX_FLOW.value(flow))
    }
    setFlexAlign(main_place, cross_place, track_cross_place) {
        Module._lv_obj_set_flex_align(this.ptr, constMapping.FLEX_ALIGN.value(main_place), constMapping.FLEX_ALIGN.value(cross_place), constMapping.FLEX_ALIGN.value(track_cross_place))
    }
    setFlexGrow(grow) {
        Module._lv_obj_set_flex_grow(this.ptr, grow)
    }
    setTile(tile_obj, anim_en) {
        Module._lv_obj_set_tile(this.ptr, tile_obj.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    setTileId(col_id, row_id, anim_en) {
        Module._lv_obj_set_tile_id(this.ptr, col_id, row_id, constMapping.ANIM_ENABLE.value(anim_en))
    }
    moveForeground() {
        Module._lv_obj_move_foreground(this.ptr)
    }
    moveBackground() {
        Module._lv_obj_move_background(this.ptr)
    }
    childId() {
        return Module._lv_obj_get_child_id(this.ptr)
    }
}
class Label extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_label_create(parent?parent.ptr:null)
    }
    setText(text) {
        Module.ccall("lv_label_set_text", "number", ["number", "string"], [this.ptr, text])
    }
    setTextStatic(text) {
        Module.ccall("lv_label_set_text_static", "number", ["number", "string"], [this.ptr, text])
    }
    setLongMode(long_mode) {
        Module._lv_label_set_long_mode(this.ptr, constMapping.LABEL_LONG_MODE.value(long_mode))
    }
    setRecolor(en) {
        Module._lv_label_set_recolor(this.ptr, en)
    }
    setTextSelStart(index) {
        Module._lv_label_set_text_sel_start(this.ptr, index)
    }
    setTextSelEnd(index) {
        Module._lv_label_set_text_sel_end(this.ptr, index)
    }
    text() {
        return Module.ccall("lv_label_get_text", "string", ["number"], [this.ptr])
    }
    longMode() {
        return constMapping.LABEL_LONG_MODE.name(Module._lv_label_get_long_mode(this.ptr))
    }
    recolor() {
        return Module.ccall("lv_label_get_recolor", "bool", ["number"], [this.ptr])
    }
    letterPos(char_id, pos) {
        Module._lv_label_get_letter_pos(this.ptr, char_id, pos)
    }
    letterOn(pos_in) {
        return Module._lv_label_get_letter_on(this.ptr, pos_in)
    }
    isCharUnderPos(pos) {
        return Module.ccall("lv_label_is_char_under_pos", "bool", ["number", "number"], [this.ptr, pos])
    }
    textSelectionStart() {
        return Module._lv_label_get_text_selection_start(this.ptr)
    }
    textSelectionEnd() {
        return Module._lv_label_get_text_selection_end(this.ptr)
    }
    insText(pos, txt) {
        Module.ccall("lv_label_ins_text", "number", ["number", "number", "string"], [this.ptr, pos, txt])
    }
    cutText(pos, cnt) {
        Module._lv_label_cut_text(this.ptr, pos, cnt)
    }
}
class Arc extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_arc_create(parent?parent.ptr:null)
    }
    setStartAngle(start) {
        Module._lv_arc_set_start_angle(this.ptr, start)
    }
    setEndAngle(end) {
        Module._lv_arc_set_end_angle(this.ptr, end)
    }
    setAngles(start, end) {
        Module._lv_arc_set_angles(this.ptr, start, end)
    }
    setBgStartAngle(start) {
        Module._lv_arc_set_bg_start_angle(this.ptr, start)
    }
    setBgEndAngle(end) {
        Module._lv_arc_set_bg_end_angle(this.ptr, end)
    }
    setBgAngles(start, end) {
        Module._lv_arc_set_bg_angles(this.ptr, start, end)
    }
    setRotation(rotation) {
        Module._lv_arc_set_rotation(this.ptr, rotation)
    }
    setMode(type) {
        Module._lv_arc_set_mode(this.ptr, constMapping.ARC_MODE.value(type))
    }
    setValue(value) {
        Module._lv_arc_set_value(this.ptr, value)
    }
    setRange(min, max) {
        Module._lv_arc_set_range(this.ptr, min, max)
    }
    setChangeRate(rate) {
        Module._lv_arc_set_change_rate(this.ptr, rate)
    }
    angleStart() {
        return Module._lv_arc_get_angle_start(this.ptr)
    }
    angleEnd() {
        return Module._lv_arc_get_angle_end(this.ptr)
    }
    bgAngleStart() {
        return Module._lv_arc_get_bg_angle_start(this.ptr)
    }
    bgAngleEnd() {
        return Module._lv_arc_get_bg_angle_end(this.ptr)
    }
    value() {
        return Module._lv_arc_get_value(this.ptr)
    }
    minValue() {
        return Module._lv_arc_get_min_value(this.ptr)
    }
    maxValue() {
        return Module._lv_arc_get_max_value(this.ptr)
    }
    mode() {
        return constMapping.ARC_MODE.name(Module._lv_arc_get_mode(this.ptr))
    }
}
class Bar extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_bar_create(parent?parent.ptr:null)
    }
    setValue(value, anim) {
        Module._lv_bar_set_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    setStartValue(start_value, anim) {
        Module._lv_bar_set_start_value(this.ptr, start_value, constMapping.ANIM_ENABLE.value(anim))
    }
    setRange(min, max) {
        Module._lv_bar_set_range(this.ptr, min, max)
    }
    setMode(mode) {
        Module._lv_bar_set_mode(this.ptr, constMapping.BAR_MODE.value(mode))
    }
    value() {
        return Module._lv_bar_get_value(this.ptr)
    }
    startValue() {
        return Module._lv_bar_get_start_value(this.ptr)
    }
    minValue() {
        return Module._lv_bar_get_min_value(this.ptr)
    }
    maxValue() {
        return Module._lv_bar_get_max_value(this.ptr)
    }
    mode() {
        return constMapping.BAR_MODE.name(Module._lv_bar_get_mode(this.ptr))
    }
}
class Btn extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_btn_create(parent?parent.ptr:null)
    }
}
class BtnMatrix extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_btnmatrix_create(parent?parent.ptr:null)
    }
    popovers() {
        return Module.ccall("lv_btnmatrix_get_popovers", "bool", ["number"], [this.ptr])
    }
    setSelectedBtn(btn_id) {
        Module._lv_btnmatrix_set_selected_btn(this.ptr, btn_id)
    }
    setBtnCtrl(btn_id, ctrl) {
        Module._lv_btnmatrix_set_btn_ctrl(this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    clearBtnCtrl(btn_id, ctrl) {
        Module._lv_btnmatrix_clear_btn_ctrl(this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    setBtnCtrlAll(ctrl) {
        Module._lv_btnmatrix_set_btn_ctrl_all(this.ptr, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    clearBtnCtrlAll(ctrl) {
        Module._lv_btnmatrix_clear_btn_ctrl_all(this.ptr, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    setBtnWidth(btn_id, width) {
        Module._lv_btnmatrix_set_btn_width(this.ptr, btn_id, width)
    }
    setOneChecked(en) {
        Module._lv_btnmatrix_set_one_checked(this.ptr, en)
    }
    selectedBtn() {
        return Module._lv_btnmatrix_get_selected_btn(this.ptr)
    }
    btnText(btn_id) {
        return Module.ccall("lv_btnmatrix_get_btn_text", "string", ["number", "number"], [this.ptr, btn_id])
    }
    hasBtnCtrl(btn_id, ctrl) {
        return Module.ccall("lv_btnmatrix_has_btn_ctrl", "bool", ["number", "number", "number"], [this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl)])
    }
    oneChecked() {
        return Module.ccall("lv_btnmatrix_get_one_checked", "bool", ["number"], [this.ptr])
    }
}
class Canvas extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_canvas_create(parent?parent.ptr:null)
    }
    setPxColor(x, y, c) {
        Module._lv_canvas_set_px_color(this.ptr, x, y, c)
    }
    setPx(x, y, c) {
        Module._lv_canvas_set_px(this.ptr, x, y, c)
    }
    setPxOpa(x, y, opa) {
        Module._lv_canvas_set_px_opa(this.ptr, x, y, opa)
    }
    setPalette(id, c) {
        Module._lv_canvas_set_palette(this.ptr, id, c)
    }
    px(x, y) {
        return Module._lv_canvas_get_px(this.ptr, x, y)
    }
    blurHor(area, r) {
        Module._lv_canvas_blur_hor(this.ptr, area, r)
    }
    blurVer(area, r) {
        Module._lv_canvas_blur_ver(this.ptr, area, r)
    }
    fillBg(color, opa) {
        Module._lv_canvas_fill_bg(this.ptr, color, opa)
    }
    drawRect(x, y, w, h, draw_dsc) {
        Module._lv_canvas_draw_rect(this.ptr, x, y, w, h, draw_dsc)
    }
    drawArc(x, y, r, start_angle, end_angle, draw_dsc) {
        Module._lv_canvas_draw_arc(this.ptr, x, y, r, start_angle, end_angle, draw_dsc)
    }
}
class Checkbox extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_checkbox_create(parent?parent.ptr:null)
    }
    setText(txt) {
        Module.ccall("lv_checkbox_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    setTextStatic(txt) {
        Module.ccall("lv_checkbox_set_text_static", "number", ["number", "string"], [this.ptr, txt])
    }
    text() {
        return Module.ccall("lv_checkbox_get_text", "string", ["number"], [this.ptr])
    }
}
class Dropdown extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_dropdown_create(parent?parent.ptr:null)
    }
    setText(txt) {
        Module.ccall("lv_dropdown_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    setOptions(options) {
        Module.ccall("lv_dropdown_set_options", "number", ["number", "string"], [this.ptr, options])
    }
    setOptionsStatic(options) {
        Module.ccall("lv_dropdown_set_options_static", "number", ["number", "string"], [this.ptr, options])
    }
    addOption(option, pos) {
        Module.ccall("lv_dropdown_add_option", "number", ["number", "string", "number"], [this.ptr, option, pos])
    }
    clearOptions() {
        Module._lv_dropdown_clear_options(this.ptr)
    }
    setSelected(sel_opt) {
        Module._lv_dropdown_set_selected(this.ptr, sel_opt)
    }
    setDir(dir) {
        Module._lv_dropdown_set_dir(this.ptr, constMapping.DIR.value(dir))
    }
    setSelectedHighlight(en) {
        Module._lv_dropdown_set_selected_highlight(this.ptr, en)
    }
    text() {
        return Module.ccall("lv_dropdown_get_text", "string", ["number"], [this.ptr])
    }
    options() {
        return Module.ccall("lv_dropdown_get_options", "string", ["number"], [this.ptr])
    }
    selected() {
        return Module._lv_dropdown_get_selected(this.ptr)
    }
    optionCnt() {
        return Module._lv_dropdown_get_option_cnt(this.ptr)
    }
    selectedStr(buf, buf_size) {
        Module.ccall("lv_dropdown_get_selected_str", "number", ["number", "string", "number"], [this.ptr, buf, buf_size])
    }
    symbol() {
        return Module.ccall("lv_dropdown_get_symbol", "string", ["number"], [this.ptr])
    }
    selectedHighlight() {
        return Module.ccall("lv_dropdown_get_selected_highlight", "bool", ["number"], [this.ptr])
    }
    dir() {
        return constMapping.DIR.name(Module._lv_dropdown_get_dir(this.ptr))
    }
    open() {
        Module._lv_dropdown_open(this.ptr)
    }
    close() {
        Module._lv_dropdown_close(this.ptr)
    }
}
class Img extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_img_create(parent?parent.ptr:null)
    }
    setOffsetX(x) {
        Module._lv_img_set_offset_x(this.ptr, x)
    }
    setOffsetY(y) {
        Module._lv_img_set_offset_y(this.ptr, y)
    }
    setAngle(angle) {
        Module._lv_img_set_angle(this.ptr, angle)
    }
    setPivot(x, y) {
        Module._lv_img_set_pivot(this.ptr, x, y)
    }
    setZoom(zoom) {
        Module._lv_img_set_zoom(this.ptr, zoom)
    }
    setAntialias(antialias) {
        Module._lv_img_set_antialias(this.ptr, antialias)
    }
    setSizeMode(mode) {
        Module._lv_img_set_size_mode(this.ptr, constMapping.IMG_SIZE_MODE.value(mode))
    }
    offsetX() {
        return Module._lv_img_get_offset_x(this.ptr)
    }
    offsetY() {
        return Module._lv_img_get_offset_y(this.ptr)
    }
    angle() {
        return Module._lv_img_get_angle(this.ptr)
    }
    pivot(pivot) {
        Module._lv_img_get_pivot(this.ptr, pivot)
    }
    zoom() {
        return Module._lv_img_get_zoom(this.ptr)
    }
    antialias() {
        return Module.ccall("lv_img_get_antialias", "bool", ["number"], [this.ptr])
    }
    sizeMode() {
        return constMapping.IMG_SIZE_MODE.name(Module._lv_img_get_size_mode(this.ptr))
    }
}
class Line extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_line_create(parent?parent.ptr:null)
    }
    setYInvert(en) {
        Module._lv_line_set_y_invert(this.ptr, en)
    }
    yInvert() {
        return Module.ccall("lv_line_get_y_invert", "bool", ["number"], [this.ptr])
    }
}
class Roller extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_roller_create(parent?parent.ptr:null)
    }
    setOptions(options, mode) {
        Module.ccall("lv_roller_set_options", "number", ["number", "string", "number"], [this.ptr, options, constMapping.ROLLER_MODE.value(mode)])
    }
    setSelected(sel_opt, anim) {
        Module._lv_roller_set_selected(this.ptr, sel_opt, constMapping.ANIM_ENABLE.value(anim))
    }
    setVisibleRowCount(row_cnt) {
        Module._lv_roller_set_visible_row_count(this.ptr, row_cnt)
    }
    selected() {
        return Module._lv_roller_get_selected(this.ptr)
    }
    selectedStr(buf, buf_size) {
        Module.ccall("lv_roller_get_selected_str", "number", ["number", "string", "number"], [this.ptr, buf, buf_size])
    }
    options() {
        return Module.ccall("lv_roller_get_options", "string", ["number"], [this.ptr])
    }
    optionCnt() {
        return Module._lv_roller_get_option_cnt(this.ptr)
    }
}
class Slider extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_slider_create(parent?parent.ptr:null)
    }
    setValue(value, anim) {
        Module._lv_slider_set_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    setLeftValue(value, anim) {
        Module._lv_slider_set_left_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    setRange(min, max) {
        Module._lv_slider_set_range(this.ptr, min, max)
    }
    setMode(mode) {
        Module._lv_slider_set_mode(this.ptr, constMapping.SLIDER_MODE.value(mode))
    }
    value() {
        return Module._lv_slider_get_value(this.ptr)
    }
    leftValue() {
        return Module._lv_slider_get_left_value(this.ptr)
    }
    minValue() {
        return Module._lv_slider_get_min_value(this.ptr)
    }
    maxValue() {
        return Module._lv_slider_get_max_value(this.ptr)
    }
    isDragged() {
        return Module.ccall("lv_slider_is_dragged", "bool", ["number"], [this.ptr])
    }
    mode() {
        return constMapping.SLIDER_MODE.name(Module._lv_slider_get_mode(this.ptr))
    }
}
class Switch extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_switch_create(parent?parent.ptr:null)
    }
}
class Table extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_table_create(parent?parent.ptr:null)
    }
    setCellValue(row, col, txt) {
        Module.ccall("lv_table_set_cell_value", "number", ["number", "number", "number", "string"], [this.ptr, row, col, txt])
    }
    setRowCnt(row_cnt) {
        Module._lv_table_set_row_cnt(this.ptr, row_cnt)
    }
    setColCnt(col_cnt) {
        Module._lv_table_set_col_cnt(this.ptr, col_cnt)
    }
    setColWidth(col_id, w) {
        Module._lv_table_set_col_width(this.ptr, col_id, w)
    }
    addCellCtrl(row, col, ctrl) {
        Module._lv_table_add_cell_ctrl(this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl))
    }
    clearCellCtrl(row, col, ctrl) {
        Module._lv_table_clear_cell_ctrl(this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl))
    }
    cellValue(row, col) {
        return Module.ccall("lv_table_get_cell_value", "string", ["number", "number", "number"], [this.ptr, row, col])
    }
    rowCnt() {
        return Module._lv_table_get_row_cnt(this.ptr)
    }
    colCnt() {
        return Module._lv_table_get_col_cnt(this.ptr)
    }
    colWidth(col) {
        return Module._lv_table_get_col_width(this.ptr, col)
    }
    hasCellCtrl(row, col, ctrl) {
        return Module.ccall("lv_table_has_cell_ctrl", "bool", ["number", "number", "number", "number"], [this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl)])
    }
}
class TextArea extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_textarea_create(parent?parent.ptr:null)
    }
    addText(txt) {
        Module.ccall("lv_textarea_add_text", "number", ["number", "string"], [this.ptr, txt])
    }
    delChar() {
        Module._lv_textarea_del_char(this.ptr)
    }
    delCharForward() {
        Module._lv_textarea_del_char_forward(this.ptr)
    }
    setText(txt) {
        Module.ccall("lv_textarea_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    setPlaceholderText(txt) {
        Module.ccall("lv_textarea_set_placeholder_text", "number", ["number", "string"], [this.ptr, txt])
    }
    setCursorPos(pos) {
        Module._lv_textarea_set_cursor_pos(this.ptr, pos)
    }
    setCursorClickPos(en) {
        Module._lv_textarea_set_cursor_click_pos(this.ptr, en)
    }
    setPasswordMode(en) {
        Module._lv_textarea_set_password_mode(this.ptr, en)
    }
    setOneLine(en) {
        Module._lv_textarea_set_one_line(this.ptr, en)
    }
    setAcceptedChars(list) {
        Module.ccall("lv_textarea_set_accepted_chars", "number", ["number", "string"], [this.ptr, list])
    }
    setMaxLength(num) {
        Module._lv_textarea_set_max_length(this.ptr, num)
    }
    setInsertReplace(txt) {
        Module.ccall("lv_textarea_set_insert_replace", "number", ["number", "string"], [this.ptr, txt])
    }
    setTextSelection(en) {
        Module._lv_textarea_set_text_selection(this.ptr, en)
    }
    setPasswordShowTime(time) {
        Module._lv_textarea_set_password_show_time(this.ptr, time)
    }
    setAlign(align) {
        Module._lv_textarea_set_align(this.ptr, constMapping.TEXT_ALIGN.value(align))
    }
    text() {
        return Module.ccall("lv_textarea_get_text", "string", ["number"], [this.ptr])
    }
    placeholderText() {
        return Module.ccall("lv_textarea_get_placeholder_text", "string", ["number"], [this.ptr])
    }
    label() {
        return WASMObject.wrap(Obj, Module._lv_textarea_get_label(this.ptr))
    }
    cursorPos() {
        return Module._lv_textarea_get_cursor_pos(this.ptr)
    }
    cursorClickPos() {
        return Module.ccall("lv_textarea_get_cursor_click_pos", "bool", ["number"], [this.ptr])
    }
    passwordMode() {
        return Module.ccall("lv_textarea_get_password_mode", "bool", ["number"], [this.ptr])
    }
    oneLine() {
        return Module.ccall("lv_textarea_get_one_line", "bool", ["number"], [this.ptr])
    }
    acceptedChars() {
        return Module.ccall("lv_textarea_get_accepted_chars", "string", ["number"], [this.ptr])
    }
    maxLength() {
        return Module._lv_textarea_get_max_length(this.ptr)
    }
    textIsSelected() {
        return Module.ccall("lv_textarea_text_is_selected", "bool", ["number"], [this.ptr])
    }
    textSelection() {
        return Module.ccall("lv_textarea_get_text_selection", "bool", ["number"], [this.ptr])
    }
    passwordShowTime() {
        return Module._lv_textarea_get_password_show_time(this.ptr)
    }
    clearSelection() {
        Module._lv_textarea_clear_selection(this.ptr)
    }
    cursorRight() {
        Module._lv_textarea_cursor_right(this.ptr)
    }
    cursorLeft() {
        Module._lv_textarea_cursor_left(this.ptr)
    }
    cursorDown() {
        Module._lv_textarea_cursor_down(this.ptr)
    }
    cursorUp() {
        Module._lv_textarea_cursor_up(this.ptr)
    }
}
class MsgBox extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_msgbox_create(parent?parent.ptr:null)
    }
    title() {
        return WASMObject.wrap(Label, Module._lv_msgbox_get_title(this.ptr))
    }
    closeBtn() {
        return WASMObject.wrap(Btn, Module._lv_msgbox_get_close_btn(this.ptr))
    }
    text() {
        return WASMObject.wrap(Label, Module._lv_msgbox_get_text(this.ptr))
    }
    content() {
        return WASMObject.wrap(Obj, Module._lv_msgbox_get_content(this.ptr))
    }
    btns() {
        return WASMObject.wrap(Obj, Module._lv_msgbox_get_btns(this.ptr))
    }
    activeBtn() {
        return Module._lv_msgbox_get_active_btn(this.ptr)
    }
    activeBtnText() {
        return Module.ccall("lv_msgbox_get_active_btn_text", "string", ["number"], [this.ptr])
    }
    close() {
        Module._lv_msgbox_close(this.ptr)
    }
    closeAsync() {
        Module._lv_msgbox_close_async(this.ptr)
    }
}
class Keyboard extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_keyboard_create(parent?parent.ptr:null)
    }
    setTextarea(textarea) {
        Module._lv_keyboard_set_textarea(this.ptr, textarea.ptr)
    }
    setMode(mode) {
        Module._lv_keyboard_set_mode(this.ptr, constMapping.KEYBOARD_MODE.value(mode))
    }
    setPopovers(en) {
        Module._lv_keyboard_set_popovers(this.ptr, en)
    }
    textarea() {
        return WASMObject.wrap(Obj, Module._lv_keyboard_get_textarea(this.ptr))
    }
    mode() {
        return constMapping.KEYBOARD_MODE.name(Module._lv_keyboard_get_mode(this.ptr))
    }
}
class TileView extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_tileview_create(parent?parent.ptr:null)
    }
    addTile(col_id, row_id, dir) {
        return WASMObject.wrap(Obj, Module._lv_tileview_add_tile(this.ptr, col_id, row_id, dir))
    }
    tileAct() {
        return WASMObject.wrap(Obj, Module._lv_tileview_get_tile_act(this.ptr))
    }
}
class List extends Obj {
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._lv_list_create(parent?parent.ptr:null)
    }
    addText(txt) {
        return WASMObject.wrap(Obj, Module.ccall("lv_list_add_text", "number", ["number", "string"], [this.ptr, txt]))
    }
    btnText(btn) {
        return Module.ccall("lv_list_get_btn_text", "string", ["number", "pointer"], [this.ptr, btn.ptr])
    }
}
// AUTO GENERATE CODE END [LVGL.JS WIDGETS] --------




// AUTO GENERATE CODE START [LVGL.JS STRUCTS] --------
class DispDrawBuf extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_draw_buf_create()
    }
    buf1() {
        return Module._lv_disp_draw_buf_get_buf1(this.ptr)
    }
    setBuf1(buf1) {
        Module._lv_disp_draw_buf_set_buf1(this.ptr, buf1)
    }
    buf2() {
        return Module._lv_disp_draw_buf_get_buf2(this.ptr)
    }
    setBuf2(buf2) {
        Module._lv_disp_draw_buf_set_buf2(this.ptr, buf2)
    }
    bufAct() {
        return Module._lv_disp_draw_buf_get_buf_act(this.ptr)
    }
    setBufAct(buf_act) {
        Module._lv_disp_draw_buf_set_buf_act(this.ptr, buf_act)
    }
    size() {
        return Module._lv_disp_draw_buf_get_size(this.ptr)
    }
    setSize(size) {
        Module._lv_disp_draw_buf_set_size(this.ptr, size)
    }
    area() {
        return Module._lv_disp_draw_buf_get_area(this.ptr)
    }
    setArea(area) {
        Module._lv_disp_draw_buf_set_area(this.ptr, area)
    }
    flushing() {
        return Module._lv_disp_draw_buf_get_flushing(this.ptr)
    }
    setFlushing(flushing) {
        Module._lv_disp_draw_buf_set_flushing(this.ptr, flushing)
    }
    flushingLast() {
        return Module._lv_disp_draw_buf_get_flushing_last(this.ptr)
    }
    setFlushingLast(flushing_last) {
        Module._lv_disp_draw_buf_set_flushing_last(this.ptr, flushing_last)
    }
    lastArea() {
        return Module._lv_disp_draw_buf_get_last_area(this.ptr)
    }
    setLastArea(last_area) {
        Module._lv_disp_draw_buf_set_last_area(this.ptr, last_area)
    }
    lastPart() {
        return Module._lv_disp_draw_buf_get_last_part(this.ptr)
    }
    setLastPart(last_part) {
        Module._lv_disp_draw_buf_set_last_part(this.ptr, last_part)
    }
    init(buf1, buf2, size_in_px_cnt) {
        Module._lv_disp_draw_buf_init(this.ptr, buf1, buf2, size_in_px_cnt)
    }
}
class DispDrv extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_drv_create()
    }
    horRes() {
        return Module._lv_disp_drv_get_hor_res(this.ptr)
    }
    setHorRes(hor_res) {
        Module._lv_disp_drv_set_hor_res(this.ptr, hor_res)
    }
    verRes() {
        return Module._lv_disp_drv_get_ver_res(this.ptr)
    }
    setVerRes(ver_res) {
        Module._lv_disp_drv_set_ver_res(this.ptr, ver_res)
    }
    physicalHorRes() {
        return Module._lv_disp_drv_get_physical_hor_res(this.ptr)
    }
    setPhysicalHorRes(physical_hor_res) {
        Module._lv_disp_drv_set_physical_hor_res(this.ptr, physical_hor_res)
    }
    physicalVerRes() {
        return Module._lv_disp_drv_get_physical_ver_res(this.ptr)
    }
    setPhysicalVerRes(physical_ver_res) {
        Module._lv_disp_drv_set_physical_ver_res(this.ptr, physical_ver_res)
    }
    offsetX() {
        return Module._lv_disp_drv_get_offset_x(this.ptr)
    }
    setOffsetX(offset_x) {
        Module._lv_disp_drv_set_offset_x(this.ptr, offset_x)
    }
    offsetY() {
        return Module._lv_disp_drv_get_offset_y(this.ptr)
    }
    setOffsetY(offset_y) {
        Module._lv_disp_drv_set_offset_y(this.ptr, offset_y)
    }
    drawBuf() {
        return WASMObject.wrap(DispDrawBuf, Module._lv_disp_drv_get_draw_buf(this.ptr))
    }
    setDrawBuf(draw_buf) {
        Module._lv_disp_drv_set_draw_buf(this.ptr, draw_buf.ptr)
    }
    directMode() {
        return Module._lv_disp_drv_get_direct_mode(this.ptr)
    }
    setDirectMode(direct_mode) {
        Module._lv_disp_drv_set_direct_mode(this.ptr, direct_mode)
    }
    fullRefresh() {
        return Module._lv_disp_drv_get_full_refresh(this.ptr)
    }
    setFullRefresh(full_refresh) {
        Module._lv_disp_drv_set_full_refresh(this.ptr, full_refresh)
    }
    swRotate() {
        return Module._lv_disp_drv_get_sw_rotate(this.ptr)
    }
    setSwRotate(sw_rotate) {
        Module._lv_disp_drv_set_sw_rotate(this.ptr, sw_rotate)
    }
    antialiasing() {
        return Module._lv_disp_drv_get_antialiasing(this.ptr)
    }
    setAntialiasing(antialiasing) {
        Module._lv_disp_drv_set_antialiasing(this.ptr, antialiasing)
    }
    rotated() {
        return Module._lv_disp_drv_get_rotated(this.ptr)
    }
    setRotated(rotated) {
        Module._lv_disp_drv_set_rotated(this.ptr, rotated)
    }
    screenTransp() {
        return Module._lv_disp_drv_get_screen_transp(this.ptr)
    }
    setScreenTransp(screen_transp) {
        Module._lv_disp_drv_set_screen_transp(this.ptr, screen_transp)
    }
    dpi() {
        return Module._lv_disp_drv_get_dpi(this.ptr)
    }
    setDpi(dpi) {
        Module._lv_disp_drv_set_dpi(this.ptr, dpi)
    }
    flushCb() {
        return Module._lv_disp_drv_get_flush_cb(this.ptr)
    }
    setFlushCb(flush_cb) {
        Module._lv_disp_drv_set_flush_cb(this.ptr, flush_cb)
    }
    rounderCb() {
        return Module._lv_disp_drv_get_rounder_cb(this.ptr)
    }
    setRounderCb(rounder_cb) {
        Module._lv_disp_drv_set_rounder_cb(this.ptr, rounder_cb)
    }
    setPxCb() {
        return Module._lv_disp_drv_get_set_px_cb(this.ptr)
    }
    setSetPxCb(set_px_cb) {
        Module._lv_disp_drv_set_set_px_cb(this.ptr, set_px_cb)
    }
    monitorCb() {
        return Module._lv_disp_drv_get_monitor_cb(this.ptr)
    }
    setMonitorCb(monitor_cb) {
        Module._lv_disp_drv_set_monitor_cb(this.ptr, monitor_cb)
    }
    waitCb() {
        return Module._lv_disp_drv_get_wait_cb(this.ptr)
    }
    setWaitCb(wait_cb) {
        Module._lv_disp_drv_set_wait_cb(this.ptr, wait_cb)
    }
    cleanDcacheCb() {
        return Module._lv_disp_drv_get_clean_dcache_cb(this.ptr)
    }
    setCleanDcacheCb(clean_dcache_cb) {
        Module._lv_disp_drv_set_clean_dcache_cb(this.ptr, clean_dcache_cb)
    }
    gpuWaitCb() {
        return Module._lv_disp_drv_get_gpu_wait_cb(this.ptr)
    }
    setGpuWaitCb(gpu_wait_cb) {
        Module._lv_disp_drv_set_gpu_wait_cb(this.ptr, gpu_wait_cb)
    }
    drvUpdateCb() {
        return Module._lv_disp_drv_get_drv_update_cb(this.ptr)
    }
    setDrvUpdateCb(drv_update_cb) {
        Module._lv_disp_drv_set_drv_update_cb(this.ptr, drv_update_cb)
    }
    gpuFillCb() {
        return Module._lv_disp_drv_get_gpu_fill_cb(this.ptr)
    }
    setGpuFillCb(gpu_fill_cb) {
        Module._lv_disp_drv_set_gpu_fill_cb(this.ptr, gpu_fill_cb)
    }
    colorChromaKey() {
        return Module._lv_disp_drv_get_color_chroma_key(this.ptr)
    }
    setColorChromaKey(color_chroma_key) {
        Module._lv_disp_drv_set_color_chroma_key(this.ptr, color_chroma_key)
    }
    userData() {
        return Module._lv_disp_drv_get_user_data(this.ptr)
    }
    setUserData(user_data) {
        Module._lv_disp_drv_set_user_data(this.ptr, user_data)
    }
    init() {
        Module._lv_disp_drv_init(this.ptr)
    }
    register() {
        return WASMObject.wrap(Disp, Module._lv_disp_drv_register(this.ptr))
    }
    useGenericSetPxCb(cf) {
        Module._lv_disp_drv_use_generic_set_px_cb(this.ptr, constMapping.IMG_CF.value(cf))
    }
}
class Disp extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_create()
    }
    driver() {
        return Module._lv_disp_get_driver(this.ptr)
    }
    setDriver(driver) {
        Module._lv_disp_set_driver(this.ptr, driver)
    }
    refrTimer() {
        return Module._lv_disp_get_refr_timer(this.ptr)
    }
    setRefrTimer(refr_timer) {
        Module._lv_disp_set_refr_timer(this.ptr, refr_timer)
    }
    screens() {
        return Module._lv_disp_get_screens(this.ptr)
    }
    setScreens(screens) {
        Module._lv_disp_set_screens(this.ptr, screens)
    }
    actScr() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_act_scr(this.ptr))
    }
    setActScr(act_scr) {
        Module._lv_disp_set_act_scr(this.ptr, act_scr.ptr)
    }
    prevScr() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_prev_scr(this.ptr))
    }
    setPrevScr(prev_scr) {
        Module._lv_disp_set_prev_scr(this.ptr, prev_scr.ptr)
    }
    scrToLoad() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_scr_to_load(this.ptr))
    }
    setScrToLoad(scr_to_load) {
        Module._lv_disp_set_scr_to_load(this.ptr, scr_to_load.ptr)
    }
    topLayer() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_top_layer(this.ptr))
    }
    setTopLayer(top_layer) {
        Module._lv_disp_set_top_layer(this.ptr, top_layer.ptr)
    }
    sysLayer() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_sys_layer(this.ptr))
    }
    setSysLayer(sys_layer) {
        Module._lv_disp_set_sys_layer(this.ptr, sys_layer.ptr)
    }
    screenCnt() {
        return Module._lv_disp_get_screen_cnt(this.ptr)
    }
    setScreenCnt(screen_cnt) {
        Module._lv_disp_set_screen_cnt(this.ptr, screen_cnt)
    }
    delPrev() {
        return Module._lv_disp_get_del_prev(this.ptr)
    }
    setDelPrev(del_prev) {
        Module._lv_disp_set_del_prev(this.ptr, del_prev)
    }
    bgOpa() {
        return Module._lv_disp_get_bg_opa(this.ptr)
    }
    bgColor() {
        return Module._lv_disp_get_bg_color(this.ptr)
    }
    bgImg() {
        return Module._lv_disp_get_bg_img(this.ptr)
    }
    setBgImg(bg_img) {
        Module._lv_disp_set_bg_img(this.ptr, bg_img)
    }
    bgFn() {
        return Module._lv_disp_get_bg_fn(this.ptr)
    }
    setBgFn(bg_fn) {
        Module._lv_disp_set_bg_fn(this.ptr, bg_fn)
    }
    invAreas() {
        return Module._lv_disp_get_inv_areas(this.ptr)
    }
    setInvAreas(inv_areas) {
        Module._lv_disp_set_inv_areas(this.ptr, inv_areas)
    }
    invAreaJoined() {
        return Module._lv_disp_get_inv_area_joined(this.ptr)
    }
    setInvAreaJoined(inv_area_joined) {
        Module._lv_disp_set_inv_area_joined(this.ptr, inv_area_joined)
    }
    invP() {
        return Module._lv_disp_get_inv_p(this.ptr)
    }
    setInvP(inv_p) {
        Module._lv_disp_set_inv_p(this.ptr, inv_p)
    }
    lastActivityTime() {
        return Module._lv_disp_get_last_activity_time(this.ptr)
    }
    setLastActivityTime(last_activity_time) {
        Module._lv_disp_set_last_activity_time(this.ptr, last_activity_time)
    }
    remove() {
        Module._lv_disp_remove(this.ptr)
    }
    setDefault() {
        Module._lv_disp_set_default(this.ptr)
    }
    horRes() {
        return Module._lv_disp_get_hor_res(this.ptr)
    }
    verRes() {
        return Module._lv_disp_get_ver_res(this.ptr)
    }
    physicalHorRes() {
        return Module._lv_disp_get_physical_hor_res(this.ptr)
    }
    physicalVerRes() {
        return Module._lv_disp_get_physical_ver_res(this.ptr)
    }
    offsetX() {
        return Module._lv_disp_get_offset_x(this.ptr)
    }
    offsetY() {
        return Module._lv_disp_get_offset_y(this.ptr)
    }
    antialiasing() {
        return Module.ccall("lv_disp_get_antialiasing", "bool", ["number"], [this.ptr])
    }
    setRotation(rotation) {
        Module._lv_disp_set_rotation(this.ptr, constMapping.DISP_ROT.value(rotation))
    }
    rotation() {
        return constMapping.DISP_ROT.name(Module._lv_disp_get_rotation(this.ptr))
    }
    next() {
        return WASMObject.wrap(Disp, Module._lv_disp_get_next(this.ptr))
    }
    drawBuf() {
        return WASMObject.wrap(DispDrawBuf, Module._lv_disp_get_draw_buf(this.ptr))
    }
    scrAct() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_scr_act(this.ptr))
    }
    scrPrev() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_scr_prev(this.ptr))
    }
    layerTop() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_layer_top(this.ptr))
    }
    layerSys() {
        return WASMObject.wrap(Obj, Module._lv_disp_get_layer_sys(this.ptr))
    }
    setTheme(th) {
        Module._lv_disp_set_theme(this.ptr, th)
    }
    theme() {
        return Module._lv_disp_get_theme(this.ptr)
    }
    setBgColor(color) {
        Module._lv_disp_set_bg_color(this.ptr, color)
    }
    setBgImage(img_src) {
        Module._lv_disp_set_bg_image(this.ptr, img_src)
    }
    setBgOpa(opa) {
        Module._lv_disp_set_bg_opa(this.ptr, opa)
    }
    trigActivity() {
        Module._lv_disp_trig_activity(this.ptr)
    }
    cleanDcache() {
        Module._lv_disp_clean_dcache(this.ptr)
    }
}
class ObjClass extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_obj_class_create()
    }
    baseClass() {
        return Module._lv_obj_class_get_base_class(this.ptr)
    }
    setBaseClass(base_class) {
        Module._lv_obj_class_set_base_class(this.ptr, base_class)
    }
    constructorCb() {
        return Module._lv_obj_class_get_constructor_cb(this.ptr)
    }
    setConstructorCb(constructor_cb) {
        Module._lv_obj_class_set_constructor_cb(this.ptr, constructor_cb)
    }
    destructorCb() {
        return Module._lv_obj_class_get_destructor_cb(this.ptr)
    }
    setDestructorCb(destructor_cb) {
        Module._lv_obj_class_set_destructor_cb(this.ptr, destructor_cb)
    }
    userData() {
        return Module._lv_obj_class_get_user_data(this.ptr)
    }
    setUserData(user_data) {
        Module._lv_obj_class_set_user_data(this.ptr, user_data)
    }
    eventCb() {
        return Module._lv_obj_class_get_event_cb(this.ptr)
    }
    setEventCb(event_cb) {
        Module._lv_obj_class_set_event_cb(this.ptr, event_cb)
    }
    widthDef() {
        return Module._lv_obj_class_get_width_def(this.ptr)
    }
    setWidthDef(width_def) {
        Module._lv_obj_class_set_width_def(this.ptr, width_def)
    }
    heightDef() {
        return Module._lv_obj_class_get_height_def(this.ptr)
    }
    setHeightDef(height_def) {
        Module._lv_obj_class_set_height_def(this.ptr, height_def)
    }
    editable() {
        return Module._lv_obj_class_get_editable(this.ptr)
    }
    setEditable(editable) {
        Module._lv_obj_class_set_editable(this.ptr, editable)
    }
    groupDef() {
        return Module._lv_obj_class_get_group_def(this.ptr)
    }
    setGroupDef(group_def) {
        Module._lv_obj_class_set_group_def(this.ptr, group_def)
    }
    instanceSize() {
        return Module._lv_obj_class_get_instance_size(this.ptr)
    }
    setInstanceSize(instance_size) {
        Module._lv_obj_class_set_instance_size(this.ptr, instance_size)
    }
}
class IndevDrv extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_indev_drv_create()
    }
    type() {
        return constMapping.INDEV_TYPE.name(Module._lv_indev_drv_get_type(this.ptr))
    }
    setType(type) {
        Module._lv_indev_drv_set_type(this.ptr, constMapping.INDEV_TYPE.value(type))
    }
    readCb() {
        return Module._lv_indev_drv_get_read_cb(this.ptr)
    }
    setReadCb(read_cb) {
        Module._lv_indev_drv_set_read_cb(this.ptr, read_cb)
    }
    feedbackCb() {
        return Module._lv_indev_drv_get_feedback_cb(this.ptr)
    }
    setFeedbackCb(feedback_cb) {
        Module._lv_indev_drv_set_feedback_cb(this.ptr, feedback_cb)
    }
    userData() {
        return Module._lv_indev_drv_get_user_data(this.ptr)
    }
    setUserData(user_data) {
        Module._lv_indev_drv_set_user_data(this.ptr, user_data)
    }
    disp() {
        return Module._lv_indev_drv_get_disp(this.ptr)
    }
    setDisp(disp) {
        Module._lv_indev_drv_set_disp(this.ptr, disp)
    }
    readTimer() {
        return Module._lv_indev_drv_get_read_timer(this.ptr)
    }
    setReadTimer(read_timer) {
        Module._lv_indev_drv_set_read_timer(this.ptr, read_timer)
    }
    scrollLimit() {
        return Module._lv_indev_drv_get_scroll_limit(this.ptr)
    }
    setScrollLimit(scroll_limit) {
        Module._lv_indev_drv_set_scroll_limit(this.ptr, scroll_limit)
    }
    scrollThrow() {
        return Module._lv_indev_drv_get_scroll_throw(this.ptr)
    }
    setScrollThrow(scroll_throw) {
        Module._lv_indev_drv_set_scroll_throw(this.ptr, scroll_throw)
    }
    gestureMinVelocity() {
        return Module._lv_indev_drv_get_gesture_min_velocity(this.ptr)
    }
    setGestureMinVelocity(gesture_min_velocity) {
        Module._lv_indev_drv_set_gesture_min_velocity(this.ptr, gesture_min_velocity)
    }
    gestureLimit() {
        return Module._lv_indev_drv_get_gesture_limit(this.ptr)
    }
    setGestureLimit(gesture_limit) {
        Module._lv_indev_drv_set_gesture_limit(this.ptr, gesture_limit)
    }
    longPressTime() {
        return Module._lv_indev_drv_get_long_press_time(this.ptr)
    }
    setLongPressTime(long_press_time) {
        Module._lv_indev_drv_set_long_press_time(this.ptr, long_press_time)
    }
    longPressRepeatTime() {
        return Module._lv_indev_drv_get_long_press_repeat_time(this.ptr)
    }
    setLongPressRepeatTime(long_press_repeat_time) {
        Module._lv_indev_drv_set_long_press_repeat_time(this.ptr, long_press_repeat_time)
    }
    init() {
        Module._lv_indev_drv_init(this.ptr)
    }
    register() {
        return Module._lv_indev_drv_register(this.ptr)
    }
}
class IndevData extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_indev_data_create()
    }
    point() {
        return Module._lv_indev_data_get_point(this.ptr)
    }
    setPoint(point) {
        Module._lv_indev_data_set_point(this.ptr, point)
    }
    key() {
        return Module._lv_indev_data_get_key(this.ptr)
    }
    setKey(key) {
        Module._lv_indev_data_set_key(this.ptr, key)
    }
    btnId() {
        return Module._lv_indev_data_get_btn_id(this.ptr)
    }
    setBtnId(btn_id) {
        Module._lv_indev_data_set_btn_id(this.ptr, btn_id)
    }
    encDiff() {
        return Module._lv_indev_data_get_enc_diff(this.ptr)
    }
    setEncDiff(enc_diff) {
        Module._lv_indev_data_set_enc_diff(this.ptr, enc_diff)
    }
    state() {
        return constMapping.INDEV_STATE.name(Module._lv_indev_data_get_state(this.ptr))
    }
    setState(state) {
        Module._lv_indev_data_set_state(this.ptr, constMapping.INDEV_STATE.value(state))
    }
    continueReading() {
        return Module.ccall("lv_indev_data_get_continue_reading", "bool", ["number"], [this.ptr])
    }
    setContinueReading(continue_reading) {
        Module._lv_indev_data_set_continue_reading(this.ptr, continue_reading)
    }
}
// AUTO GENERATE CODE END [LVGL.JS STRUCTS] --------


const constMapping = {
// AUTO GENERATE CODE START [LVGL.JS CONST MAPPING] --------
    RES : {
        value(name) {
            return Module.ccall("lv_res_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_res_to_name", "string", ["number"], [value])
        }
    } ,
    ANIM_ENABLE : {
        value(name) {
            return Module.ccall("lv_anim_enable_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_anim_enable_to_name", "string", ["number"], [value])
        }
    } ,
    PALETTE : {
        value(name) {
            return Module.ccall("lv_palette_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_palette_to_name", "string", ["number"], [value])
        }
    } ,
    ALIGN : {
        value(name) {
            return Module.ccall("lv_align_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_align_to_name", "string", ["number"], [value])
        }
    } ,
    DIR : {
        value(name) {
            return Module.ccall("lv_dir_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_dir_to_name", "string", ["number"], [value])
        }
    } ,
    IMG_CF : {
        value(name) {
            return Module.ccall("lv_img_cf_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_img_cf_to_name", "string", ["number"], [value])
        }
    } ,
    DISP_ROT : {
        value(name) {
            return Module.ccall("lv_disp_rot_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_disp_rot_to_name", "string", ["number"], [value])
        }
    } ,
    INDEV_TYPE : {
        value(name) {
            return Module.ccall("lv_indev_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_indev_type_to_name", "string", ["number"], [value])
        }
    } ,
    INDEV_STATE : {
        value(name) {
            return Module.ccall("lv_indev_state_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_indev_state_to_name", "string", ["number"], [value])
        }
    } ,
    FONT_SUBPX : {
        value(name) {
            return Module.ccall("lv_font_subpx_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_font_subpx_to_name", "string", ["number"], [value])
        }
    } ,
    TEXT_FLAG : {
        value(name) {
            return Module.ccall("lv_text_flag_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_text_flag_to_name", "string", ["number"], [value])
        }
    } ,
    TEXT_CMD_STATE : {
        value(name) {
            return Module.ccall("lv_text_cmd_state_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_text_cmd_state_to_name", "string", ["number"], [value])
        }
    } ,
    TEXT_ALIGN : {
        value(name) {
            return Module.ccall("lv_text_align_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_text_align_to_name", "string", ["number"], [value])
        }
    } ,
    BASE_DIR : {
        value(name) {
            return Module.ccall("lv_base_dir_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_base_dir_to_name", "string", ["number"], [value])
        }
    } ,
    BLEND_MODE : {
        value(name) {
            return Module.ccall("lv_blend_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_blend_mode_to_name", "string", ["number"], [value])
        }
    } ,
    TEXT_DECOR : {
        value(name) {
            return Module.ccall("lv_text_decor_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_text_decor_to_name", "string", ["number"], [value])
        }
    } ,
    BORDER_SIDE : {
        value(name) {
            return Module.ccall("lv_border_side_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_border_side_to_name", "string", ["number"], [value])
        }
    } ,
    GRAD_DIR : {
        value(name) {
            return Module.ccall("lv_grad_dir_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_grad_dir_to_name", "string", ["number"], [value])
        }
    } ,
    STYLE_PROP : {
        value(name) {
            return Module.ccall("lv_style_prop_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_style_prop_to_name", "string", ["number"], [value])
        }
    } ,
    STATE : {
        value(name) {
            return Module.ccall("lv_state_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_state_to_name", "string", ["number"], [value])
        }
    } ,
    PART : {
        value(name) {
            return Module.ccall("lv_part_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_part_to_name", "string", ["number"], [value])
        }
    } ,
    OBJ_FLAG : {
        value(name) {
            return Module.ccall("lv_obj_flag_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_obj_flag_to_name", "string", ["number"], [value])
        }
    } ,
    OBJ_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_obj_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_obj_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    OBJ_TREE_WALK_RES : {
        value(name) {
            return Module.ccall("lv_obj_tree_walk_res_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_obj_tree_walk_res_to_name", "string", ["number"], [value])
        }
    } ,
    SCROLLBAR_MODE : {
        value(name) {
            return Module.ccall("lv_scrollbar_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_scrollbar_mode_to_name", "string", ["number"], [value])
        }
    } ,
    SCROLL_SNAP : {
        value(name) {
            return Module.ccall("lv_scroll_snap_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_scroll_snap_to_name", "string", ["number"], [value])
        }
    } ,
    _STYLE_STATE_CMP : {
        value(name) {
            return Module.ccall("lv__style_state_cmp_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv__style_state_cmp_to_name", "string", ["number"], [value])
        }
    } ,
    FS_RES : {
        value(name) {
            return Module.ccall("lv_fs_res_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_fs_res_to_name", "string", ["number"], [value])
        }
    } ,
    FS_MODE : {
        value(name) {
            return Module.ccall("lv_fs_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_fs_mode_to_name", "string", ["number"], [value])
        }
    } ,
    FS_WHENCE : {
        value(name) {
            return Module.ccall("lv_fs_whence_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_fs_whence_to_name", "string", ["number"], [value])
        }
    } ,
    IMG_SRC : {
        value(name) {
            return Module.ccall("lv_img_src_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_img_src_to_name", "string", ["number"], [value])
        }
    } ,
    DRAW_MASK_RES : {
        value(name) {
            return Module.ccall("lv_draw_mask_res_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_draw_mask_res_to_name", "string", ["number"], [value])
        }
    } ,
    DRAW_MASK_TYPE : {
        value(name) {
            return Module.ccall("lv_draw_mask_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_draw_mask_type_to_name", "string", ["number"], [value])
        }
    } ,
    DRAW_MASK_XCB : {
        value(name) {
            return Module.ccall("lv_draw_mask_xcb_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_draw_mask_xcb_to_name", "string", ["number"], [value])
        }
    } ,
    COVER_RES : {
        value(name) {
            return Module.ccall("lv_cover_res_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_cover_res_to_name", "string", ["number"], [value])
        }
    } ,
    OBJ_CLASS_EDITABLE : {
        value(name) {
            return Module.ccall("lv_obj_class_editable_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_obj_class_editable_to_name", "string", ["number"], [value])
        }
    } ,
    OBJ_CLASS_GROUP_DEF : {
        value(name) {
            return Module.ccall("lv_obj_class_group_def_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_obj_class_group_def_to_name", "string", ["number"], [value])
        }
    } ,
    EVENT_CODE : {
        value(name) {
            return Module.ccall("lv_event_code_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_event_code_to_name", "string", ["number"], [value])
        }
    } ,
    KEY : {
        value(name) {
            return Module.ccall("lv_key_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_key_to_name", "string", ["number"], [value])
        }
    } ,
    GROUP_REFOCUS_POLICY : {
        value(name) {
            return Module.ccall("lv_group_refocus_policy_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_group_refocus_policy_to_name", "string", ["number"], [value])
        }
    } ,
    SCR_LOAD_ANIM : {
        value(name) {
            return Module.ccall("lv_scr_load_anim_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_scr_load_anim_to_name", "string", ["number"], [value])
        }
    } ,
    FONT_FMT_TXT_CMAP_TYPE : {
        value(name) {
            return Module.ccall("lv_font_fmt_txt_cmap_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_font_fmt_txt_cmap_type_to_name", "string", ["number"], [value])
        }
    } ,
    FONT_FMT_TXT_BITMAP_FORMAT : {
        value(name) {
            return Module.ccall("lv_font_fmt_txt_bitmap_format_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_font_fmt_txt_bitmap_format_to_name", "string", ["number"], [value])
        }
    } ,
    ARC_MODE : {
        value(name) {
            return Module.ccall("lv_arc_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_arc_mode_to_name", "string", ["number"], [value])
        }
    } ,
    ARC_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_arc_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_arc_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    IMG_SIZE_MODE : {
        value(name) {
            return Module.ccall("lv_img_size_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_img_size_mode_to_name", "string", ["number"], [value])
        }
    } ,
    LABEL_LONG_MODE : {
        value(name) {
            return Module.ccall("lv_label_long_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_label_long_mode_to_name", "string", ["number"], [value])
        }
    } ,
    TABLE_CELL_CTRL : {
        value(name) {
            return Module.ccall("lv_table_cell_ctrl_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_table_cell_ctrl_to_name", "string", ["number"], [value])
        }
    } ,
    TABLE_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_table_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_table_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    CHECKBOX_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_checkbox_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_checkbox_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    BAR_MODE : {
        value(name) {
            return Module.ccall("lv_bar_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_bar_mode_to_name", "string", ["number"], [value])
        }
    } ,
    BAR_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_bar_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_bar_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    SLIDER_MODE : {
        value(name) {
            return Module.ccall("lv_slider_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_slider_mode_to_name", "string", ["number"], [value])
        }
    } ,
    SLIDER_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_slider_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_slider_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    BTNMATRIX_CTRL : {
        value(name) {
            return Module.ccall("lv_btnmatrix_ctrl_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_btnmatrix_ctrl_to_name", "string", ["number"], [value])
        }
    } ,
    BTNMATRIX_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_btnmatrix_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_btnmatrix_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    ROLLER_MODE : {
        value(name) {
            return Module.ccall("lv_roller_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_roller_mode_to_name", "string", ["number"], [value])
        }
    } ,
    ANIMIMG_PART : {
        value(name) {
            return Module.ccall("lv_animimg_part_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_animimg_part_to_name", "string", ["number"], [value])
        }
    } ,
    CHART_TYPE : {
        value(name) {
            return Module.ccall("lv_chart_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_chart_type_to_name", "string", ["number"], [value])
        }
    } ,
    CHART_UPDATE_MODE : {
        value(name) {
            return Module.ccall("lv_chart_update_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_chart_update_mode_to_name", "string", ["number"], [value])
        }
    } ,
    CHART_AXIS : {
        value(name) {
            return Module.ccall("lv_chart_axis_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_chart_axis_to_name", "string", ["number"], [value])
        }
    } ,
    CHART_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_chart_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_chart_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    KEYBOARD_MODE : {
        value(name) {
            return Module.ccall("lv_keyboard_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_keyboard_mode_to_name", "string", ["number"], [value])
        }
    } ,
    FLEX_ALIGN : {
        value(name) {
            return Module.ccall("lv_flex_align_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_flex_align_to_name", "string", ["number"], [value])
        }
    } ,
    FLEX_FLOW : {
        value(name) {
            return Module.ccall("lv_flex_flow_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_flex_flow_to_name", "string", ["number"], [value])
        }
    } ,
    METER_INDICATOR_TYPE : {
        value(name) {
            return Module.ccall("lv_meter_indicator_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_meter_indicator_type_to_name", "string", ["number"], [value])
        }
    } ,
    METER_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_meter_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_meter_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    COLORWHEEL_MODE : {
        value(name) {
            return Module.ccall("lv_colorwheel_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_colorwheel_mode_to_name", "string", ["number"], [value])
        }
    } ,
    LED_DRAW_PART_TYPE : {
        value(name) {
            return Module.ccall("lv_led_draw_part_type_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_led_draw_part_type_to_name", "string", ["number"], [value])
        }
    } ,
    IMGBTN_STATE : {
        value(name) {
            return Module.ccall("lv_imgbtn_state_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_imgbtn_state_to_name", "string", ["number"], [value])
        }
    } ,
    SPAN_OVERFLOW : {
        value(name) {
            return Module.ccall("lv_span_overflow_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_span_overflow_to_name", "string", ["number"], [value])
        }
    } ,
    SPAN_MODE : {
        value(name) {
            return Module.ccall("lv_span_mode_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_span_mode_to_name", "string", ["number"], [value])
        }
    } ,
    GRID_ALIGN : {
        value(name) {
            return Module.ccall("lv_grid_align_to_value", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("lv_grid_align_to_name", "string", ["number"], [value])
        }
    } ,
// AUTO GENERATE CODE END [LVGL.JS CONST MAPPING] --------
}


IndevData.prototype.x = function() {
    return Module._lv_indev_data_get_point_x(this.ptr)
}
IndevData.prototype.setX = function(x) {
    Module._lv_indev_data_set_point_x(this.ptr,x)
}
IndevData.prototype.y = function() {
    return Module._lv_indev_data_get_point_y(this.ptr)
}
IndevData.prototype.setY = function(y) {
    Module._lv_indev_data_set_point_y(this.ptr,y)
}

let SizeContent = -1

function size(val) {
    if(typeof val=='string' && val[val.length-1]=='%') {
        let _val = Math.round(val.substr(0,val.length-1))
        if(isNaN(_val)) {
            throw new Error ("invalid percent number: "+val)
        }
        val = Module._lv_coord_pct(_val)
    }
    if(val==-1) {
        val = SizeContent
    }
    return val
}

// pad for beapi.lvgl
Obj.prototype.show = function show() {
    this.clearFlag("hidden")
}
Obj.prototype.hide = function hide() {
    this.addFlag("hidden")
}
Obj.prototype.toggle = function hide() {
    if(this.isVisible()) {
        this.addFlag("hidden")
        return false
    }
    else {
        this.clearFlag("hidden")
        return true
    }
}


function requireLVGL(exports) {
    if(!exports) {
        exports = {}
    }

    SizeContent = Module._lv_coord_size_content()

    exports.EventEmitter = EventEmitter
    exports.WASMObject = WASMObject

    exports.init = function() {
        Module._lv_init()
    }

    exports.SizeContent = SizeContent
    exports.constMapping = constMapping

// AUTO GENERATE CODE START [EXPORT JS CLASS] --------
    exports.Obj = Obj
    exports.Label = Label
    exports.Arc = Arc
    exports.Bar = Bar
    exports.Btn = Btn
    exports.BtnMatrix = BtnMatrix
    exports.Canvas = Canvas
    exports.Checkbox = Checkbox
    exports.Dropdown = Dropdown
    exports.Img = Img
    exports.Line = Line
    exports.Roller = Roller
    exports.Slider = Slider
    exports.Switch = Switch
    exports.Table = Table
    exports.TextArea = TextArea
    exports.MsgBox = MsgBox
    exports.Keyboard = Keyboard
    exports.TileView = TileView
    exports.List = List
    exports.DispDrawBuf = DispDrawBuf
    exports.DispDrv = DispDrv
    exports.Disp = Disp
    exports.ObjClass = ObjClass
    exports.IndevDrv = IndevDrv
    exports.IndevData = IndevData
// AUTO GENERATE CODE END [EXPORT JS CLASS] --------

    return exports
}
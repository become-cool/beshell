declare var Module: any ;

export class WASMObject {
    constructor(public ptr = 0) {
        this.registerPointer()
    }
    public static wrap(ptr:number, clz:any=null): any {
        if(!ptr) {
            return null
        }
        if(WASMObject.pool[ptr]) {
            return WASMObject.pool[ptr]
        }
        if(!clz || !WASMObject.isPrototypeOf(clz)) {
            return null
        }
        let obj:any = {
            ptr, 
            __proto__: clz.prototype
        }
        obj.registerPointer()
        return obj
    }
    public static pool: {[key:number]:WASMObject} = {}
    protected registerPointer() {
        if(this.ptr!=0) {
            WASMObject.pool[this.ptr] = this
        }
    }
    public free() {
        if(!this.ptr) {
            return
        }
        delete WASMObject.pool[this.ptr]
        Module._lv_free(this.ptr)
        this.ptr = 0
    }
}
class EventEmitter extends WASMObject {
    _handlers: {[key:string]:Function[]} = {}
    /**
     * 
     * @param {string|string[]} event 
     * @param {function} handle 
     * @param {bool} norepeat 
     */
    on(event:string|string[], handle: Function, norepeat=false) {
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
            for(let eventName of event) {
                this.on(eventName, handle, norepeat)
            }
        }
        return
    }
    /**
     * 
     * @param {string} eventName 
     * @param {function} handle 
     */
    once(eventName:string, handle:Function, norepeat=false) {
        let wrapper =  (...args: any[]) => {
            this.off(eventName, wrapper)
            handle.apply(this, args)
        }
        (wrapper as any).__origin = handle
        this.on(eventName, wrapper, norepeat)
        return
    }
    off(eventName:string, handle:Function, all=false) {
        if(!this._handlers[eventName]) {
            return
        }
        for(let h=this._handlers[eventName].length-1; h>=0; h--) {
            let func = this.originHanlde(this._handlers[eventName][h])
            if(!handle || handle==func) {
                (this._handlers[eventName][h] as any).__origin = null
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
    originHanlde(handle:Function) {
        for(var h=handle; (h as any).__origin; h=(h as any).__origin){}
        return h
    }
    isListening(event:string, handle:Function) {
        if(!this._handlers[event])
            return false
        for(let cb of this._handlers[event]) {
            if( this.originHanlde(cb)==handle )
                return true
        }
        return false
    }
    emit(eventName:string, ...args: any[]) {
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
                if((cb as any).__origin) {
                    delete (cb as any).__origin
                }
            })
            this._handlers[eventName].splice(0)
            delete this._handlers[eventName]
        }
    }
}


// AUTO GENERATE CODE START [LVGL.JS WIDGETS] --------
export class Obj extends EventEmitter {
    public id = ""
    public name = ""
    constructor(parent: Obj|null, ptr=0) {
        super()
        this.on("#EVENT.ADD#", (name:string)=>{
            try{ Module._lv_obj_enable_event(this.ptr,constMapping.EVENT_CODE.value(name)) }catch(e){}
        })
        this.on("#EVENT.CLEAR#", (name:string)=>{
            try{ Module._lv_obj_disable_event(this.ptr,constMapping.EVENT_CODE.value(name)) }catch(e){}
        })
        if(ptr) {
            this.ptr = ptr
            this.registerPointer()
        }
        else {
            this._createWidget(parent)
        }
    }
    
    public setCoordX(val:number) {
        Module._lv_obj_set_coord_x(this.ptr, val)
    }
    public setCoordY(val:number) {
        Module._lv_obj_set_coord_y(this.ptr, val)
    }
    public setCoords(x:number,y:number) {
        Module._lv_obj_set_coords_redefine(this.ptr, x, y)
    }
    public coordX() {
        return Module._lv_obj_get_coord_x(this.ptr)
    }
    public coordY() {
        return Module._lv_obj_get_coord_y(this.ptr)
    }
    public coords() {
        return [Module._lv_obj_get_coord_x(this.ptr), Module._lv_obj_get_coord_y(this.ptr)]
    }

    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_obj_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public addFlag(f:string): void {
        Module._lv_obj_add_flag(this.ptr, constMapping.OBJ_FLAG.value(f))
    }
    public clearFlag(f:string): void {
        Module._lv_obj_clear_flag(this.ptr, constMapping.OBJ_FLAG.value(f))
    }
    public addState(state:string): void {
        Module._lv_obj_add_state(this.ptr, constMapping.STATE.value(state))
    }
    public clearState(state:string): void {
        Module._lv_obj_clear_state(this.ptr, constMapping.STATE.value(state))
    }
    public hasFlag(f:string): boolean {
        return Module.ccall("lv_obj_has_flag", "bool", ["number", "number"], [this.ptr, constMapping.OBJ_FLAG.value(f)])
    }
    public hasFlagAny(f:string): boolean {
        return Module.ccall("lv_obj_has_flag_any", "bool", ["number", "number"], [this.ptr, constMapping.OBJ_FLAG.value(f)])
    }
    public state(): any {
        return constMapping.STATE.name(Module._lv_obj_get_state(this.ptr))
    }
    public hasState(state:string): boolean {
        return Module.ccall("lv_obj_has_state", "bool", ["number", "number"], [this.ptr, constMapping.STATE.value(state)])
    }
    public allocateSpecAttr(): void {
        Module._lv_obj_allocate_spec_attr(this.ptr)
    }
    public isValid(): boolean {
        return Module.ccall("lv_obj_is_valid", "bool", ["number"], [this.ptr])
    }
    public dpx(n:any): any {
        return Module._lv_obj_dpx(this.ptr, n)
    }
    public isEditable(): boolean {
        return Module.ccall("lv_obj_is_editable", "bool", ["number"], [this.ptr])
    }
    public setPos(x:any, y:any): void {
        Module._lv_obj_set_pos(this.ptr, x, y)
    }
    public setX(x:any): void {
        Module._lv_obj_set_x(this.ptr, x)
    }
    public setY(y:any): void {
        Module._lv_obj_set_y(this.ptr, y)
    }
    public setSize(w:any, h:any): void {
        Module._lv_obj_set_size(this.ptr, w, h)
    }
    public refrSize(): boolean {
        return Module.ccall("lv_obj_refr_size", "bool", ["number"], [this.ptr])
    }
    public setWidth(width:string|number) {
        Module._lv_obj_set_width(this.ptr, size(width))
    }
    public setHeight(height:string|number) {
        Module._lv_obj_set_height(this.ptr, size(height))
    }
    public setContentWidth(w:any): void {
        Module._lv_obj_set_content_width(this.ptr, w)
    }
    public setContentHeight(h:any): void {
        Module._lv_obj_set_content_height(this.ptr, h)
    }
    public setLayout(layout:any): void {
        Module._lv_obj_set_layout(this.ptr, layout)
    }
    public isLayoutPositioned(): boolean {
        return Module.ccall("lv_obj_is_layout_positioned", "bool", ["number"], [this.ptr])
    }
    public markLayoutAsDirty(): void {
        Module._lv_obj_mark_layout_as_dirty(this.ptr)
    }
    public updateLayout(): void {
        Module._lv_obj_update_layout(this.ptr)
    }
    public setAlign(align:string): void {
        Module._lv_obj_set_align(this.ptr, constMapping.ALIGN.value(align))
    }
    public align(align:string, x_ofs:any, y_ofs:any): void {
        Module._lv_obj_align(this.ptr, constMapping.ALIGN.value(align), x_ofs, y_ofs)
    }
    public alignTo(base:Obj, align:string, x_ofs:any, y_ofs:any): void {
        Module._lv_obj_align_to(this.ptr, base.ptr, constMapping.ALIGN.value(align), x_ofs, y_ofs)
    }
    public center(): void {
        Module._lv_obj_center(this.ptr)
    }
    public x(): any {
        return Module._lv_obj_get_x(this.ptr)
    }
    public y(): any {
        return Module._lv_obj_get_y(this.ptr)
    }
    public xAligned(): any {
        return Module._lv_obj_get_x_aligned(this.ptr)
    }
    public yAligned(): any {
        return Module._lv_obj_get_y_aligned(this.ptr)
    }
    public width(): any {
        return Module._lv_obj_get_width(this.ptr)
    }
    public height(): any {
        return Module._lv_obj_get_height(this.ptr)
    }
    public contentWidth(): any {
        return Module._lv_obj_get_content_width(this.ptr)
    }
    public contentHeight(): any {
        return Module._lv_obj_get_content_height(this.ptr)
    }
    public contentCoords(area:any): void {
        Module._lv_obj_get_content_coords(this.ptr, area)
    }
    public selfWidth(): any {
        return Module._lv_obj_get_self_width(this.ptr)
    }
    public selfHeight(): any {
        return Module._lv_obj_get_self_height(this.ptr)
    }
    public refreshSelfSize(): boolean {
        return Module.ccall("lv_obj_refresh_self_size", "bool", ["number"], [this.ptr])
    }
    public refrPos(): void {
        Module._lv_obj_refr_pos(this.ptr)
    }
    public moveTo(x:any, y:any): void {
        Module._lv_obj_move_to(this.ptr, x, y)
    }
    public moveChildrenBy(x_diff:any, y_diff:any, ignore_floating:any): void {
        Module._lv_obj_move_children_by(this.ptr, x_diff, y_diff, ignore_floating)
    }
    public invalidateArea(area:any): void {
        Module._lv_obj_invalidate_area(this.ptr, area)
    }
    public invalidate(): void {
        Module._lv_obj_invalidate(this.ptr)
    }
    public areaIsVisible(area:any): boolean {
        return Module.ccall("lv_obj_area_is_visible", "bool", ["number", "number"], [this.ptr, area])
    }
    public isVisible(): boolean {
        return Module.ccall("lv_obj_is_visible", "bool", ["number"], [this.ptr])
    }
    public setExtClickArea(size:any): void {
        Module._lv_obj_set_ext_click_area(this.ptr, size)
    }
    public clickArea(area:any): void {
        Module._lv_obj_get_click_area(this.ptr, area)
    }
    public hitTest(point:Point): boolean {
        return Module.ccall("lv_obj_hit_test", "bool", ["number", "pointer"], [this.ptr, point.ptr])
    }
    public setScrollbarMode(mode:string): void {
        Module._lv_obj_set_scrollbar_mode(this.ptr, constMapping.SCROLLBAR_MODE.value(mode))
    }
    public setScrollDir(dir:string): void {
        Module._lv_obj_set_scroll_dir(this.ptr, constMapping.DIR.value(dir))
    }
    public setScrollSnapX(align:string): void {
        Module._lv_obj_set_scroll_snap_x(this.ptr, constMapping.SCROLL_SNAP.value(align))
    }
    public setScrollSnapY(align:string): void {
        Module._lv_obj_set_scroll_snap_y(this.ptr, constMapping.SCROLL_SNAP.value(align))
    }
    public scrollbarMode(): any {
        return constMapping.SCROLLBAR_MODE.name(Module._lv_obj_get_scrollbar_mode(this.ptr))
    }
    public scrollDir(): any {
        return constMapping.DIR.name(Module._lv_obj_get_scroll_dir(this.ptr))
    }
    public scrollSnapX(): any {
        return constMapping.SCROLL_SNAP.name(Module._lv_obj_get_scroll_snap_x(this.ptr))
    }
    public scrollSnapY(): any {
        return constMapping.SCROLL_SNAP.name(Module._lv_obj_get_scroll_snap_y(this.ptr))
    }
    public scrollX(): any {
        return Module._lv_obj_get_scroll_x(this.ptr)
    }
    public scrollY(): any {
        return Module._lv_obj_get_scroll_y(this.ptr)
    }
    public scrollTop(): any {
        return Module._lv_obj_get_scroll_top(this.ptr)
    }
    public scrollBottom(): any {
        return Module._lv_obj_get_scroll_bottom(this.ptr)
    }
    public scrollLeft(): any {
        return Module._lv_obj_get_scroll_left(this.ptr)
    }
    public scrollRight(): any {
        return Module._lv_obj_get_scroll_right(this.ptr)
    }
    public scrollEnd(end:Point): void {
        Module._lv_obj_get_scroll_end(this.ptr, end.ptr)
    }
    public scrollBy(x:any, y:any, anim_en:string): void {
        Module._lv_obj_scroll_by(this.ptr, x, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollTo(x:any, y:any, anim_en:string): void {
        Module._lv_obj_scroll_to(this.ptr, x, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollToX(x:any, anim_en:string): void {
        Module._lv_obj_scroll_to_x(this.ptr, x, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollToY(y:any, anim_en:string): void {
        Module._lv_obj_scroll_to_y(this.ptr, y, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollToView(anim_en:string): void {
        Module._lv_obj_scroll_to_view(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollToViewRecursive(anim_en:string): void {
        Module._lv_obj_scroll_to_view_recursive(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public isScrolling(): boolean {
        return Module.ccall("lv_obj_is_scrolling", "bool", ["number"], [this.ptr])
    }
    public updateSnap(anim_en:string): void {
        Module._lv_obj_update_snap(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public scrollbarArea(hor:any, ver:any): void {
        Module._lv_obj_get_scrollbar_area(this.ptr, hor, ver)
    }
    public scrollbarInvalidate(): void {
        Module._lv_obj_scrollbar_invalidate(this.ptr)
    }
    public readjustScroll(anim_en:string): void {
        Module._lv_obj_readjust_scroll(this.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public addStyle(style:Style, selector:any): void {
        Module._lv_obj_add_style(this.ptr, style.ptr, selector)
    }
    public removeStyle(style:Style, selector:any): void {
        Module._lv_obj_remove_style(this.ptr, style.ptr, selector)
    }
    public removeStyleAll(): void {
        Module._lv_obj_remove_style_all(this.ptr)
    }
    public fadeIn(time:any, delay:any): void {
        Module._lv_obj_fade_in(this.ptr, time, delay)
    }
    public fadeOut(time:any, delay:any): void {
        Module._lv_obj_fade_out(this.ptr, time, delay)
    }
    public del(): void {
        Module._lv_obj_del(this.ptr)
    }
    public clean(): void {
        Module._lv_obj_clean(this.ptr)
    }
    public delAsync(): void {
        Module._lv_obj_del_async(this.ptr)
    }
    public setParent(parent:Obj): void {
        Module._lv_obj_set_parent(this.ptr, parent.ptr)
    }
    public moveToIndex(index:any): void {
        Module._lv_obj_move_to_index(this.ptr, index)
    }
    public screen(): Obj {
        return WASMObject.wrap(Module._lv_obj_get_screen(this.ptr), Obj)
    }
    public disp(): Disp {
        return WASMObject.wrap(Module._lv_obj_get_disp(this.ptr), Disp)
    }
    public parent(): Obj {
        return WASMObject.wrap(Module._lv_obj_get_parent(this.ptr), Obj)
    }
    public child(id:any): Obj {
        return WASMObject.wrap(Module._lv_obj_get_child(this.ptr, id), Obj)
    }
    public childCnt(): any {
        return Module._lv_obj_get_child_cnt(this.ptr)
    }
    public index(): any {
        return Module._lv_obj_get_index(this.ptr)
    }
    public setFlexFlow(flow:string): void {
        Module._lv_obj_set_flex_flow(this.ptr, constMapping.FLEX_FLOW.value(flow))
    }
    public setFlexAlign(main_place:string, cross_place:string, track_cross_place:string): void {
        Module._lv_obj_set_flex_align(this.ptr, constMapping.FLEX_ALIGN.value(main_place), constMapping.FLEX_ALIGN.value(cross_place), constMapping.FLEX_ALIGN.value(track_cross_place))
    }
    public setFlexGrow(grow:any): void {
        Module._lv_obj_set_flex_grow(this.ptr, grow)
    }
    public setTile(tile_obj:Obj, anim_en:string): void {
        Module._lv_obj_set_tile(this.ptr, tile_obj.ptr, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public setTileId(col_id:any, row_id:any, anim_en:string): void {
        Module._lv_obj_set_tile_id(this.ptr, col_id, row_id, constMapping.ANIM_ENABLE.value(anim_en))
    }
    public moveForeground(): void {
        Module._lv_obj_move_foreground(this.ptr)
    }
    public moveBackground(): void {
        Module._lv_obj_move_background(this.ptr)
    }
    public childId(): any {
        return Module._lv_obj_get_child_id(this.ptr)
    }
}
export class Label extends Obj {
    public font(selector=0) {
        return Module.ccall("lv_label_get_font", "string", ["number","number"], [this.ptr, selector])
    }
    public setFont(font: string, selector=0) {
        return Module.ccall("lv_label_set_font", "bool", ["number","string","number"], [this.ptr, font, selector])
    }

    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_label_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setText(text:string): void {
        Module.ccall("lv_label_set_text", "number", ["number", "string"], [this.ptr, text])
    }
    public setTextStatic(text:string): void {
        Module.ccall("lv_label_set_text_static", "number", ["number", "string"], [this.ptr, text])
    }
    public setLongMode(long_mode:string): void {
        Module._lv_label_set_long_mode(this.ptr, constMapping.LABEL_LONG_MODE.value(long_mode))
    }
    public setRecolor(en:any): void {
        Module._lv_label_set_recolor(this.ptr, en)
    }
    public setTextSelStart(index:any): void {
        Module._lv_label_set_text_sel_start(this.ptr, index)
    }
    public setTextSelEnd(index:any): void {
        Module._lv_label_set_text_sel_end(this.ptr, index)
    }
    public text(): string {
        return Module.ccall("lv_label_get_text", "string", ["number"], [this.ptr])
    }
    public longMode(): any {
        return constMapping.LABEL_LONG_MODE.name(Module._lv_label_get_long_mode(this.ptr))
    }
    public recolor(): boolean {
        return Module.ccall("lv_label_get_recolor", "bool", ["number"], [this.ptr])
    }
    public letterPos(char_id:any, pos:Point): void {
        Module._lv_label_get_letter_pos(this.ptr, char_id, pos.ptr)
    }
    public letterOn(pos_in:Point): any {
        return Module._lv_label_get_letter_on(this.ptr, pos_in.ptr)
    }
    public isCharUnderPos(pos:Point): boolean {
        return Module.ccall("lv_label_is_char_under_pos", "bool", ["number", "pointer"], [this.ptr, pos.ptr])
    }
    public textSelectionStart(): any {
        return Module._lv_label_get_text_selection_start(this.ptr)
    }
    public textSelectionEnd(): any {
        return Module._lv_label_get_text_selection_end(this.ptr)
    }
    public insText(pos:any, txt:string): void {
        Module.ccall("lv_label_ins_text", "number", ["number", "number", "string"], [this.ptr, pos, txt])
    }
    public cutText(pos:any, cnt:any): void {
        Module._lv_label_cut_text(this.ptr, pos, cnt)
    }
}
export class Arc extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_arc_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setStartAngle(start:any): void {
        Module._lv_arc_set_start_angle(this.ptr, start)
    }
    public setEndAngle(end:any): void {
        Module._lv_arc_set_end_angle(this.ptr, end)
    }
    public setAngles(start:any, end:any): void {
        Module._lv_arc_set_angles(this.ptr, start, end)
    }
    public setBgStartAngle(start:any): void {
        Module._lv_arc_set_bg_start_angle(this.ptr, start)
    }
    public setBgEndAngle(end:any): void {
        Module._lv_arc_set_bg_end_angle(this.ptr, end)
    }
    public setBgAngles(start:any, end:any): void {
        Module._lv_arc_set_bg_angles(this.ptr, start, end)
    }
    public setRotation(rotation:any): void {
        Module._lv_arc_set_rotation(this.ptr, rotation)
    }
    public setMode(type:string): void {
        Module._lv_arc_set_mode(this.ptr, constMapping.ARC_MODE.value(type))
    }
    public setValue(value:any): void {
        Module._lv_arc_set_value(this.ptr, value)
    }
    public setRange(min:any, max:any): void {
        Module._lv_arc_set_range(this.ptr, min, max)
    }
    public setChangeRate(rate:any): void {
        Module._lv_arc_set_change_rate(this.ptr, rate)
    }
    public angleStart(): any {
        return Module._lv_arc_get_angle_start(this.ptr)
    }
    public angleEnd(): any {
        return Module._lv_arc_get_angle_end(this.ptr)
    }
    public bgAngleStart(): any {
        return Module._lv_arc_get_bg_angle_start(this.ptr)
    }
    public bgAngleEnd(): any {
        return Module._lv_arc_get_bg_angle_end(this.ptr)
    }
    public value(): any {
        return Module._lv_arc_get_value(this.ptr)
    }
    public minValue(): any {
        return Module._lv_arc_get_min_value(this.ptr)
    }
    public maxValue(): any {
        return Module._lv_arc_get_max_value(this.ptr)
    }
    public mode(): any {
        return constMapping.ARC_MODE.name(Module._lv_arc_get_mode(this.ptr))
    }
}
export class Bar extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_bar_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setValue(value:any, anim:string): void {
        Module._lv_bar_set_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    public setStartValue(start_value:any, anim:string): void {
        Module._lv_bar_set_start_value(this.ptr, start_value, constMapping.ANIM_ENABLE.value(anim))
    }
    public setRange(min:any, max:any): void {
        Module._lv_bar_set_range(this.ptr, min, max)
    }
    public setMode(mode:string): void {
        Module._lv_bar_set_mode(this.ptr, constMapping.BAR_MODE.value(mode))
    }
    public value(): any {
        return Module._lv_bar_get_value(this.ptr)
    }
    public startValue(): any {
        return Module._lv_bar_get_start_value(this.ptr)
    }
    public minValue(): any {
        return Module._lv_bar_get_min_value(this.ptr)
    }
    public maxValue(): any {
        return Module._lv_bar_get_max_value(this.ptr)
    }
    public mode(): any {
        return constMapping.BAR_MODE.name(Module._lv_bar_get_mode(this.ptr))
    }
}
export class Btn extends Obj {
    public label?: Label
    constructor(parent: Obj|null, ptr=0) {
        super(parent,ptr)
        if(ptr==0) {
            this.label = new Label(this)
            this.label.setHeight(-1)
            this.label.setWidth("100%")
            this.label.setStyle("text-align","center")
            this.label.align("center",0,0)
        }
    }
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_btn_create(parent?parent.ptr:null)
        this.setWidth(80)
        this.setHeight(20)
        this.registerPointer()
    }
    text() {
        return this.label?.text()
    }
    setText(text: string) {
        this.label?.setText(text)
    }
    font() {
        return this.label?.font()
    }
    setFont(font: string) {
        this.label?.setFont(font)
    }
    
}
export class BtnMatrix extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_btnmatrix_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public popovers(): boolean {
        return Module.ccall("lv_btnmatrix_get_popovers", "bool", ["number"], [this.ptr])
    }
    public setSelectedBtn(btn_id:any): void {
        Module._lv_btnmatrix_set_selected_btn(this.ptr, btn_id)
    }
    public setBtnCtrl(btn_id:any, ctrl:string): void {
        Module._lv_btnmatrix_set_btn_ctrl(this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    public clearBtnCtrl(btn_id:any, ctrl:string): void {
        Module._lv_btnmatrix_clear_btn_ctrl(this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    public setBtnCtrlAll(ctrl:string): void {
        Module._lv_btnmatrix_set_btn_ctrl_all(this.ptr, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    public clearBtnCtrlAll(ctrl:string): void {
        Module._lv_btnmatrix_clear_btn_ctrl_all(this.ptr, constMapping.BTNMATRIX_CTRL.value(ctrl))
    }
    public setBtnWidth(btn_id:any, width:any): void {
        Module._lv_btnmatrix_set_btn_width(this.ptr, btn_id, width)
    }
    public setOneChecked(en:any): void {
        Module._lv_btnmatrix_set_one_checked(this.ptr, en)
    }
    public selectedBtn(): any {
        return Module._lv_btnmatrix_get_selected_btn(this.ptr)
    }
    public btnText(btn_id:any): string {
        return Module.ccall("lv_btnmatrix_get_btn_text", "string", ["number", "number"], [this.ptr, btn_id])
    }
    public hasBtnCtrl(btn_id:any, ctrl:string): boolean {
        return Module.ccall("lv_btnmatrix_has_btn_ctrl", "bool", ["number", "number", "number"], [this.ptr, btn_id, constMapping.BTNMATRIX_CTRL.value(ctrl)])
    }
    public oneChecked(): boolean {
        return Module.ccall("lv_btnmatrix_get_one_checked", "bool", ["number"], [this.ptr])
    }
}
export class Canvas extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_canvas_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setPxColor(x:any, y:any, c:any): void {
        Module._lv_canvas_set_px_color(this.ptr, x, y, c)
    }
    public setPx(x:any, y:any, c:any): void {
        Module._lv_canvas_set_px(this.ptr, x, y, c)
    }
    public setPxOpa(x:any, y:any, opa:any): void {
        Module._lv_canvas_set_px_opa(this.ptr, x, y, opa)
    }
    public setPalette(id:any, c:any): void {
        Module._lv_canvas_set_palette(this.ptr, id, c)
    }
    public px(x:any, y:any): any {
        return Module._lv_canvas_get_px(this.ptr, x, y)
    }
    public blurHor(area:any, r:any): void {
        Module._lv_canvas_blur_hor(this.ptr, area, r)
    }
    public blurVer(area:any, r:any): void {
        Module._lv_canvas_blur_ver(this.ptr, area, r)
    }
    public fillBg(color:any, opa:any): void {
        Module._lv_canvas_fill_bg(this.ptr, color, opa)
    }
    public drawRect(x:any, y:any, w:any, h:any, draw_dsc:any): void {
        Module._lv_canvas_draw_rect(this.ptr, x, y, w, h, draw_dsc)
    }
    public drawArc(x:any, y:any, r:any, start_angle:any, end_angle:any, draw_dsc:any): void {
        Module._lv_canvas_draw_arc(this.ptr, x, y, r, start_angle, end_angle, draw_dsc)
    }
}
export class Checkbox extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_checkbox_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setText(txt:string): void {
        Module.ccall("lv_checkbox_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    public setTextStatic(txt:string): void {
        Module.ccall("lv_checkbox_set_text_static", "number", ["number", "string"], [this.ptr, txt])
    }
    public text(): string {
        return Module.ccall("lv_checkbox_get_text", "string", ["number"], [this.ptr])
    }
}
export class Dropdown extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_dropdown_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setText(txt:string): void {
        Module.ccall("lv_dropdown_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    public setOptions(options:string): void {
        Module.ccall("lv_dropdown_set_options", "number", ["number", "string"], [this.ptr, options])
    }
    public setOptionsStatic(options:string): void {
        Module.ccall("lv_dropdown_set_options_static", "number", ["number", "string"], [this.ptr, options])
    }
    public addOption(option:string, pos:any): void {
        Module.ccall("lv_dropdown_add_option", "number", ["number", "string", "number"], [this.ptr, option, pos])
    }
    public clearOptions(): void {
        Module._lv_dropdown_clear_options(this.ptr)
    }
    public setSelected(sel_opt:any): void {
        Module._lv_dropdown_set_selected(this.ptr, sel_opt)
    }
    public setDir(dir:string): void {
        Module._lv_dropdown_set_dir(this.ptr, constMapping.DIR.value(dir))
    }
    public setSelectedHighlight(en:any): void {
        Module._lv_dropdown_set_selected_highlight(this.ptr, en)
    }
    public text(): string {
        return Module.ccall("lv_dropdown_get_text", "string", ["number"], [this.ptr])
    }
    public options(): string {
        return Module.ccall("lv_dropdown_get_options", "string", ["number"], [this.ptr])
    }
    public selected(): any {
        return Module._lv_dropdown_get_selected(this.ptr)
    }
    public optionCnt(): any {
        return Module._lv_dropdown_get_option_cnt(this.ptr)
    }
    public selectedStr(buf:string, buf_size:any): void {
        Module.ccall("lv_dropdown_get_selected_str", "number", ["number", "string", "number"], [this.ptr, buf, buf_size])
    }
    public symbol(): string {
        return Module.ccall("lv_dropdown_get_symbol", "string", ["number"], [this.ptr])
    }
    public selectedHighlight(): boolean {
        return Module.ccall("lv_dropdown_get_selected_highlight", "bool", ["number"], [this.ptr])
    }
    public dir(): any {
        return constMapping.DIR.name(Module._lv_dropdown_get_dir(this.ptr))
    }
    public open(): void {
        Module._lv_dropdown_open(this.ptr)
    }
    public close(): void {
        Module._lv_dropdown_close(this.ptr)
    }
}
export class Img extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_img_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setOffsetX(x:any): void {
        Module._lv_img_set_offset_x(this.ptr, x)
    }
    public setOffsetY(y:any): void {
        Module._lv_img_set_offset_y(this.ptr, y)
    }
    public setAngle(angle:any): void {
        Module._lv_img_set_angle(this.ptr, angle)
    }
    public setPivot(x:any, y:any): void {
        Module._lv_img_set_pivot(this.ptr, x, y)
    }
    public setZoom(zoom:any): void {
        Module._lv_img_set_zoom(this.ptr, zoom)
    }
    public setAntialias(antialias:any): void {
        Module._lv_img_set_antialias(this.ptr, antialias)
    }
    public setSizeMode(mode:string): void {
        Module._lv_img_set_size_mode(this.ptr, constMapping.IMG_SIZE_MODE.value(mode))
    }
    public offsetX(): any {
        return Module._lv_img_get_offset_x(this.ptr)
    }
    public offsetY(): any {
        return Module._lv_img_get_offset_y(this.ptr)
    }
    public angle(): any {
        return Module._lv_img_get_angle(this.ptr)
    }
    public pivot(pivot:Point): void {
        Module._lv_img_get_pivot(this.ptr, pivot.ptr)
    }
    public zoom(): any {
        return Module._lv_img_get_zoom(this.ptr)
    }
    public antialias(): boolean {
        return Module.ccall("lv_img_get_antialias", "bool", ["number"], [this.ptr])
    }
    public sizeMode(): any {
        return constMapping.IMG_SIZE_MODE.name(Module._lv_img_get_size_mode(this.ptr))
    }
}
export class Line extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_line_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setYInvert(en:any): void {
        Module._lv_line_set_y_invert(this.ptr, en)
    }
    public yInvert(): boolean {
        return Module.ccall("lv_line_get_y_invert", "bool", ["number"], [this.ptr])
    }
}
export class Roller extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_roller_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setOptions(options:string, mode:string): void {
        Module.ccall("lv_roller_set_options", "number", ["number", "string", "number"], [this.ptr, options, constMapping.ROLLER_MODE.value(mode)])
    }
    public setSelected(sel_opt:any, anim:string): void {
        Module._lv_roller_set_selected(this.ptr, sel_opt, constMapping.ANIM_ENABLE.value(anim))
    }
    public setVisibleRowCount(row_cnt:any): void {
        Module._lv_roller_set_visible_row_count(this.ptr, row_cnt)
    }
    public selected(): any {
        return Module._lv_roller_get_selected(this.ptr)
    }
    public selectedStr(buf:string, buf_size:any): void {
        Module.ccall("lv_roller_get_selected_str", "number", ["number", "string", "number"], [this.ptr, buf, buf_size])
    }
    public options(): string {
        return Module.ccall("lv_roller_get_options", "string", ["number"], [this.ptr])
    }
    public optionCnt(): any {
        return Module._lv_roller_get_option_cnt(this.ptr)
    }
}
export class Slider extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_slider_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setValue(value:any, anim:string): void {
        Module._lv_slider_set_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    public setLeftValue(value:any, anim:string): void {
        Module._lv_slider_set_left_value(this.ptr, value, constMapping.ANIM_ENABLE.value(anim))
    }
    public setRange(min:any, max:any): void {
        Module._lv_slider_set_range(this.ptr, min, max)
    }
    public setMode(mode:string): void {
        Module._lv_slider_set_mode(this.ptr, constMapping.SLIDER_MODE.value(mode))
    }
    public value(): any {
        return Module._lv_slider_get_value(this.ptr)
    }
    public leftValue(): any {
        return Module._lv_slider_get_left_value(this.ptr)
    }
    public minValue(): any {
        return Module._lv_slider_get_min_value(this.ptr)
    }
    public maxValue(): any {
        return Module._lv_slider_get_max_value(this.ptr)
    }
    public isDragged(): boolean {
        return Module.ccall("lv_slider_is_dragged", "bool", ["number"], [this.ptr])
    }
    public mode(): any {
        return constMapping.SLIDER_MODE.name(Module._lv_slider_get_mode(this.ptr))
    }
}
export class Switch extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_switch_create(parent?parent.ptr:null)
        this.registerPointer()
    }
}
export class Table extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_table_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setCellValue(row:any, col:any, txt:string): void {
        Module.ccall("lv_table_set_cell_value", "number", ["number", "number", "number", "string"], [this.ptr, row, col, txt])
    }
    public setRowCnt(row_cnt:any): void {
        Module._lv_table_set_row_cnt(this.ptr, row_cnt)
    }
    public setColCnt(col_cnt:any): void {
        Module._lv_table_set_col_cnt(this.ptr, col_cnt)
    }
    public setColWidth(col_id:any, w:any): void {
        Module._lv_table_set_col_width(this.ptr, col_id, w)
    }
    public addCellCtrl(row:any, col:any, ctrl:string): void {
        Module._lv_table_add_cell_ctrl(this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl))
    }
    public clearCellCtrl(row:any, col:any, ctrl:string): void {
        Module._lv_table_clear_cell_ctrl(this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl))
    }
    public cellValue(row:any, col:any): string {
        return Module.ccall("lv_table_get_cell_value", "string", ["number", "number", "number"], [this.ptr, row, col])
    }
    public rowCnt(): any {
        return Module._lv_table_get_row_cnt(this.ptr)
    }
    public colCnt(): any {
        return Module._lv_table_get_col_cnt(this.ptr)
    }
    public colWidth(col:any): any {
        return Module._lv_table_get_col_width(this.ptr, col)
    }
    public hasCellCtrl(row:any, col:any, ctrl:string): boolean {
        return Module.ccall("lv_table_has_cell_ctrl", "bool", ["number", "number", "number", "number"], [this.ptr, row, col, constMapping.TABLE_CELL_CTRL.value(ctrl)])
    }
}
export class TextArea extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_textarea_create(parent?parent.ptr:null)
        this.registerPointer()
        this.on("clicked",()=>{
            this.disp().popupSharedKeyboard(this)
        })
    }
    public addText(txt:string): void {
        Module.ccall("lv_textarea_add_text", "number", ["number", "string"], [this.ptr, txt])
    }
    public delChar(): void {
        Module._lv_textarea_del_char(this.ptr)
    }
    public delCharForward(): void {
        Module._lv_textarea_del_char_forward(this.ptr)
    }
    public setText(txt:string): void {
        Module.ccall("lv_textarea_set_text", "number", ["number", "string"], [this.ptr, txt])
    }
    public setPlaceholderText(txt:string): void {
        Module.ccall("lv_textarea_set_placeholder_text", "number", ["number", "string"], [this.ptr, txt])
    }
    public setCursorPos(pos:any): void {
        Module._lv_textarea_set_cursor_pos(this.ptr, pos)
    }
    public setCursorClickPos(en:any): void {
        Module._lv_textarea_set_cursor_click_pos(this.ptr, en)
    }
    public setPasswordMode(en:any): void {
        Module._lv_textarea_set_password_mode(this.ptr, en)
    }
    public setOneLine(en:any): void {
        Module._lv_textarea_set_one_line(this.ptr, en)
    }
    public setAcceptedChars(list:string): void {
        Module.ccall("lv_textarea_set_accepted_chars", "number", ["number", "string"], [this.ptr, list])
    }
    public setMaxLength(num:any): void {
        Module._lv_textarea_set_max_length(this.ptr, num)
    }
    public setInsertReplace(txt:string): void {
        Module.ccall("lv_textarea_set_insert_replace", "number", ["number", "string"], [this.ptr, txt])
    }
    public setTextSelection(en:any): void {
        Module._lv_textarea_set_text_selection(this.ptr, en)
    }
    public setPasswordShowTime(time:any): void {
        Module._lv_textarea_set_password_show_time(this.ptr, time)
    }
    public setAlign(align:string): void {
        Module._lv_textarea_set_align(this.ptr, constMapping.TEXT_ALIGN.value(align))
    }
    public text(): string {
        return Module.ccall("lv_textarea_get_text", "string", ["number"], [this.ptr])
    }
    public placeholderText(): string {
        return Module.ccall("lv_textarea_get_placeholder_text", "string", ["number"], [this.ptr])
    }
    public label(): Obj {
        return WASMObject.wrap(Module._lv_textarea_get_label(this.ptr), Obj)
    }
    public cursorPos(): any {
        return Module._lv_textarea_get_cursor_pos(this.ptr)
    }
    public cursorClickPos(): boolean {
        return Module.ccall("lv_textarea_get_cursor_click_pos", "bool", ["number"], [this.ptr])
    }
    public passwordMode(): boolean {
        return Module.ccall("lv_textarea_get_password_mode", "bool", ["number"], [this.ptr])
    }
    public oneLine(): boolean {
        return Module.ccall("lv_textarea_get_one_line", "bool", ["number"], [this.ptr])
    }
    public acceptedChars(): string {
        return Module.ccall("lv_textarea_get_accepted_chars", "string", ["number"], [this.ptr])
    }
    public maxLength(): any {
        return Module._lv_textarea_get_max_length(this.ptr)
    }
    public textIsSelected(): boolean {
        return Module.ccall("lv_textarea_text_is_selected", "bool", ["number"], [this.ptr])
    }
    public textSelection(): boolean {
        return Module.ccall("lv_textarea_get_text_selection", "bool", ["number"], [this.ptr])
    }
    public passwordShowTime(): any {
        return Module._lv_textarea_get_password_show_time(this.ptr)
    }
    public clearSelection(): void {
        Module._lv_textarea_clear_selection(this.ptr)
    }
    public cursorRight(): void {
        Module._lv_textarea_cursor_right(this.ptr)
    }
    public cursorLeft(): void {
        Module._lv_textarea_cursor_left(this.ptr)
    }
    public cursorDown(): void {
        Module._lv_textarea_cursor_down(this.ptr)
    }
    public cursorUp(): void {
        Module._lv_textarea_cursor_up(this.ptr)
    }
}
export class MsgBox extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_msgbox_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public title(): Label {
        return WASMObject.wrap(Module._lv_msgbox_get_title(this.ptr), Label)
    }
    public closeBtn(): Btn {
        return WASMObject.wrap(Module._lv_msgbox_get_close_btn(this.ptr), Btn)
    }
    public text(): Label {
        return WASMObject.wrap(Module._lv_msgbox_get_text(this.ptr), Label)
    }
    public content(): Obj {
        return WASMObject.wrap(Module._lv_msgbox_get_content(this.ptr), Obj)
    }
    public btns(): Obj {
        return WASMObject.wrap(Module._lv_msgbox_get_btns(this.ptr), Obj)
    }
    public activeBtn(): any {
        return Module._lv_msgbox_get_active_btn(this.ptr)
    }
    public activeBtnText(): string {
        return Module.ccall("lv_msgbox_get_active_btn_text", "string", ["number"], [this.ptr])
    }
    public close(): void {
        Module._lv_msgbox_close(this.ptr)
    }
    public closeAsync(): void {
        Module._lv_msgbox_close_async(this.ptr)
    }
}
export class Keyboard extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_keyboard_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public setTextarea(textarea:Obj): void {
        Module._lv_keyboard_set_textarea(this.ptr, textarea.ptr)
    }
    public setMode(mode:string): void {
        Module._lv_keyboard_set_mode(this.ptr, constMapping.KEYBOARD_MODE.value(mode))
    }
    public setPopovers(en:any): void {
        Module._lv_keyboard_set_popovers(this.ptr, en)
    }
    public textarea(): Obj {
        return WASMObject.wrap(Module._lv_keyboard_get_textarea(this.ptr), Obj)
    }
    public mode(): any {
        return constMapping.KEYBOARD_MODE.name(Module._lv_keyboard_get_mode(this.ptr))
    }
}
export class TileView extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_tileview_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public addTile(col_id:any, row_id:any, dir:any): Obj {
        return WASMObject.wrap(Module._lv_tileview_add_tile(this.ptr, col_id, row_id, dir), Obj)
    }
    public tileAct(): Obj {
        return WASMObject.wrap(Module._lv_tileview_get_tile_act(this.ptr), Obj)
    }
}
export class List extends Obj {
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_list_create(parent?parent.ptr:null)
        this.registerPointer()
    }
    public addText(txt:string): Obj {
        return WASMObject.wrap(Module.ccall("lv_list_add_text", "number", ["number", "string"], [this.ptr, txt]), Obj)
    }
    public btnText(btn:Obj): string {
        return Module.ccall("lv_list_get_btn_text", "string", ["number", "pointer"], [this.ptr, btn.ptr])
    }
}
// AUTO GENERATE CODE END [LVGL.JS WIDGETS] --------



export const Widgets = {
// AUTO GENERATE CODE START [LVGL.JS WIDGET NAMES] --------
Obj, 
Label, 
Arc, 
Bar, 
Btn, 
BtnMatrix, 
Canvas, 
Checkbox, 
Dropdown, 
Img, 
Line, 
Roller, 
Slider, 
Switch, 
Table, 
TextArea, 
MsgBox, 
Keyboard, 
TileView, 
List
// AUTO GENERATE CODE END [LVGL.JS WIDGET NAMES] --------
}


// AUTO GENERATE CODE START [LVGL.JS STRUCTS] --------
export class DispDrawBuf extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_draw_buf_create()
    }
    public buf1(): any {
        return Module._lv_disp_draw_buf_get_buf1(this.ptr)
    }
    public setBuf1(buf1:any): void {
        Module._lv_disp_draw_buf_set_buf1(this.ptr, buf1)
    }
    public buf2(): any {
        return Module._lv_disp_draw_buf_get_buf2(this.ptr)
    }
    public setBuf2(buf2:any): void {
        Module._lv_disp_draw_buf_set_buf2(this.ptr, buf2)
    }
    public bufAct(): any {
        return Module._lv_disp_draw_buf_get_buf_act(this.ptr)
    }
    public setBufAct(buf_act:any): void {
        Module._lv_disp_draw_buf_set_buf_act(this.ptr, buf_act)
    }
    public size(): any {
        return Module._lv_disp_draw_buf_get_size(this.ptr)
    }
    public setSize(size:any): void {
        Module._lv_disp_draw_buf_set_size(this.ptr, size)
    }
    public area(): any {
        return Module._lv_disp_draw_buf_get_area(this.ptr)
    }
    public setArea(area:any): void {
        Module._lv_disp_draw_buf_set_area(this.ptr, area)
    }
    public flushing(): any {
        return Module._lv_disp_draw_buf_get_flushing(this.ptr)
    }
    public setFlushing(flushing:any): void {
        Module._lv_disp_draw_buf_set_flushing(this.ptr, flushing)
    }
    public flushingLast(): any {
        return Module._lv_disp_draw_buf_get_flushing_last(this.ptr)
    }
    public setFlushingLast(flushing_last:any): void {
        Module._lv_disp_draw_buf_set_flushing_last(this.ptr, flushing_last)
    }
    public lastArea(): any {
        return Module._lv_disp_draw_buf_get_last_area(this.ptr)
    }
    public setLastArea(last_area:any): void {
        Module._lv_disp_draw_buf_set_last_area(this.ptr, last_area)
    }
    public lastPart(): any {
        return Module._lv_disp_draw_buf_get_last_part(this.ptr)
    }
    public setLastPart(last_part:any): void {
        Module._lv_disp_draw_buf_set_last_part(this.ptr, last_part)
    }
    public init(buf1:any, buf2:any, size_in_px_cnt:any): void {
        Module._lv_disp_draw_buf_init(this.ptr, buf1, buf2, size_in_px_cnt)
    }
}
export class DispDrv extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_drv_create()
    }
    public horRes(): any {
        return Module._lv_disp_drv_get_hor_res(this.ptr)
    }
    public setHorRes(hor_res:any): void {
        Module._lv_disp_drv_set_hor_res(this.ptr, hor_res)
    }
    public verRes(): any {
        return Module._lv_disp_drv_get_ver_res(this.ptr)
    }
    public setVerRes(ver_res:any): void {
        Module._lv_disp_drv_set_ver_res(this.ptr, ver_res)
    }
    public physicalHorRes(): any {
        return Module._lv_disp_drv_get_physical_hor_res(this.ptr)
    }
    public setPhysicalHorRes(physical_hor_res:any): void {
        Module._lv_disp_drv_set_physical_hor_res(this.ptr, physical_hor_res)
    }
    public physicalVerRes(): any {
        return Module._lv_disp_drv_get_physical_ver_res(this.ptr)
    }
    public setPhysicalVerRes(physical_ver_res:any): void {
        Module._lv_disp_drv_set_physical_ver_res(this.ptr, physical_ver_res)
    }
    public offsetX(): any {
        return Module._lv_disp_drv_get_offset_x(this.ptr)
    }
    public setOffsetX(offset_x:any): void {
        Module._lv_disp_drv_set_offset_x(this.ptr, offset_x)
    }
    public offsetY(): any {
        return Module._lv_disp_drv_get_offset_y(this.ptr)
    }
    public setOffsetY(offset_y:any): void {
        Module._lv_disp_drv_set_offset_y(this.ptr, offset_y)
    }
    public drawBuf(): DispDrawBuf {
        return WASMObject.wrap(Module._lv_disp_drv_get_draw_buf(this.ptr), DispDrawBuf)
    }
    public setDrawBuf(draw_buf:DispDrawBuf): void {
        Module._lv_disp_drv_set_draw_buf(this.ptr, draw_buf.ptr)
    }
    public directMode(): any {
        return Module._lv_disp_drv_get_direct_mode(this.ptr)
    }
    public setDirectMode(direct_mode:any): void {
        Module._lv_disp_drv_set_direct_mode(this.ptr, direct_mode)
    }
    public fullRefresh(): any {
        return Module._lv_disp_drv_get_full_refresh(this.ptr)
    }
    public setFullRefresh(full_refresh:any): void {
        Module._lv_disp_drv_set_full_refresh(this.ptr, full_refresh)
    }
    public swRotate(): any {
        return Module._lv_disp_drv_get_sw_rotate(this.ptr)
    }
    public setSwRotate(sw_rotate:any): void {
        Module._lv_disp_drv_set_sw_rotate(this.ptr, sw_rotate)
    }
    public antialiasing(): any {
        return Module._lv_disp_drv_get_antialiasing(this.ptr)
    }
    public setAntialiasing(antialiasing:any): void {
        Module._lv_disp_drv_set_antialiasing(this.ptr, antialiasing)
    }
    public rotated(): any {
        return Module._lv_disp_drv_get_rotated(this.ptr)
    }
    public setRotated(rotated:any): void {
        Module._lv_disp_drv_set_rotated(this.ptr, rotated)
    }
    public screenTransp(): any {
        return Module._lv_disp_drv_get_screen_transp(this.ptr)
    }
    public setScreenTransp(screen_transp:any): void {
        Module._lv_disp_drv_set_screen_transp(this.ptr, screen_transp)
    }
    public dpi(): any {
        return Module._lv_disp_drv_get_dpi(this.ptr)
    }
    public setDpi(dpi:any): void {
        Module._lv_disp_drv_set_dpi(this.ptr, dpi)
    }
    public flushCb(): any {
        return Module._lv_disp_drv_get_flush_cb(this.ptr)
    }
    public setFlushCb(flush_cb:any): void {
        Module._lv_disp_drv_set_flush_cb(this.ptr, flush_cb)
    }
    public rounderCb(): any {
        return Module._lv_disp_drv_get_rounder_cb(this.ptr)
    }
    public setRounderCb(rounder_cb:any): void {
        Module._lv_disp_drv_set_rounder_cb(this.ptr, rounder_cb)
    }
    public setPxCb(): any {
        return Module._lv_disp_drv_get_set_px_cb(this.ptr)
    }
    public setSetPxCb(set_px_cb:any): void {
        Module._lv_disp_drv_set_set_px_cb(this.ptr, set_px_cb)
    }
    public monitorCb(): any {
        return Module._lv_disp_drv_get_monitor_cb(this.ptr)
    }
    public setMonitorCb(monitor_cb:any): void {
        Module._lv_disp_drv_set_monitor_cb(this.ptr, monitor_cb)
    }
    public waitCb(): any {
        return Module._lv_disp_drv_get_wait_cb(this.ptr)
    }
    public setWaitCb(wait_cb:any): void {
        Module._lv_disp_drv_set_wait_cb(this.ptr, wait_cb)
    }
    public cleanDcacheCb(): any {
        return Module._lv_disp_drv_get_clean_dcache_cb(this.ptr)
    }
    public setCleanDcacheCb(clean_dcache_cb:any): void {
        Module._lv_disp_drv_set_clean_dcache_cb(this.ptr, clean_dcache_cb)
    }
    public gpuWaitCb(): any {
        return Module._lv_disp_drv_get_gpu_wait_cb(this.ptr)
    }
    public setGpuWaitCb(gpu_wait_cb:any): void {
        Module._lv_disp_drv_set_gpu_wait_cb(this.ptr, gpu_wait_cb)
    }
    public drvUpdateCb(): any {
        return Module._lv_disp_drv_get_drv_update_cb(this.ptr)
    }
    public setDrvUpdateCb(drv_update_cb:any): void {
        Module._lv_disp_drv_set_drv_update_cb(this.ptr, drv_update_cb)
    }
    public gpuFillCb(): any {
        return Module._lv_disp_drv_get_gpu_fill_cb(this.ptr)
    }
    public setGpuFillCb(gpu_fill_cb:any): void {
        Module._lv_disp_drv_set_gpu_fill_cb(this.ptr, gpu_fill_cb)
    }
    public colorChromaKey(): any {
        return Module._lv_disp_drv_get_color_chroma_key(this.ptr)
    }
    public setColorChromaKey(color_chroma_key:any): void {
        Module._lv_disp_drv_set_color_chroma_key(this.ptr, color_chroma_key)
    }
    public userData(): any {
        return Module._lv_disp_drv_get_user_data(this.ptr)
    }
    public setUserData(user_data:any): void {
        Module._lv_disp_drv_set_user_data(this.ptr, user_data)
    }
    public init(): void {
        Module._lv_disp_drv_init(this.ptr)
    }
    public register(): Disp {
        return WASMObject.wrap(Module._lv_disp_drv_register(this.ptr), Disp)
    }
    public useGenericSetPxCb(cf:string): void {
        Module._lv_disp_drv_use_generic_set_px_cb(this.ptr, constMapping.IMG_CF.value(cf))
    }
}
export class Disp extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_disp_create()
    }
    public sharedKeyboard?: Keyboard
    public popupSharedKeyboard(txtarea:TextArea) {
        if(!this.sharedKeyboard){
            this.sharedKeyboard = new Keyboard(this.actScr())
        }
        else {
            this.sharedKeyboard.setParent(this.actScr())
        }
        this.sharedKeyboard.popup(txtarea)
    }

    public driver(): DispDrv {
        return WASMObject.wrap(Module._lv_disp_get_driver(this.ptr), DispDrv)
    }
    public setDriver(driver:DispDrv): void {
        Module._lv_disp_set_driver(this.ptr, driver.ptr)
    }
    public refrTimer(): any {
        return Module._lv_disp_get_refr_timer(this.ptr)
    }
    public setRefrTimer(refr_timer:any): void {
        Module._lv_disp_set_refr_timer(this.ptr, refr_timer)
    }
    public screens(): any {
        return Module._lv_disp_get_screens(this.ptr)
    }
    public setScreens(screens:any): void {
        Module._lv_disp_set_screens(this.ptr, screens)
    }
    public actScr(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_act_scr(this.ptr), Obj)
    }
    public setActScr(act_scr:Obj): void {
        Module._lv_disp_set_act_scr(this.ptr, act_scr.ptr)
    }
    public prevScr(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_prev_scr(this.ptr), Obj)
    }
    public setPrevScr(prev_scr:Obj): void {
        Module._lv_disp_set_prev_scr(this.ptr, prev_scr.ptr)
    }
    public scrToLoad(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_scr_to_load(this.ptr), Obj)
    }
    public setScrToLoad(scr_to_load:Obj): void {
        Module._lv_disp_set_scr_to_load(this.ptr, scr_to_load.ptr)
    }
    public topLayer(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_top_layer(this.ptr), Obj)
    }
    public setTopLayer(top_layer:Obj): void {
        Module._lv_disp_set_top_layer(this.ptr, top_layer.ptr)
    }
    public sysLayer(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_sys_layer(this.ptr), Obj)
    }
    public setSysLayer(sys_layer:Obj): void {
        Module._lv_disp_set_sys_layer(this.ptr, sys_layer.ptr)
    }
    public screenCnt(): any {
        return Module._lv_disp_get_screen_cnt(this.ptr)
    }
    public setScreenCnt(screen_cnt:any): void {
        Module._lv_disp_set_screen_cnt(this.ptr, screen_cnt)
    }
    public delPrev(): any {
        return Module._lv_disp_get_del_prev(this.ptr)
    }
    public setDelPrev(del_prev:any): void {
        Module._lv_disp_set_del_prev(this.ptr, del_prev)
    }
    public bgOpa(): any {
        return Module._lv_disp_get_bg_opa(this.ptr)
    }
    public bgColor(): any {
        return Module._lv_disp_get_bg_color(this.ptr)
    }
    public bgImg(): any {
        return Module._lv_disp_get_bg_img(this.ptr)
    }
    public setBgImg(bg_img:any): void {
        Module._lv_disp_set_bg_img(this.ptr, bg_img)
    }
    public bgFn(): any {
        return Module._lv_disp_get_bg_fn(this.ptr)
    }
    public setBgFn(bg_fn:any): void {
        Module._lv_disp_set_bg_fn(this.ptr, bg_fn)
    }
    public invAreas(): any {
        return Module._lv_disp_get_inv_areas(this.ptr)
    }
    public setInvAreas(inv_areas:any): void {
        Module._lv_disp_set_inv_areas(this.ptr, inv_areas)
    }
    public invAreaJoined(): any {
        return Module._lv_disp_get_inv_area_joined(this.ptr)
    }
    public setInvAreaJoined(inv_area_joined:any): void {
        Module._lv_disp_set_inv_area_joined(this.ptr, inv_area_joined)
    }
    public invP(): any {
        return Module._lv_disp_get_inv_p(this.ptr)
    }
    public setInvP(inv_p:any): void {
        Module._lv_disp_set_inv_p(this.ptr, inv_p)
    }
    public lastActivityTime(): any {
        return Module._lv_disp_get_last_activity_time(this.ptr)
    }
    public setLastActivityTime(last_activity_time:any): void {
        Module._lv_disp_set_last_activity_time(this.ptr, last_activity_time)
    }
    public remove(): void {
        Module._lv_disp_remove(this.ptr)
    }
    public setDefault(): void {
        Module._lv_disp_set_default(this.ptr)
    }
    public horRes(): any {
        return Module._lv_disp_get_hor_res(this.ptr)
    }
    public verRes(): any {
        return Module._lv_disp_get_ver_res(this.ptr)
    }
    public physicalHorRes(): any {
        return Module._lv_disp_get_physical_hor_res(this.ptr)
    }
    public physicalVerRes(): any {
        return Module._lv_disp_get_physical_ver_res(this.ptr)
    }
    public offsetX(): any {
        return Module._lv_disp_get_offset_x(this.ptr)
    }
    public offsetY(): any {
        return Module._lv_disp_get_offset_y(this.ptr)
    }
    public antialiasing(): boolean {
        return Module.ccall("lv_disp_get_antialiasing", "bool", ["number"], [this.ptr])
    }
    public setRotation(rotation:string): void {
        Module._lv_disp_set_rotation(this.ptr, constMapping.DISP_ROT.value(rotation))
    }
    public rotation(): any {
        return constMapping.DISP_ROT.name(Module._lv_disp_get_rotation(this.ptr))
    }
    public next(): Disp {
        return WASMObject.wrap(Module._lv_disp_get_next(this.ptr), Disp)
    }
    public drawBuf(): DispDrawBuf {
        return WASMObject.wrap(Module._lv_disp_get_draw_buf(this.ptr), DispDrawBuf)
    }
    public scrAct(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_scr_act(this.ptr), Obj)
    }
    public scrPrev(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_scr_prev(this.ptr), Obj)
    }
    public layerTop(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_layer_top(this.ptr), Obj)
    }
    public layerSys(): Obj {
        return WASMObject.wrap(Module._lv_disp_get_layer_sys(this.ptr), Obj)
    }
    public setTheme(th:any): void {
        Module._lv_disp_set_theme(this.ptr, th)
    }
    public theme(): any {
        return Module._lv_disp_get_theme(this.ptr)
    }
    public setBgColor(color:any): void {
        Module._lv_disp_set_bg_color(this.ptr, color)
    }
    public setBgImage(img_src:any): void {
        Module._lv_disp_set_bg_image(this.ptr, img_src)
    }
    public setBgOpa(opa:any): void {
        Module._lv_disp_set_bg_opa(this.ptr, opa)
    }
    public trigActivity(): void {
        Module._lv_disp_trig_activity(this.ptr)
    }
    public cleanDcache(): void {
        Module._lv_disp_clean_dcache(this.ptr)
    }
}
export class ObjClass extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_obj_class_create()
    }
    public baseClass(): any {
        return Module._lv_obj_class_get_base_class(this.ptr)
    }
    public setBaseClass(base_class:any): void {
        Module._lv_obj_class_set_base_class(this.ptr, base_class)
    }
    public constructorCb(): any {
        return Module._lv_obj_class_get_constructor_cb(this.ptr)
    }
    public setConstructorCb(constructor_cb:any): void {
        Module._lv_obj_class_set_constructor_cb(this.ptr, constructor_cb)
    }
    public destructorCb(): any {
        return Module._lv_obj_class_get_destructor_cb(this.ptr)
    }
    public setDestructorCb(destructor_cb:any): void {
        Module._lv_obj_class_set_destructor_cb(this.ptr, destructor_cb)
    }
    public userData(): any {
        return Module._lv_obj_class_get_user_data(this.ptr)
    }
    public setUserData(user_data:any): void {
        Module._lv_obj_class_set_user_data(this.ptr, user_data)
    }
    public eventCb(): any {
        return Module._lv_obj_class_get_event_cb(this.ptr)
    }
    public setEventCb(event_cb:any): void {
        Module._lv_obj_class_set_event_cb(this.ptr, event_cb)
    }
    public widthDef(): any {
        return Module._lv_obj_class_get_width_def(this.ptr)
    }
    public setWidthDef(width_def:any): void {
        Module._lv_obj_class_set_width_def(this.ptr, width_def)
    }
    public heightDef(): any {
        return Module._lv_obj_class_get_height_def(this.ptr)
    }
    public setHeightDef(height_def:any): void {
        Module._lv_obj_class_set_height_def(this.ptr, height_def)
    }
    public editable(): any {
        return Module._lv_obj_class_get_editable(this.ptr)
    }
    public setEditable(editable:any): void {
        Module._lv_obj_class_set_editable(this.ptr, editable)
    }
    public groupDef(): any {
        return Module._lv_obj_class_get_group_def(this.ptr)
    }
    public setGroupDef(group_def:any): void {
        Module._lv_obj_class_set_group_def(this.ptr, group_def)
    }
    public instanceSize(): any {
        return Module._lv_obj_class_get_instance_size(this.ptr)
    }
    public setInstanceSize(instance_size:any): void {
        Module._lv_obj_class_set_instance_size(this.ptr, instance_size)
    }
}
export class IndevDrv extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_indev_drv_create()
    }
    public type(): any {
        return constMapping.INDEV_TYPE.name(Module._lv_indev_drv_get_type(this.ptr))
    }
    public setType(type:string): void {
        Module._lv_indev_drv_set_type(this.ptr, constMapping.INDEV_TYPE.value(type))
    }
    public readCb(): any {
        return Module._lv_indev_drv_get_read_cb(this.ptr)
    }
    public setReadCb(read_cb:any): void {
        Module._lv_indev_drv_set_read_cb(this.ptr, read_cb)
    }
    public feedbackCb(): any {
        return Module._lv_indev_drv_get_feedback_cb(this.ptr)
    }
    public setFeedbackCb(feedback_cb:any): void {
        Module._lv_indev_drv_set_feedback_cb(this.ptr, feedback_cb)
    }
    public userData(): any {
        return Module._lv_indev_drv_get_user_data(this.ptr)
    }
    public setUserData(user_data:any): void {
        Module._lv_indev_drv_set_user_data(this.ptr, user_data)
    }
    public disp(): Disp {
        return WASMObject.wrap(Module._lv_indev_drv_get_disp(this.ptr), Disp)
    }
    public setDisp(disp:Disp): void {
        Module._lv_indev_drv_set_disp(this.ptr, disp.ptr)
    }
    public readTimer(): any {
        return Module._lv_indev_drv_get_read_timer(this.ptr)
    }
    public setReadTimer(read_timer:any): void {
        Module._lv_indev_drv_set_read_timer(this.ptr, read_timer)
    }
    public scrollLimit(): any {
        return Module._lv_indev_drv_get_scroll_limit(this.ptr)
    }
    public setScrollLimit(scroll_limit:any): void {
        Module._lv_indev_drv_set_scroll_limit(this.ptr, scroll_limit)
    }
    public scrollThrow(): any {
        return Module._lv_indev_drv_get_scroll_throw(this.ptr)
    }
    public setScrollThrow(scroll_throw:any): void {
        Module._lv_indev_drv_set_scroll_throw(this.ptr, scroll_throw)
    }
    public gestureMinVelocity(): any {
        return Module._lv_indev_drv_get_gesture_min_velocity(this.ptr)
    }
    public setGestureMinVelocity(gesture_min_velocity:any): void {
        Module._lv_indev_drv_set_gesture_min_velocity(this.ptr, gesture_min_velocity)
    }
    public gestureLimit(): any {
        return Module._lv_indev_drv_get_gesture_limit(this.ptr)
    }
    public setGestureLimit(gesture_limit:any): void {
        Module._lv_indev_drv_set_gesture_limit(this.ptr, gesture_limit)
    }
    public longPressTime(): any {
        return Module._lv_indev_drv_get_long_press_time(this.ptr)
    }
    public setLongPressTime(long_press_time:any): void {
        Module._lv_indev_drv_set_long_press_time(this.ptr, long_press_time)
    }
    public longPressRepeatTime(): any {
        return Module._lv_indev_drv_get_long_press_repeat_time(this.ptr)
    }
    public setLongPressRepeatTime(long_press_repeat_time:any): void {
        Module._lv_indev_drv_set_long_press_repeat_time(this.ptr, long_press_repeat_time)
    }
    public init(): void {
        Module._lv_indev_drv_init(this.ptr)
    }
    public register(): any {
        return Module._lv_indev_drv_register(this.ptr)
    }
}
export class IndevData extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_indev_data_create()
    }
    public point(): any {
        return Module._lv_indev_data_get_point(this.ptr)
    }
    public setPoint(point:any): void {
        Module._lv_indev_data_set_point(this.ptr, point)
    }
    public key(): any {
        return Module._lv_indev_data_get_key(this.ptr)
    }
    public setKey(key:any): void {
        Module._lv_indev_data_set_key(this.ptr, key)
    }
    public btnId(): any {
        return Module._lv_indev_data_get_btn_id(this.ptr)
    }
    public setBtnId(btn_id:any): void {
        Module._lv_indev_data_set_btn_id(this.ptr, btn_id)
    }
    public encDiff(): any {
        return Module._lv_indev_data_get_enc_diff(this.ptr)
    }
    public setEncDiff(enc_diff:any): void {
        Module._lv_indev_data_set_enc_diff(this.ptr, enc_diff)
    }
    public state(): any {
        return constMapping.INDEV_STATE.name(Module._lv_indev_data_get_state(this.ptr))
    }
    public setState(state:string): void {
        Module._lv_indev_data_set_state(this.ptr, constMapping.INDEV_STATE.value(state))
    }
    public continueReading(): boolean {
        return Module.ccall("lv_indev_data_get_continue_reading", "bool", ["number"], [this.ptr])
    }
    public setContinueReading(continue_reading:any): void {
        Module._lv_indev_data_set_continue_reading(this.ptr, continue_reading)
    }
}
export class Point extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._lv_point_create()
    }
    public x(): any {
        return Module._lv_point_get_x(this.ptr)
    }
    public setX(x:any): void {
        Module._lv_point_set_x(this.ptr, x)
    }
    public y(): any {
        return Module._lv_point_get_y(this.ptr)
    }
    public setY(y:any): void {
        Module._lv_point_set_y(this.ptr, y)
    }
}
// AUTO GENERATE CODE END [LVGL.JS STRUCTS] --------


// AUTO GENERATE CODE START [LVGL.JS CONST MAPPING] --------
const constMapping = {
    RES : {
        value(name: string): number {
            let value = Module.ccall("lv_res_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const RES value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_res_to_name", "string", ["number"], [value]) }
    } ,
    ANIM_ENABLE : {
        value(name: string): number {
            let value = Module.ccall("lv_anim_enable_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ANIM_ENABLE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_anim_enable_to_name", "string", ["number"], [value]) }
    } ,
    PALETTE : {
        value(name: string): number {
            let value = Module.ccall("lv_palette_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const PALETTE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_palette_to_name", "string", ["number"], [value]) }
    } ,
    ALIGN : {
        value(name: string): number {
            let value = Module.ccall("lv_align_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ALIGN value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_align_to_name", "string", ["number"], [value]) }
    } ,
    DIR : {
        value(name: string): number {
            let value = Module.ccall("lv_dir_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const DIR value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_dir_to_name", "string", ["number"], [value]) }
    } ,
    IMG_CF : {
        value(name: string): number {
            let value = Module.ccall("lv_img_cf_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const IMG_CF value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_img_cf_to_name", "string", ["number"], [value]) }
    } ,
    DISP_ROT : {
        value(name: string): number {
            let value = Module.ccall("lv_disp_rot_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const DISP_ROT value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_disp_rot_to_name", "string", ["number"], [value]) }
    } ,
    INDEV_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_indev_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const INDEV_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_indev_type_to_name", "string", ["number"], [value]) }
    } ,
    INDEV_STATE : {
        value(name: string): number {
            let value = Module.ccall("lv_indev_state_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const INDEV_STATE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_indev_state_to_name", "string", ["number"], [value]) }
    } ,
    FONT_SUBPX : {
        value(name: string): number {
            let value = Module.ccall("lv_font_subpx_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FONT_SUBPX value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_font_subpx_to_name", "string", ["number"], [value]) }
    } ,
    TEXT_FLAG : {
        value(name: string): number {
            let value = Module.ccall("lv_text_flag_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TEXT_FLAG value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_text_flag_to_name", "string", ["number"], [value]) }
    } ,
    TEXT_CMD_STATE : {
        value(name: string): number {
            let value = Module.ccall("lv_text_cmd_state_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TEXT_CMD_STATE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_text_cmd_state_to_name", "string", ["number"], [value]) }
    } ,
    TEXT_ALIGN : {
        value(name: string): number {
            let value = Module.ccall("lv_text_align_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TEXT_ALIGN value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_text_align_to_name", "string", ["number"], [value]) }
    } ,
    BASE_DIR : {
        value(name: string): number {
            let value = Module.ccall("lv_base_dir_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BASE_DIR value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_base_dir_to_name", "string", ["number"], [value]) }
    } ,
    BLEND_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_blend_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BLEND_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_blend_mode_to_name", "string", ["number"], [value]) }
    } ,
    TEXT_DECOR : {
        value(name: string): number {
            let value = Module.ccall("lv_text_decor_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TEXT_DECOR value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_text_decor_to_name", "string", ["number"], [value]) }
    } ,
    BORDER_SIDE : {
        value(name: string): number {
            let value = Module.ccall("lv_border_side_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BORDER_SIDE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_border_side_to_name", "string", ["number"], [value]) }
    } ,
    GRAD_DIR : {
        value(name: string): number {
            let value = Module.ccall("lv_grad_dir_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const GRAD_DIR value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_grad_dir_to_name", "string", ["number"], [value]) }
    } ,
    STYLE_PROP : {
        value(name: string): number {
            let value = Module.ccall("lv_style_prop_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const STYLE_PROP value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_style_prop_to_name", "string", ["number"], [value]) }
    } ,
    STATE : {
        value(name: string): number {
            let value = Module.ccall("lv_state_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const STATE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_state_to_name", "string", ["number"], [value]) }
    } ,
    PART : {
        value(name: string): number {
            let value = Module.ccall("lv_part_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const PART value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_part_to_name", "string", ["number"], [value]) }
    } ,
    OBJ_FLAG : {
        value(name: string): number {
            let value = Module.ccall("lv_obj_flag_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const OBJ_FLAG value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_obj_flag_to_name", "string", ["number"], [value]) }
    } ,
    OBJ_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_obj_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const OBJ_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_obj_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    OBJ_TREE_WALK_RES : {
        value(name: string): number {
            let value = Module.ccall("lv_obj_tree_walk_res_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const OBJ_TREE_WALK_RES value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_obj_tree_walk_res_to_name", "string", ["number"], [value]) }
    } ,
    SCROLLBAR_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_scrollbar_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SCROLLBAR_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_scrollbar_mode_to_name", "string", ["number"], [value]) }
    } ,
    SCROLL_SNAP : {
        value(name: string): number {
            let value = Module.ccall("lv_scroll_snap_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SCROLL_SNAP value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_scroll_snap_to_name", "string", ["number"], [value]) }
    } ,
    _STYLE_STATE_CMP : {
        value(name: string): number {
            let value = Module.ccall("lv__style_state_cmp_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const _STYLE_STATE_CMP value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv__style_state_cmp_to_name", "string", ["number"], [value]) }
    } ,
    FS_RES : {
        value(name: string): number {
            let value = Module.ccall("lv_fs_res_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FS_RES value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_fs_res_to_name", "string", ["number"], [value]) }
    } ,
    FS_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_fs_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FS_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_fs_mode_to_name", "string", ["number"], [value]) }
    } ,
    FS_WHENCE : {
        value(name: string): number {
            let value = Module.ccall("lv_fs_whence_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FS_WHENCE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_fs_whence_to_name", "string", ["number"], [value]) }
    } ,
    IMG_SRC : {
        value(name: string): number {
            let value = Module.ccall("lv_img_src_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const IMG_SRC value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_img_src_to_name", "string", ["number"], [value]) }
    } ,
    DRAW_MASK_RES : {
        value(name: string): number {
            let value = Module.ccall("lv_draw_mask_res_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const DRAW_MASK_RES value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_draw_mask_res_to_name", "string", ["number"], [value]) }
    } ,
    DRAW_MASK_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_draw_mask_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const DRAW_MASK_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_draw_mask_type_to_name", "string", ["number"], [value]) }
    } ,
    DRAW_MASK_XCB : {
        value(name: string): number {
            let value = Module.ccall("lv_draw_mask_xcb_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const DRAW_MASK_XCB value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_draw_mask_xcb_to_name", "string", ["number"], [value]) }
    } ,
    COVER_RES : {
        value(name: string): number {
            let value = Module.ccall("lv_cover_res_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const COVER_RES value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_cover_res_to_name", "string", ["number"], [value]) }
    } ,
    OBJ_CLASS_EDITABLE : {
        value(name: string): number {
            let value = Module.ccall("lv_obj_class_editable_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const OBJ_CLASS_EDITABLE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_obj_class_editable_to_name", "string", ["number"], [value]) }
    } ,
    OBJ_CLASS_GROUP_DEF : {
        value(name: string): number {
            let value = Module.ccall("lv_obj_class_group_def_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const OBJ_CLASS_GROUP_DEF value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_obj_class_group_def_to_name", "string", ["number"], [value]) }
    } ,
    EVENT_CODE : {
        value(name: string): number {
            let value = Module.ccall("lv_event_code_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const EVENT_CODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_event_code_to_name", "string", ["number"], [value]) }
    } ,
    KEY : {
        value(name: string): number {
            let value = Module.ccall("lv_key_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const KEY value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_key_to_name", "string", ["number"], [value]) }
    } ,
    GROUP_REFOCUS_POLICY : {
        value(name: string): number {
            let value = Module.ccall("lv_group_refocus_policy_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const GROUP_REFOCUS_POLICY value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_group_refocus_policy_to_name", "string", ["number"], [value]) }
    } ,
    SCR_LOAD_ANIM : {
        value(name: string): number {
            let value = Module.ccall("lv_scr_load_anim_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SCR_LOAD_ANIM value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_scr_load_anim_to_name", "string", ["number"], [value]) }
    } ,
    FONT_FMT_TXT_CMAP_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_font_fmt_txt_cmap_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FONT_FMT_TXT_CMAP_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_font_fmt_txt_cmap_type_to_name", "string", ["number"], [value]) }
    } ,
    FONT_FMT_TXT_BITMAP_FORMAT : {
        value(name: string): number {
            let value = Module.ccall("lv_font_fmt_txt_bitmap_format_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FONT_FMT_TXT_BITMAP_FORMAT value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_font_fmt_txt_bitmap_format_to_name", "string", ["number"], [value]) }
    } ,
    ARC_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_arc_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ARC_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_arc_mode_to_name", "string", ["number"], [value]) }
    } ,
    ARC_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_arc_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ARC_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_arc_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    IMG_SIZE_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_img_size_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const IMG_SIZE_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_img_size_mode_to_name", "string", ["number"], [value]) }
    } ,
    LABEL_LONG_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_label_long_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const LABEL_LONG_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_label_long_mode_to_name", "string", ["number"], [value]) }
    } ,
    TABLE_CELL_CTRL : {
        value(name: string): number {
            let value = Module.ccall("lv_table_cell_ctrl_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TABLE_CELL_CTRL value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_table_cell_ctrl_to_name", "string", ["number"], [value]) }
    } ,
    TABLE_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_table_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const TABLE_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_table_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    CHECKBOX_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_checkbox_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const CHECKBOX_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_checkbox_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    BAR_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_bar_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BAR_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_bar_mode_to_name", "string", ["number"], [value]) }
    } ,
    BAR_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_bar_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BAR_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_bar_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    SLIDER_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_slider_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SLIDER_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_slider_mode_to_name", "string", ["number"], [value]) }
    } ,
    SLIDER_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_slider_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SLIDER_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_slider_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    BTNMATRIX_CTRL : {
        value(name: string): number {
            let value = Module.ccall("lv_btnmatrix_ctrl_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BTNMATRIX_CTRL value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_btnmatrix_ctrl_to_name", "string", ["number"], [value]) }
    } ,
    BTNMATRIX_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_btnmatrix_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const BTNMATRIX_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_btnmatrix_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    ROLLER_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_roller_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ROLLER_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_roller_mode_to_name", "string", ["number"], [value]) }
    } ,
    ANIMIMG_PART : {
        value(name: string): number {
            let value = Module.ccall("lv_animimg_part_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ANIMIMG_PART value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_animimg_part_to_name", "string", ["number"], [value]) }
    } ,
    CHART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_chart_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const CHART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_chart_type_to_name", "string", ["number"], [value]) }
    } ,
    CHART_UPDATE_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_chart_update_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const CHART_UPDATE_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_chart_update_mode_to_name", "string", ["number"], [value]) }
    } ,
    CHART_AXIS : {
        value(name: string): number {
            let value = Module.ccall("lv_chart_axis_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const CHART_AXIS value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_chart_axis_to_name", "string", ["number"], [value]) }
    } ,
    CHART_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_chart_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const CHART_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_chart_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    KEYBOARD_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_keyboard_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const KEYBOARD_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_keyboard_mode_to_name", "string", ["number"], [value]) }
    } ,
    FLEX_ALIGN : {
        value(name: string): number {
            let value = Module.ccall("lv_flex_align_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FLEX_ALIGN value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_flex_align_to_name", "string", ["number"], [value]) }
    } ,
    FLEX_FLOW : {
        value(name: string): number {
            let value = Module.ccall("lv_flex_flow_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const FLEX_FLOW value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_flex_flow_to_name", "string", ["number"], [value]) }
    } ,
    METER_INDICATOR_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_meter_indicator_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const METER_INDICATOR_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_meter_indicator_type_to_name", "string", ["number"], [value]) }
    } ,
    METER_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_meter_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const METER_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_meter_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    COLORWHEEL_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_colorwheel_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const COLORWHEEL_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_colorwheel_mode_to_name", "string", ["number"], [value]) }
    } ,
    LED_DRAW_PART_TYPE : {
        value(name: string): number {
            let value = Module.ccall("lv_led_draw_part_type_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const LED_DRAW_PART_TYPE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_led_draw_part_type_to_name", "string", ["number"], [value]) }
    } ,
    IMGBTN_STATE : {
        value(name: string): number {
            let value = Module.ccall("lv_imgbtn_state_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const IMGBTN_STATE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_imgbtn_state_to_name", "string", ["number"], [value]) }
    } ,
    SPAN_OVERFLOW : {
        value(name: string): number {
            let value = Module.ccall("lv_span_overflow_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SPAN_OVERFLOW value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_span_overflow_to_name", "string", ["number"], [value]) }
    } ,
    SPAN_MODE : {
        value(name: string): number {
            let value = Module.ccall("lv_span_mode_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const SPAN_MODE value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_span_mode_to_name", "string", ["number"], [value]) }
    } ,
    GRID_ALIGN : {
        value(name: string): number {
            let value = Module.ccall("lv_grid_align_to_value", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const GRID_ALIGN value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("lv_grid_align_to_name", "string", ["number"], [value]) }
    } ,
}

export type FLEX_FLOW = "row" | "column" | "row-wrap" | "row-reverse" | "row-wrap-reverse" | "column-wrap" | "column-reverse" | "column-wrap-reverse"
export type FLEX_ALIGN = "start" | "end" | "center" | "space-evenly" | "space-around" | "space-between"
export type EVENT_CODE = "all" | "pressed" | "pressing" | "press-lost" | "short-clicked" | "long-pressed" | "long-pressed-repeat" | "clicked" | "released" | "scroll-begin" | "scroll-end" | "scroll" | "gesture" | "key" | "focused" | "defocused" | "leave" | "hit-test" | "cover-check" | "refr-ext-draw-size" | "draw-main-begin" | "draw-main" | "draw-main-end" | "draw-post-begin" | "draw-post" | "draw-post-end" | "draw-part-begin" | "draw-part-end" | "value-changed" | "insert" | "refresh" | "ready" | "cancel" | "delete" | "child-changed" | "child-created" | "child-deleted" | "screen-unload-start" | "screen-load-start" | "screen-loaded" | "screen-unloaded" | "size-changed" | "style-changed" | "layout-changed" | "get-self-size"
export type DIR = "none" | "left" | "right" | "top" | "bottom" | "hor" | "ver" | "all"
export type ALIGN = "default" | "top-left" | "top-mid" | "top-right" | "bottom-left" | "bottom-mid" | "bottom-right" | "left-mid" | "right-mid" | "center" | "out-top-left" | "out-top-mid" | "out-top-right" | "out-bottom-left" | "out-bottom-mid" | "out-bottom-right" | "out-left-top" | "out-left-mid" | "out-left-bottom" | "out-right-top" | "out-right-mid" | "out-right-bottom"
export type BORDER_SIDE = "none" | "bottom" | "top" | "left" | "right" | "full" | "internal"
export type BASE_DIR = "ltr" | "rtl" | "auto" | "neutral" | "weak"
export type STYLE_PROP = "prop-inv" | "width" | "min-width" | "max-width" | "height" | "min-height" | "max-height" | "x" | "y" | "align" | "transform-width" | "transform-height" | "translate-x" | "translate-y" | "transform-zoom" | "transform-angle" | "pad-top" | "pad-bottom" | "pad-left" | "pad-right" | "pad-row" | "pad-column" | "bg-color" | "bg-color-filtered" | "bg-opa" | "bg-grad-color" | "bg-grad-color-filtered" | "bg-grad-dir" | "bg-main-stop" | "bg-grad-stop" | "bg-img-src" | "bg-img-opa" | "bg-img-recolor" | "bg-img-recolor-filtered" | "bg-img-recolor-opa" | "bg-img-tiled" | "border-color" | "border-color-filtered" | "border-opa" | "border-width" | "border-side" | "border-post" | "outline-width" | "outline-color" | "outline-color-filtered" | "outline-opa" | "outline-pad" | "shadow-width" | "shadow-ofs-x" | "shadow-ofs-y" | "shadow-spread" | "shadow-color" | "shadow-color-filtered" | "shadow-opa" | "img-opa" | "img-recolor" | "img-recolor-filtered" | "img-recolor-opa" | "line-width" | "line-dash-width" | "line-dash-gap" | "line-rounded" | "line-color" | "line-color-filtered" | "line-opa" | "arc-width" | "arc-rounded" | "arc-color" | "arc-color-filtered" | "arc-opa" | "arc-img-src" | "text-color" | "text-color-filtered" | "text-opa" | "text-font" | "text-letter-space" | "text-line-space" | "text-decor" | "text-align" | "radius" | "clip-corner" | "opa" | "color-filter-dsc" | "color-filter-opa" | "anim-time" | "anim-speed" | "transition" | "blend-mode" | "layout" | "base-dir" | "-last-built-in-prop"
export type SCR_LOAD_ANIM = "none" | "over-left" | "over-right" | "over-top" | "over-bottom" | "move-left" | "move-right" | "move-top" | "move-bottom" | "fade-on"
export type STATE = "default" | "checked" | "focused" | "focus-key" | "edited" | "hovered" | "pressed" | "scrolled" | "disabled" | "user-1" | "user-2" | "user-3" | "user-4" | "any"
export type OBJ_FLAG = "hidden" | "clickable" | "click-focusable" | "checkable" | "scrollable" | "scroll-elastic" | "scroll-momentum" | "scroll-one" | "scroll-chain" | "scroll-on-focus" | "snappable" | "press-lock" | "event-bubble" | "gesture-bubble" | "adv-hittest" | "ignore-layout" | "floating" | "layout-1" | "layout-2" | "widget-1" | "widget-2" | "user-1" | "user-2" | "user-3" | "user-4"
export type TEXT_ALIGN = "auto" | "left" | "center" | "right"
export type PALETTE = "red" | "pink" | "purple" | "deep-purple" | "indigo" | "blue" | "light-blue" | "cyan" | "teal" | "green" | "light-green" | "lime" | "yellow" | "amber" | "orange" | "deep-orange" | "brown" | "blue-grey" | "grey" | "-last" | "none"
export type LABEL_LONG_MODE = "wrap" | "dot" | "scroll" | "scroll-circular" | "clip"
export type KEYBOARD_MODE = "text-lower" | "text-upper" | "special" | "number" | "user-1" | "user-2" | "user-3" | "user-4"
export type IMG_CF = "unknown" | "raw" | "raw-alpha" | "raw-chroma-keyed" | "true-color" | "true-color-alpha" | "true-color-chroma-keyed" | "indexed-1bit" | "indexed-2bit" | "indexed-4bit" | "indexed-8bit" | "alpha-1bit" | "alpha-2bit" | "alpha-4bit" | "alpha-8bit" | "reserved-15" | "reserved-16" | "reserved-17" | "reserved-18" | "reserved-19" | "reserved-20" | "reserved-21" | "reserved-22" | "reserved-23" | "user-encoded-0" | "user-encoded-1" | "user-encoded-2" | "user-encoded-3" | "user-encoded-4" | "user-encoded-5" | "user-encoded-6" | "user-encoded-7"
export type SCROLL_SNAP = "none" | "start" | "end" | "center"
// AUTO GENERATE CODE END [LVGL.JS CONST MAPPING] --------


export declare interface IndevData {
    x(): number ;
    setX(val:number): void ;
    y(): number ;
    setY(val:number): void ;
}
; (IndevData as any).prototype.x = function() {
    return Module._lv_indev_data_get_point_x(this.ptr)
}
; (IndevData as any).prototype.setX = function(x:number) {
    Module._lv_indev_data_set_point_x(this.ptr,x)
}
; (IndevData as any).prototype.y = function() {
    return Module._lv_indev_data_get_point_y(this.ptr)
}
; (IndevData as any).prototype.setY = function(y:number) {
    Module._lv_indev_data_set_point_y(this.ptr,y)
}


export declare interface IndevDrv {
    tick(): void ;
}
; (IndevDrv as any).prototype.tick = function() {
    Module._lv_indev_drv_tick(this.ptr)
}


; (Obj as any).prototype.show = function show() {
    this.clearFlag("hidden")
}
; (Obj as any).prototype.hide = function hide() {
    this.addFlag("hidden")
}
; (Obj as any).prototype.toggle = function toggle() {
    if(this.isVisible()) {
        this.addFlag("hidden")
        return false
    }
    else {
        this.clearFlag("hidden")
        return true
    }
}

export const SizeContent = Module._lv_coord_size_content()

function size(val:string|number) {
    if(typeof val=='string' && val[val.length-1]=='%') {
        let _val = Math.round(val.substr(0,val.length-1) as any)
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

let lastTime = 0
function loop() {
    let now = Date.now()
    Module.asm.lv_tick_inc(now-lastTime)
    // console.log(now-lastTime)
    Module.asm.lv_timer_handler() // lv_task_handler
    lastTime = now
    requestAnimationFrame(loop)
}

let inited = false
export function init() {
    if(inited) {
        return false
    }
    inited = true
    
    Module._lv_init()
    
    lastTime = Date.now()
    requestAnimationFrame(loop)

    return true
}


export class CanvasDispDrv extends DispDrv {
    constructor(private canvas: HTMLCanvasElement, private colorDepth = 2) {
        super()
        this.setAntialiasing(true)
        this.registerPointer()
    }

    public dispBuff?:DispDrawBuf
    public disp?:Disp
    public indevDrv?:IndevDrv
    public indevData?:IndevData

    public setup() {

        this.dispBuff = new DispDrawBuf()
        let pixels = this.canvas.width*this.canvas.height
        let buff = Module._malloc(this.colorDepth*pixels)
        this.dispBuff.init(buff, 0, pixels)

        this.setDrawBuf(this.dispBuff)
        this.setHorRes(this.canvas.width)
        this.setVerRes(this.canvas.height)
        this.setFlushCb(Module._cb_adapter_disp_drv_flush())
        
        // display
        this.disp = this.register()
        
        // indev driver
        this.indevDrv = new IndevDrv()
        this.indevDrv.init()
        this.indevDrv.setType("pointer")
        this.indevDrv.setGestureLimit(30)
        this.indevDrv.setReadCb(Module._cb_adapter_indev_drv_read())
        this.indevDrv.register()

        this.indevData = new IndevData()
        this.indevDrv.setUserData(this.indevData.ptr)
        this.canvas.onmousemove = (e)=> {
            if(e.buttons&1) {
                this.indevData?.setX(e.offsetX)
                this.indevData?.setY(e.offsetY)
            }
        }
        this.canvas.onmousedown = (e) => {
            this.indevData?.setX(e.offsetX)
            this.indevData?.setY(e.offsetY)
            this.indevData?.setState("pressed")
        }
        this.canvas.onmouseup = (e) => {
            this.indevData?.setState("released")
        }
        this.canvas.onmouseout = (e) => {
            this.indevData?.setState("released")
        }

        return this.disp
    }
}

Module.onDispDrvFlush = function(pdrv:number, x1:number, y1:number, x2:number, y2:number, data:number) {
    let width = x2 - x1 + 1
    let height = y2 - y1 + 1
    let datalen = width * height * 2

    const pixels = new Uint8ClampedArray(new ArrayBuffer(width * height * 4))
    for(let i=0;i<datalen;i+=2) {
        let hb = Module.HEAPU8[data+i]
        let lb = Module.HEAPU8[data+i+1]

        let pi = i *2
        pixels[pi] = ((hb>>3) & 255) * 255 / 31 
        pixels[pi+1] = (((hb & 0x07) << 3) | ((lb >> 5) & 255))*255/63
        pixels[pi+2] = (lb & 0x1f)* 255 / 31 
        pixels[pi+3] = 255
    }

    const imageData = new ImageData(pixels, width, height);

    let drv = WASMObject.wrap(pdrv)
    if(drv){
        drv.canvas.getContext("2d").putImageData(imageData, x1, y1);
    }
}

Module.onObjEventEmit = function(pobj:number, event:number, ...params: any[]) {
    let eventName = constMapping.EVENT_CODE.name(event)
    let obj: Obj = WASMObject.wrap(pobj)
    if(obj) {
        obj.emit(eventName, ...(params||[]))
    }
}

Module.asString = function(ptr:number) {
    let str = ''
    for(let i=ptr; i<Module.HEAP8.length; i++) {
        if(Module.HEAP8[i]==0)
            return str
        str+= String.fromCharCode(Module.HEAP8[i]) ;
    }
}



export type DraggablePos = {x:number|false, y:number|false} ;
export type DragStartCB = () => void|false ;
export type DraggingCB = (pos:DraggablePos) => void|false ;
export type DragStopCB = (userStop:boolean) => void ;

export class Draggable {
    constructor(private target: Obj) {
        target.addFlag("clickable")
        // start
        target.on("pressed", ()=> this.start())
        // dragging
        target.on("pressing", (e:any)=>{
            if(!this.isEnabled || !this.isDragging || !this.indevData) {
                return
            }
            this.pos.x = this.indevData.x() + this.ox
            this.pos.y = this.indevData.y() + this.oy
            if(this.cbDragging && this.cbDragging(this.pos)===false) {
                this.cbStop && this.cbStop(true)
                this.isDragging = false
                return
            }
            if((this.pos as any).x!==false) {
                target.setCoordX(this.pos.x as number)
            }
            if((this.pos as any).y!==false) {
                target.setCoordY(this.pos.y as number)
            }
        })
        // stop
        target.on("released", ()=>{
            this.cbStop && this.cbStop(false)
            this.isDragging = false
        })
    }

    start() {
        if(!this.isEnabled || this.isDragging) {
            return
        }
        this.indevData = (this.target.disp().driver() as CanvasDispDrv).indevData
        if(!this.indevData) {
            return
        }
        let [x,y] = this.target.coords()
        this.ox = x - this.indevData.x()
        this.oy = y - this.indevData.y()
        if(this.cbStart && this.cbStart()===false) {
            return
        }
        this.isDragging = true
    }

    private isEnabled = true
    private isDragging = false
    private cbStart?: DragStartCB
    private cbDragging?: DraggingCB
    private cbStop?: DragStopCB

    private pos: DraggablePos = {x:false, y:false}
    private ox: number = 0
    private oy: number = 0
    private indevData?: IndevData

    setStart(callback: DragStartCB) {
        this.cbStart = callback
    }
    setDragging(callback: DraggingCB) {
        this.cbDragging = callback
    }
    setStop(callback: DragStopCB) {
        this.cbStop = callback
    }

    enable() {
        this.isEnabled = true
    }
    disable() {
        this.isEnabled = false
    }
}

export type StyleValue = string | number | Color
export type StyleProp = string

export class Style extends WASMObject {
    constructor(propsOrPtr:number|{[key:string]:StyleValue}=0) {
        let ptr = 0
        if(typeof propsOrPtr=="number") {
            ptr = propsOrPtr
        }
        if(ptr==0) {
            ptr = Module._lv_style_create()
        }
        super(ptr)
        if(typeof propsOrPtr=='object' ) {
            for(let propName in propsOrPtr) {
                this.set(propName, propsOrPtr[propName])
            }
        }
    }

    public get(propName: string): StyleValue {
        Module.__lastError = null
        Module.ccall("lv_style_get", "void", ["number","string"], [this.ptr, propName])
        if(Module.__lastError) {
            throw Module.__lastError
        }
        return Module.__return
    }
    public set(propName: string, value: StyleValue) {
        switch(Module.ccall("lv_style_datatype", "number", ["string"], [propName])) {
            case 1: // number
                return Module.ccall("lv_style_set_number", "boolean", ["number", "string", "number"], [this.ptr, propName, value])
            case 2: // string
                return Module.ccall("lv_style_set_string", "boolean", ["number", "string", "string"], [this.ptr, propName, value])
            case 3: // color
                if(value instanceof Color) {
                    value = value.toRGB565()
                }
                return Module.ccall("lv_style_set_color", "boolean", ["number", "string", "number"], [this.ptr, propName, value])
        }
        return false
    }
    public props(): StyleProp[] {
        Module._lv_style_props(this.ptr)
        return Module.__return
    }
}


;(Obj as any).prototype.draggable = function(onstart?:DragStartCB|null, ondragging?:DraggingCB|null, onstop?: DragStopCB|null) {
    if(!this._draggable) {
        this._draggable = new Draggable(this)
        if(onstart) { this._draggable.setStart(onstart) }
        if(ondragging) { this._draggable.setDragging(ondragging) }
        if(onstop) { this._draggable.setStop(onstop) }
    }
    this._draggable.enable()
    return this._draggable
}

;(Obj as any).prototype.localStyle = function (selector=0) {
    let stylePtr = Module._lv_obj_get_local_style(this.ptr, selector)
    if(!stylePtr) {
        return null
    }
    return new Style(stylePtr)
}

;(Obj as any).prototype.show = function show() {
    this.clearFlag("hidden")
}
;(Obj as any).prototype.hide = function hide() {
    this.addFlag("hidden")
}
;(Obj as any).prototype.toggle = function toggle() {
    if(this.isVisible()) {
        this.addFlag("hidden")
        return false
    }
    else {
        this.clearFlag("hidden")
        return true
    }
}
;(Obj as any).prototype.setStyle = function setStyle(styleName:string, value: string|number|Color, selector=0): boolean {
    switch(Module.ccall("lv_style_datatype", "number", ["string"], [styleName])) {
        case 1: // number
            return Module.ccall("js_lv_obj_set_style_num", "boolean", ["number", "string", "number", "number"], [this.ptr, styleName, value, selector])
        case 2: // string
            return Module.ccall("js_lv_obj_set_style_string", "boolean", ["number", "string", "string", "number"], [this.ptr, styleName, value, selector])
        case 3: // color
            if(value instanceof Color) {
                value = value.toRGB565()
            }
            return Module.ccall("js_lv_obj_set_style_color", "boolean", ["number", "string", "number", "number"], [this.ptr, styleName, value, selector])
    }
    return false
}

;(Obj as any).prototype.style = function setStyle(styleName:string, selector=0): any {
    Module.__lastError = null
    Module.ccall("lv_obj_get_style", "void", ["number", "string", "number"], [this.ptr, styleName, selector])
    if(Module.__lastError) {
        throw Module.__lastError
    }
    // color
    if( Module.ccall("lv_style_datatype", "number", ["string"], [styleName]) == 3 ) {
        return Color.fromRGB565(Module.__return)
    }
    return Module.__return
}


const MapPropFuncs = {
    // clear: 'removeStyleAll' ,
    align: 'setAlign' ,
    flex: 'setFlexFlow' ,
    flexAlign: 'setFlexAlign' ,
    text: 'setText' ,
    grow: 'setFlexGrow' ,
    center: 'center' ,
    width: 'setWidth' ,
    height: 'setHeight' ,
    font: 'setFont' ,
    value: 'setValue' ,
    src: 'setSrc' ,
    longMode: 'setLongMode' ,
    oneLine: 'setOneLine' ,
    x: 'setX' ,
    y: 'setY' ,
    leftValue: 'setLeftValue' ,
}
const MapPressEvents = {
    pressed:'pressed',
    pressing:'pressing',
    released:'released',
    clicked:'clicked',
}
const MapEvents = {
    valueChanged: 'value-changed',
    __proto__: MapPressEvents ,
}

export function fromJson(json: any, parent: Obj, refs?: any): any{
    if(!refs) {
        refs={}
    }
    try{
        if(json instanceof Array) {
            let lst = []
            for(let childJson of json) {
                lst.push(fromJson(childJson, parent, refs))
            }
            return lst
        }
        else {
            let clz = json["class"] || json.clazz || "Obj"
            if(!clz) {
                throw new Error("missing class name")
            }

            if(typeof clz=='string'){
                if(typeof (Widgets as any)[clz]!='function'){
                    throw new Error("unknow class name: "+clz)
                }
                clz = (Widgets as any)[clz]
            }
            if( typeof clz!="function" ){
                throw new Error("invalid lv obj class: "+(typeof clz))
            }

            var obj = new clz(parent, ...(json.args || []))
            obj.fromJson(json,refs)
            return obj
        }
    }catch(e:any){
        console.error(e)
    }
}

;(Obj as any).prototype.fromJson = function (json: any, refs?: any): any{
    if(!refs)
        refs={}
    try{
        if(json instanceof Array) {
            fromJson(json, this, refs)
        }
        else {

            this.id = json.id || ''
            this.name = json.name || ''

            if(json.clear) {
                this.removeStyleAll()
            }

            for(let propName in json) {
                if((MapPropFuncs as any)[propName]) {
                    let methodName = (MapPropFuncs as any)[propName]
                    if(typeof this[methodName]=='function') {
                        this[methodName]( json[propName] )
                        continue
                    }
                }
                if((MapEvents as any)[propName] && typeof json[propName]=="function") {
                    this.on((MapEvents as any)[propName], json[propName])
                    if((MapPressEvents as any)[propName]) {
                        this.addFlag("clickable")
                    }
                    continue
                }
                let setter = 'set' + propName[0] + propName.substr(1)
                if(typeof this[setter]=='function') {
                    this[setter](json[propName])
                    continue
                }
            }

            if(json.style) {
                for(let propName in json.style) {
                    this.setStyle(propName, json.style[propName])
                }
            }

            if(json.flag) {
                for(let flag of json.flag) {
                    this.addFlag(flag)
                }
            }
            if(json.state) {
                for(let state of json.state) {
                    this.addState(state)
                }
            }

            if(json.bubble) {
                this.addFlag("event-bubble")
            }
            
            if(json.props && typeof json.props=="object") {
                for(let name in json.props) {
                    this[name] = json.props[name]
                }
            }

            if(json.visible!=undefined){
                if(json.visible) {
                    this.show()
                } else {
                    this.hide()
                }
            }

            if(json.children) {
                fromJson(json.children, this, refs)
            }

            if(json.ref) {
                refs[json.ref] = this
            }
        }
        
        this.updateLayout()

    }catch(e:any){
        console.error(e)
    }

    return refs
}

const arrStyleAsProp = ['x', 'y', 'width', 'height']
;(Obj as any).prototype.toJson = function (): any {
    let json: any = {
        class: this.constructor.name ,
        id: this.id ,
        name: this.name ,
        visible: this.isVisible() ,
    }
    let parent: Obj = this.parent()
    if(parent && parent.id) {
        json.parent = parent.id
    }
    
    let style = this.localStyle()
    if(style) {
        let styleJson:any = {}
        let props = style.props()
        for(let propName of props) {
            if("unknow"==propName) {
                continue
            }
            if( arrStyleAsProp.includes(propName) ) {
                json[propName] = style.get(propName)
            }
            else {
                styleJson[propName] = style.get(propName)
            }
        }
        if(Object.keys(styleJson).length) {
            json.style = styleJson
        }
    }
    return json
}

;(Obj as any).prototype.asRow = function asRow() {
    this.removeStyleAll()
    this.setFlexFlow("row")
}
;(Obj as any).prototype.asColumn = function asColumn() {
    this.removeStyleAll()
    this.setFlexFlow("column")
}



export class CleanObj extends Obj {
    constructor(parent: Obj|null, ptr=0) {
        super(parent, ptr)
        // this.removeStyleAll()
        this.setStyle("pad-top", 0)
        this.setStyle("pad-bottom", 0)
        this.setStyle("pad-left", 0)
        this.setStyle("pad-right", 0)
        this.setStyle("border-width", 0)
        this.setStyle("radius", 0)
        this.setStyle("bg-opa", 0)
        this.clearFlag("scrollable")
    }
}

export class Row extends Obj {
    constructor(parent: Obj|null, ptr=0) {
        super(parent,ptr)
        this.asRow()
        this.setWidth("100%")
        this.setHeight(-1)
    }
}

export class Column extends Obj {
    constructor(parent: Obj|null, ptr=0) {
        super(parent,ptr)
        this.asColumn()
        this.setWidth(-1)
        this.setHeight("100%")
    }
}

; (Keyboard as any).prototype.popup = function(textarea: TextArea, cb?:(obj:Obj,event:"ready"|"cancel")=>void|false) {
    if( !this._doneCb ){
        this._doneCb = (event:"ready"|"cancel")=>{
            if(!this._popupCb){
                this.hide()
                return
            }
            if(this._popupCb(this, event)!==false) {
                this.hide()
            }
            this._popupCb = null
        }
        this.on("ready", this._doneCb)
        this.on("cancel", this._doneCb)
    }
    if(textarea) {
        this.setTextarea(textarea)
    }
    this._popupCb = cb || null
    this.show()
    return this
}



export declare interface Obj {
    asRow():void ;
    asColumn():void ;
    show():void ;
    hide():void ;
    toggle():void ;
    localStyle(): Style|null ;
    draggable(onstart?:DragStartCB|null, ondragging?:DraggingCB|null, onstop?: DragStopCB|null): Draggable ;
    setStyle(styleName:string, value: string|number|Color, selector?:number): boolean ;
    style(styleName:string, selector?:number): string|number|Color ;
    fromJson (json: any, refs?: any): any ;
    toJson(): any ;
}
export declare interface Keyboard {
    popup(textarea: Obj, cb?:(obj:Obj,event:"ready"|"cancel")=>void|false):void ;
}

export class Color {
    public r = 0
    public g = 0
    public b = 0
    constructor(r=0,g=0,b=0) {
        this.r = (r||0) & 255
        this.g = (g||0) & 255
        this.b = (b||0) & 255
    }
    
    // high byte [ 3:G(1-3) | 5:B ] 
    // low byte  [ 5:R | 3:G(4-6) ]
    toRGB565() {
        let r = Math.round((this.r&255)*31/255)
        let g = Math.round((this.g&255)*63/255)
        let b = Math.round((this.b&255)*31/255)
        let v = b << 8
        v|= (g>>3)&0b111
        v|= (g&0b111) << 13
        v|= r << 3
        return  v
    }

    toHex() {
        let r = (this.r&255).toString(16).toUpperCase()
        if(r.length<2)
            r = '0'+r
        let g = (this.g&255).toString(16).toUpperCase()
        if(g.length<2)
            g = '0'+g
        let b = (this.b&255).toString(16).toUpperCase()
        if(b.length<2)
            b = '0'+b
        return '#'+r+g+b
    }

    public static fromHex (val:string): Color{
        val = val.trim().replace(/^#/,'')
        let color = new Color
        if(val.length<3) {
            color.r = parseInt(val,16)
            color.g = color.r
            color.b = color.r
        }
        else{
            color.r = parseInt(val.substr(0,2),16)
            color.g = parseInt(val.substr(2,2),16)
            color.b = parseInt(val.substr(4,2),16)
        }
        return color
    }

    public static fromRGB565 (val:number) : Color {
        let lb = val & 255
        let hb = (val >> 8) & 255
        let r = (lb>>3) & 255
        let g = ((lb&7)<<3) | hb>>5
        let b = hb & 31
        return new Color(
            Math.round(r * 255/31) ,
            Math.round(g * 255/63) ,
            Math.round(b * 255/31) 
        )
    }
}

export function palette(colorName:string, level=4) {
    let color = Module.ccall("lv_palette_color","number",["string","number"],[colorName,level])
    return Color.fromRGB565(color)
}
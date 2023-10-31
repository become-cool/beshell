const lv = require('lv')

const PortSnapDis = 10



class Input extends lv.TextArea {
    numeric = false
    constructor(parent) {
        super(parent)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            center: true ,
            oneLine: true ,
            style: {
                // radius: lv.RADIUS_CIRCLE ,
                "pad-top": 2 ,
                "pad-bottom": 2 ,
                "pad-left": 8 ,
                "pad-right": 8 ,
            } ,
            clicked: ()=>{
                lv.keyboard().popup(this).setMode(this.numeric? "number": "text-lower")
            } ,
        })
        let txt = this.child(0).as(lv.Label)
        txt.setFont("m10")
        txt.setWidth(-1)
        txt.setStyle("min-width",5)
    }
}

class CardMenu extends lv.Row{
    menu = null
    value = null
    constructor(card, menu, itemsCallback, program) {
        super(card) 
        this.menu = menu

        this.fromJson({
            width: -1 ,
            height: -1 ,
            bubble: true ,
            style: {
                "flex-cross-place": "center" ,
            } ,
            children: [
                {
                    class: lv.Label ,
                    ref: "title" ,
                    width: -1 ,
                    height: -1 ,
                    font: "m12" ,
                    text: menu.activeTitle() || '' ,
                    style: {
                        "min-width": 20 ,
                        "text-align": "center" ,
                    }
                } , 
                {
                    class: lv.Label ,
                    width: -1 ,
                    height: 17 ,
                    font: "m10" ,
                    text: lv.symbol.down ,
                    style: {
                        "text-color": lv.rgb(220) ,
                    } ,
                    clicked: ()=>{
                        if(this.menu){
                            if(itemsCallback) {
                                let items = itemsCallback(program)
                                if(!(items instanceof Array)) {
                                    return
                                }
                                this.menu.clean()
                                this.menu.fromItemJson(items)
                            }
                            if(!this.hitTest()) {
                                return
                            }
                            this.menu.setActive(this.value)
                            this.menu.popup(null, null, (value)=>{
                                this.setValue(value)
                            })
                        }
                    }
                }
            ] ,
        }, this)

        this.setValue(menu.value)
    }
    setValue(value) {
        if(this.value==value) {
            return
        }
        this.emit("value-change", value, this.value)
        this.value = value
        let title = this.menu?.findItem(value)?.text()
        this.title.setText(title||value)
    }
}


const SlotMinWidth = 24
const SlotMinHeight = 16

class CardSlotExpression extends lv.CleanObj {

    expr = null
    input = null

    constructor(card, noInput) {
        super(card)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "min-width": SlotMinWidth ,
                "min-height": SlotMinHeight ,
            }
        })
        this.appearance()
        if(!noInput) {
            this.input = new Input(this)
        }
    }
    appearance() {
        this.fromJson({
            style: {
                radius: lv.RADIUS_CIRCLE ,
                "border-width": 1 ,
                "border-color": lv.palette("grey") ,
                "border-opa": 180 ,
                "bg-color": lv.palette("grey") ,
                "bg-opa": 60 ,
            }
        })
    }
    program() {
        const Program = require("../Program")
        for(let parent = this.parent(); parent; parent=parent.parent()){
            if(parent instanceof Program) {
                return parent
            }
        }
    }

    insert(expr) {
        expr.setParent(this)
        expr.center()
        this.expr = expr
        if(this.input)
            this.input.hide()
    }
    clean() {
        if(!this.expr) {
            return
        }
        this.expr.setParent( this.program() )
        this.expr = null
        if(this.input)
            this.input.show()
    }

    // extend 用于拖出时扩大范围，避免在边界上抖动
    _testInsert(ex,ey, extend) {
        let [x,y] = this.coords()
        if(ey<y-extend)
            return false
        // 按空 slot 计算
        if(ey>y+SlotMinHeight+extend)
            return false
        if(ex<x+SlotMinHeight/2-extend || ex>x+SlotMinWidth-SlotMinHeight/2+extend) 
            return false
        return true
    }

    evaluate() {
        if(this.expr) {
            return this.expr.evaluate()
        }
        if(this.input) {
            return this.input.text()
        }
        return null
    }
    generate() {
        if(this.expr) {
            return this.expr.generate()
        }
        else if (this.input) {
            return this.input.text() || ""
        }
        else {
            return 'undefined'
        }
    }
}


const slotDsc = new lv.DrawRectDsc()
slotDsc.setBgColor(lv.palette("grey"))
slotDsc.setBgOpa(80)

const slotBorderDsc = new lv.DrawLineDsc()
slotBorderDsc.setColor(lv.palette("grey"))
slotBorderDsc.setWidth(1)
slotBorderDsc.setOpa(180)

class CardSlotCompare extends CardSlotExpression {
    constructor(card) {
        super(card, true)
    }
    appearance() {
        this.setStyle("min-width", 30)
        this.on("draw-main", (e, target, clip)=>{
            if(this.expr) {
                return
            }
            let h = this.height()
            let half = h/2
            h-=1
            let w = this.width() - 1
            let [x,y] = this.coords()

            let points = [
                [x, y+half] ,
                [x+half, y] ,
                [x+w-half, y] ,
                [x+w, y+half] ,
                [x+w-half, y+h] ,
                [x+half, y+h] ,
                [x, y+half] ,
            ]
            lv.drawPolygon(points, clip, slotDsc)
            lv.drawLine(points[0],points[1], clip, slotBorderDsc)
            lv.drawLine(points[1],points[2], clip, slotBorderDsc)
            lv.drawLine(points[2],points[3], clip, slotBorderDsc)
            lv.drawLine(points[3],points[4], clip, slotBorderDsc)
            lv.drawLine(points[4],points[5], clip, slotBorderDsc)
            lv.drawLine(points[5],points[0], clip, slotBorderDsc)
        })
    }
    generate() {
        return this.expr? this.expr.generate(): 'false'
    }
}


class CardExpressionBase extends lv.Row {
    slots = {}
    program = null
    constructor(parent, program) {
        super(parent)
        this.program = program

        this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "pad-left":2 ,
                "pad-right":2 ,
                "flex-cross-place": "center" ,
                "pad-column": 5 ,
                "min-width": 60 ,
                "min-height": 16 ,
            } ,
        })
    }
    
    addLabel(text, name) {
        let label = new lv.Label(this, {
            text ,
            width: -1 ,
            height: -1 ,
            font: "m10" ,
            style: {
                "text-color": lv.rgb(220,220,220)
            }
        })
        if(name) {
            this.slots[name] = label
        }
        return label
    }
    addMenu(menu, name, itemsCallback) {
        let obj = new CardMenu(this, menu, itemsCallback, this.program)
        if(name) {
            this.slots[name] = obj
        }
        return obj
    }

    addSlot(name, noInput) {
        let slot = new CardSlotExpression(this, noInput)
        this.slots[name] = slot
        return slot
    }
    addCompareSlot(name) {
        let slot = new CardSlotCompare(this)
        this.slots[name] = slot
        return slot
    }

    hittest() {
        return false
    }

    evaluate() {
        return null
    }

    serialize(json) {
        let slotjson = {}
        for(let name in this.slots) {
            let slot = this.slots[name]
            if( slot instanceof CardSlotExpression ) {
                let obj = {
                    expr: undefined, input: undefined
                }
                if(slot.expr) {
                    obj.expr = slot.expr.uuid
                }
                if(slot.input) {
                    let text = slot.input.text()
                    if(text) {
                        obj.input = text
                    }
                }
                if(obj.expr!=undefined || obj.input!=undefined) {
                    slotjson[name] = obj
                }
            }
            else if( slot instanceof CardMenu ){
                slotjson[name] = slot.value
            }
            else if( slot instanceof lv.Label ){
                slotjson[name] = slot.text()
            }
        }
        if(Object.keys(slotjson).length) {
            json.slots = slotjson
        }
        return json
    }
    unserialize(json, vm) {
        if(json.slots) {
            for(let name in this.slots) {
                if(json.slots[name]==undefined){
                    continue
                }
                let slot = this.slots[name]
                if( slot instanceof CardSlotExpression ) {
                    if(json.slots[name].input!=undefined) {
                        this.slots[name].input.setText(json.slots[name].input)
                    }
                    if(json.slots[name].expr!=undefined) {

                        let uuid = json.slots[name].expr
                        if(!vm.cards[uuid]){
                            throw new Error("unknow uuid of card: "+uuid)
                        }
                        this.slots[name].insert(vm.cards[uuid])
                    }
                }
                else if( slot instanceof CardMenu ){
                    slot.setValue(json.slots[name])
                }
                else if( slot instanceof lv.Label ){
                    slot.setText(json.slots[name])
                }
            }
        }
    }
}

class CardExpression extends CardExpressionBase {

    id = null

    _matchSlotClz = CardSlotExpression
    
    constructor(parent, program) {
        super(parent, program)

        this.draggable()

        this.appearance()

        // 卡片堆里的卡片 program == null
        if(program) {
            let draggable = this.draggable()
            draggable.setStart(()=>{
                this.moveForeground()
            })
            draggable.setDragging(pos=>{
                let x = pos.x 
                let y = pos.y + this.height()/2
                let parent = this.parent()
                if( parent instanceof this._matchSlotClz ){
                    if(!parent._testInsert(x, y, 10)) {
                        parent.clean()
                        this.setCoords(pos.x, pos.y)
                        this.updateLayout()
                    }
                    else {
                        pos.x = pos.y = false
                    }
                    return
                }
                for(let uuid in program.model.vm.cards) {
                    let statement = program.model.vm.cards[uuid]
                    if(statement==this) {
                        continue
                    }
                    let slots = statement.slots || statement.expr.slots
                    for(let name in slots) {
                        let slot = slots[name]
                        if( !(slot instanceof this._matchSlotClz) || slot.expr )
                            continue
                        if(!slot._testInsert(x, y, 0)) {
                            continue
                        }
                        slot.insert(this)
                        pos.x = pos.y = false
                        return
                    }
                }
            })
            
        }
    }

    bgColor() {
        return lv.palette("orange")
    }

    appearance() {
        this.fromJson({
            style: {
                // "border-width": 1 ,
                // "border-color": this.borderColor ,
                "border-opa": 180 ,
                "bg-color": this.bgColor() ,
                "bg-opa": 255 ,
                "radius": lv.RADIUS_CIRCLE ,
            }
        })
    }
    
    generate() {
        console.log(this.constructor.name+".generate() not implemented")
        return ''
    }
    
    isTop() {
        return this.parent() == this.program
    }
    
    serialize() {
        let json = {
            class: this.constructor.name ,
        }
        if(this.isTop()) {
            json.x = this.x()
            json.y = this.y()
        }
        super.serialize(json)
        return json
    }
    unserialize(json, vm) {
        if(json.x!=undefined) {
            this.setX(json.x)
        }
        if(json.y!=undefined) {
            this.setY(json.y)
        }
        this.updateLayout()
        super.unserialize(json, vm)
    }
}

class CardCompare extends CardExpression{
    constructor(parent, program) {
        super(parent, program)
        this._matchSlotClz = CardSlotCompare
    }
    appearance() {
        this.fromJson({
            style: {
                "pad-left": 8 ,
                "pad-right": 8 ,
                "pad-top": 2 ,
                "pad-bottom": 2 ,
            }
        })
        this.on("draw-main", (e, target, clip)=>{
            let h = this.height()
            let half = h/2
            let w = this.width() - 1
            
            let path = new lv.Path()
            path.begin(0,half)
            path.lineTo(half,0)
            path.lineTo(w-half,0)
            path.lineTo(w,half)
            path.lineTo(w-half,h)
            path.lineTo(half,h)
            path.end()
            path.fill( ...this.coords(), 1, this.bgColor(), clip)
        })
    }
}


const bodyDsc = new lv.DrawRectDsc()
bodyDsc.setBgColor(lv.palette("purple"))
bodyDsc.setBgOpa(255)
bodyDsc.setRadius(2)

const SPortSlopeW = 3
const SPortX1 = 6
const SPortX2 = SPortX1 + SPortSlopeW
const SPortX2_5 = SPortX2 + 5
const SPortX3 = SPortX2 + 10
const SPortX4 = SPortX3 + SPortSlopeW
const SPortSlopeH = 5
const InnerPad = 8

/*
      x1       x4
    .____     _______________________
    |    \___/                       |
    |    x2  x3                      |
    |____     _______________________|
         \___/
*/

class CardStatement extends lv.Column{

    id = null

    prev = null
    next = null

    expr = null
    program = null

    constructor(parent, program) {
        super(parent)

        this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "pad-row":InnerPad ,
                "pad-left":InnerPad ,
                "pad-right":InnerPad ,
                "pad-top":SPortSlopeH + 2 ,
                "pad-bottom":SPortSlopeH + 3 ,
                "min-width": 60 ,
                "min-height": 30 ,
                // "border-width": 1 ,
            } ,
        })

        this.expr = new CardExpressionBase(this, program)
        this.expr.addFlag("event-bubble")

        this.appearance()
        
        // 卡片堆里的卡片 program == null
        if(program) {

            this.program = program
    
            let draggable = this.draggable()
            draggable.setStart(()=>{
                this.moveForeground()
                draggable.setFollowers(this.followers())
            })
            draggable.setDragging((pos)=>{    
                let linked = this.linkedStatement()
                if(linked) {
                    let [x,y] = this.coords()
                    if( Math.abs(x-pos.x) < PortSnapDis && Math.abs(y-pos.y) < PortSnapDis ) {
                        pos.x = pos.y = false
                        return
                    }
                    this.tearDown()
                }
    
                let [px, py] = this.coordsPortPrev(pos.x, pos.y)
                for(let uuid in program.model.vm.cards) {
                    let statement = program.model.vm.cards[uuid]
                    if(statement==this || !(statement instanceof CardStatement) ) {
                        continue
                    }
                    if( statement._tryLinkin(this, pos, px, py) ) {
                        return
                    }
                }
            })
            draggable.setStop(()=>{
                draggable.setFollowers(null)
            })
    
            // 当内部插入卡片，或其他原因导致高度发生变化，顺序移动后续开篇
            let requireMoveNext = false 
            this.on("size-changed", ()=>{
                if(!this.next || requireMoveNext)
                    return
                requireMoveNext = true
                setTimeout(()=>{
                    for(let next=this.next, prev=this; next; prev=next, next=next.next) {
                        prev._placeNext(next)
                        next.updateLayout()
                    }
                    requireMoveNext = false
                },0)
            })
        }
    }

    bgColor() {
        return lv.palette("orange")
    }
    appearance() {
        this.on("draw-main",(e, target, clip)=>{
            let [x,y] = this.coords()
            let h = this.height()
            let w = this.width()
            bodyDsc.setBgColor(this.bgColor())
            lv.drawPolygon([
                [x,y] ,
                [x+SPortX1,y] ,
                [x+SPortX2,y+SPortSlopeH] ,
                [x+SPortX2,y+h-SPortSlopeH] ,
                [x,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)

            lv.drawPolygon([
                [x+SPortX3,y+SPortSlopeH] ,
                [x+SPortX4,y] ,
                [x+w,y] ,
                [x+w,y+h-SPortSlopeH] ,
                [x+SPortX3,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)
            
            lv.drawPolygon([
                [x+SPortX2,y+SPortSlopeH] ,
                [x+SPortX3,y+SPortSlopeH] ,
                [x+SPortX4,y+h-SPortSlopeH] ,
                [x+SPortX3,y+h] ,
                [x+SPortX2,y+h] ,
                [x+SPortX1,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)
        })
    }

    // px, py 为 statement up port 的坐标(横线中点)
    _testAppend(px,py) {
        let [x,y] = this.coordsPortNext()
        return Math.abs(x-px) < PortSnapDis && Math.abs(y-py) < PortSnapDis
    }
    
    linkedStatement() {
        if(this.prev) {
            return this.prev
        }
        let parent = this.parent()
        if(parent instanceof CardStatementBody) {
            return parent
        }
        return null
    }
    tearDown() {
        if(this.prev) {
            this.prev.next = null
            this.prev = null
        }
        let parent = this.parent()
        if(parent instanceof CardStatementBody) {
            this.setParent(this.program)
            this.updateLayout()

            for(let next=this.next;next;next=next.next) {
                next.setParent(this.program)
            }
            if(parent.first==this) {
                parent.first = null
            }
        }
        
    }
    // 如果位置合适，将 statement 插入自己的后面
    // px, py : port 位置
    _tryLinkin(statement, pos, px, py) {
        if( this.next || !this._testAppend(px, py)) {
            return false
        }
        this.append(statement)
        this._placeNext(null, pos)
        return true
    }

    coordsPortPrev(x, y) {
        if(x==undefined || y==undefined){
            [x, y] = this.coords()
        }
        return [x+SPortX2_5, y+SPortSlopeH]
    }
    coordsPortNext(x, y) {
        if(x==undefined || y==undefined){
            [x, y] = this.coords()
        }
        return [x+SPortX2_5, y+this.height()]
    }

    last() {
        if(this.next) {
            return this.next.last()
        }
        return this
    }

    append(statement) {
        if(statement==this) {
            throw new Error("append() self ??")
        }
        if(this.next) {
            statement.last().append(this.next)
            return
        }
        statement.prev = this
        this.next = statement
        
        let parent = this.parent()
        if(statement.parent()!=parent) {
            statement.setParent(parent)
            statement.updateLayout()
        }
    }

    _placeNext(statement, out, x, y) {
        if(x==undefined) {
            ([x,y] = this.coords())
        }
        if(statement) {
            statement.setCoords(x, y+this.height()-SPortSlopeH)
        }
        if(out) {
            out.x = x
            out.y = y+this.height()-SPortSlopeH
        }
    }

    followers() {
        let lst = []
        for(let widget=this.next; widget; widget=widget.next) {
            lst.push(widget)
        }
        return lst ;
    }
    run() {
        this.runNext()
    }
    runNext() {
        if(this.next) {
            this.next.run()
        }
    }
    generate(indent) {
        console.log(this.constructor.name+".generate() not implemented")
        return ''
    }
    generateQueue(indent) {
        let code = ''
        for(let statement=this; statement; statement=statement.next) {
            if(code) {
                code+= "\r\n"
            }
            code+= statement.generate(indent)
        }
        return code
    }
    isTop() {
        return this.parent()==this.program && !this.prev
    }
    serialize() {
        let json = {
            class: this.constructor.name ,
        }
        this.expr.serialize(json)

        if(this.isTop()) {
            json.x = this.x()
            json.y = this.y()

            let stack = []
            for(let next=this.next;next;next=next.next) {
                stack.push(next.uuid)
            }
            if(stack.length) {
                json.stack = stack
            }
        }

        return json
    }
    unserialize(json, vm) {
        if(json.x!=undefined) {
            this.setX(json.x)
        }
        if(json.y!=undefined) {
            this.setY(json.y)
        }
        this.expr.unserialize(json, vm)
        this.updateLayout()

        if(json.stack) {
            let last = this
            for(let uuid of json.stack) {
                let card = vm.cards[uuid]
                if(!card) {
                    throw new Error("unknow card uuid: "+uuid)
                }
                last._placeNext(card)
                card.updateLayout()
                last.next = card
                card.prev = last
                last = card
            }
        }
    }
}

const ArcWidth = 30
const ArcHeight = SPortSlopeH
const ArcCX = ArcWidth/2
const ArcCY = 25
class CardEvent extends CardStatement{
    constructor(parent, program){
        super(parent, program)
        if(program) {
            this.draggable().setDragging(null)
        }
    }
    appearance() {
        this.on("draw-main", (e, target, clip)=>{
            let width = this.width() - 1 ;
            let height = this.height() - 1 ;
            let path = new lv.Path()
            path.begin(0, ArcHeight)
            path.arcTo( ArcWidth, ArcHeight, ArcCX, ArcCY, true)
            path.lineTo(width, ArcHeight)
            path.lineTo(width, height-SPortSlopeH)
            path.lineTo(SPortX4, height-SPortSlopeH)
            path.lineTo(SPortX3, height)
            path.lineTo(SPortX2, height)
            path.lineTo(SPortX1, height-SPortSlopeH)
            path.lineTo(0, height-SPortSlopeH)
            path.end()
            path.fill( ...this.coords(), 1, this.bgColor(), clip)
        })
    }
}

class CardStatementBody extends lv.CleanObj {
    first = null
    constructor(statement){
        super(statement)
    }
    run() {
        if(this.first) {
            this.first.run()
        }
    }
}

class CardControl extends CardStatement{

    proc1 = null
    proc2 = null

    constructor(parent, program){
        super(parent, program)
        this.setStyle("pad-bottom", SPortSlopeH + 11)
        this.proc1 = this.createProcess()

        // this.expr.setStyle("border-width",1)
    }

    _innerCeil(proc) {
        let [,y] = this.coords()
        let [,_y] = proc.coords()
        return proc.y() + InnerPad 
    }
    _innerFloor(proc) {
        return proc.y() + proc.height() + InnerPad - SPortSlopeH - 1
    }

    appearance() {
        this.on("draw-main", (e, target, clip)=>{
            let width = this.width() - 1 ;
            let y1 = this._innerCeil(this.proc1)
            let y2 = this._innerFloor(this.proc1)
            let height = this.height() - 1 ;
            let path = new lv.Path()
            path.begin()
            
            path.lineTo(SPortX1, 0)
            path.lineTo(SPortX2, SPortSlopeH)
            path.lineTo(SPortX3, SPortSlopeH)
            path.lineTo(SPortX4, 0)
            path.lineTo(width, 0)
            
            path.lineTo(width, y1)
            path.lineTo(SPortX4+InnerPad, y1)
            path.lineTo(SPortX3+InnerPad, SPortSlopeH + y1)
            path.lineTo(SPortX2+InnerPad, SPortSlopeH + y1)
            path.lineTo(SPortX1+InnerPad, y1)
            path.lineTo(InnerPad, y1)

            path.lineTo(InnerPad, y2)
            path.lineTo(SPortX1+InnerPad, y2)
            path.lineTo(SPortX2+InnerPad, SPortSlopeH + y2)
            path.lineTo(SPortX3+InnerPad, SPortSlopeH + y2)
            path.lineTo(SPortX4+InnerPad, y2)
            path.lineTo(width, y2)

            if(this.proc2) {
                let y3 = this._innerCeil(this.proc2)
                let y4 = this._innerFloor(this.proc2)
                
                path.lineTo(width, y3)
                path.lineTo(SPortX4+InnerPad, y3)
                path.lineTo(SPortX3+InnerPad, SPortSlopeH + y3)
                path.lineTo(SPortX2+InnerPad, SPortSlopeH + y3)
                path.lineTo(SPortX1+InnerPad, y3)
                path.lineTo(InnerPad, y3)
                
                path.lineTo(InnerPad, y4)
                path.lineTo(SPortX1+InnerPad, y4)
                path.lineTo(SPortX2+InnerPad, SPortSlopeH + y4)
                path.lineTo(SPortX3+InnerPad, SPortSlopeH + y4)
                path.lineTo(SPortX4+InnerPad, y4)
                path.lineTo(width, y4)
            }

            path.lineTo(width, height-SPortSlopeH)
            path.lineTo(SPortX4, height-SPortSlopeH)
            path.lineTo(SPortX3, height)
            path.lineTo(SPortX2, height)
            path.lineTo(SPortX1, height-SPortSlopeH)
            path.lineTo(0, height-SPortSlopeH)
            path.end()
            path.fill( ...this.coords(), 1, this.bgColor(), clip)
        })
    }
    createProcess() {
        let proc = new CardStatementBody(this)
        proc.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "min-width": 20 ,
                "min-height": 20 ,
                // "border-width": 1 ,
            }
        })
        return proc ;
    }

    // 如果位置合适，将 statement 插入自己的后面或里面
    // px, py : port 位置
    _tryLinkin(statement, pos, px, py) {
        if( super._tryLinkin(statement, pos, px, py) ) {
            return true
        }
        if(this._tryLinkinInner(this.proc1, statement, pos, px, py)){
            this.proc1.first = statement
            return true
        }
        if(this._tryLinkinInner(this.proc2, statement, pos, px, py)){
            this.proc2.first = statement
            return true
        }
        return false
    }
    
    _tryLinkinInner(proc, statement, pos, px, py) {
        if( proc && !proc.childCnt() ){
            let [x, y] = this.coords()
            x+= InnerPad + SPortX2_5
            y+= this._innerCeil(proc) + SPortSlopeH
            if( Math.abs(x-px) < PortSnapDis && Math.abs(y-py) < PortSnapDis ) {
                statement.setParent(proc)
                statement.setX(0)
                statement.setY(0)
                statement.updateLayout()

                for(let next=statement.next; next; next=next.next) {
                    next.setParent(proc)
                    next.updateLayout()
                    next.prev._placeNext(next)
                    proc.first = next
                }
                
                pos.x = pos.y = false
                return true
            }
        }
    }

    _serializeProc(json, proc, name) {
        let stack = []
        for(let next=proc?.first; next; next=next.next) {
            stack.push(next.uuid)
        }
        if(stack.length) {
            json[name] = stack
        }
    }

    serialize() {
        let json = super.serialize()
        this._serializeProc(json, this.proc1, "proc1")
        this._serializeProc(json, this.proc2, "proc2")

        return json
    }
    unserializeStack(proc, lstjson, vm) {
        if( lstjson instanceof Array ) {
            let last = null
            for(let uuid of lstjson) {
                let card = vm.cards[uuid]
                if(!card) {
                    throw new Error("unknow card uuid: "+uuid)
                }
                card.setParent(proc)
                if(last) {
                    last.next = card
                    card.prev = last
                }
                if(!proc.first) {
                    proc.first = card
                }
                last = card
            }
        }
    }
    unserialize(json, vm) {
        super.unserialize(json, vm)
        this.unserializeStack(this.proc1, json.proc1, vm)
        this.unserializeStack(this.proc2, json.proc2, vm)
    }
}


const mapShredMenu = {}
exports.shareMenu = function(name, items) {
    if(!mapShredMenu[name]) {
        mapShredMenu[name] = new lv.Menu(null, {items})
        if((items instanceof Array) && items.length) {
            mapShredMenu[name].setActive(items[0])
        }
    }
    return mapShredMenu[name]
}

exports.CardStatement = CardStatement
exports.CardEvent = CardEvent
exports.CardControl = CardControl
exports.CardExpression = CardExpression
exports.CardCompare = CardCompare
const lv = require('lv')

class HelperHandler extends lv.CleanObj{
    
    constructor(helper, dirx, diry) {
        super(helper)
        this.fromJson({
            width: 10 ,
            height: 10 ,
            style: {
                "border-width": 1 ,
                "border-color": lv.palette("grey") ,
                "border-opa": 180 ,
            }
        })
        let lx , ly
        this.draggable(
            ()=>{
                if(!helper.target)
                    return false
                let [x,y] = this.coords()
                lx=x; ly=y;
            } ,
            pos=>{
                if(!helper.target)
                    return false
                if(pos.x==lx && pos.y==ly) {
                    return
                }
                if(dirx) {
                    if(dirx<0) {
                        helper.setTarget('setX', pos.x)
                    }
                    let dx = pos.x-lx
                    let w = helper.target.width() + dx*dirx
                    helper.setTarget('setWidth', w)

                    lx = pos.x
                }
                else {
                    pos.x = false
                }

                if(diry) {
                    if(diry<0) {
                        helper.setTarget('setY', pos.y)
                    }
                    let dy = pos.y-ly
                    let h = helper.target.height() + dy*diry
                    helper.setTarget('setHeight', h)

                    ly = pos.y
                }
                else {
                    pos.y = false
                }
            }
        )
    }
}

class WidgetHelper extends lv.CleanObj{

    target = null

    constructor(parent) {
        super(parent)
        global.helper = this

        this.clearFlag("clickable")

        this.fromJson({
            visible : false ,
            style: {} ,
            children: [
                {
                    class: HelperHandler ,
                    ref: "lfttop" ,
                    align: "top-left" ,
                    args: [-1, -1]
                } ,
                {
                    class: HelperHandler ,
                    ref: "top" ,
                    align: "top-mid" ,
                    args: [0, -1]
                } ,
                {
                    class: HelperHandler ,
                    ref: "rgttop" ,
                    align: "top-right" ,
                    args: [1, -1]
                } ,
                {
                    class: HelperHandler ,
                    ref: "lft" ,
                    align: "left-mid" ,
                    args: [-1, 0]
                } ,
                {
                    class: HelperHandler ,
                    ref: "rgt" ,
                    align: "right-mid" ,
                    args: [1, 0]
                } ,
                {
                    class: HelperHandler ,
                    ref: "lftbtm" ,
                    align: "bottom-left" ,
                    args: [-1, 1]
                } ,
                {
                    class: HelperHandler ,
                    ref: "btm" ,
                    align: "bottom-mid" ,
                    args: [0, 1]
                } ,
                {
                    class: HelperHandler ,
                    ref: "rgtbtm" ,
                    align: "bottom-right" ,
                    args: [1, 1]
                } ,
            ]
        }, this)
    }

    attach(widget) {
        if(this.target) {
            if(this.target.shadow) {
                this.target.shadow.removeStyle(WidgetShadow.styleActive)
            }
        }
        this.setWidth(widget.width())
        this.setHeight(widget.height())
        this.setCoords( ... widget.coords() )
        this.target = widget
        
        if(widget.shadow) {
            widget.shadow.addStyle(WidgetShadow.styleActive)
        }

        this.show()
    }

    disattach() {
        if(this.target) {
            if(this.target.shadow) {
                this.target.shadow.removeStyle(WidgetShadow.styleActive)
            }
        }
        this.hide()
    }

    setTarget(setter, v) {
        this.target[setter](v)
        this.target.shadow[setter](v)
        this[setter](v)
        
        this.target.updateLayout()
        this.target.shadow.updateLayout()
        this.updateLayout()
    }
}


class WidgetShadow extends lv.CleanObj{
    target = null
    constructor(widget, ui) {
        super(ui.shadow)
        this.target = widget
        widget.updateLayout()
        this.fromJson({
            width: widget.width() ,
            height: widget.height() ,
            style: {
                // "bg-color": lv.palette("blue") ,
                // "bg-opa": 100 ,
            } ,
            clicked: ()=>{
                console.log("shadow clicked")
                ui.setActiveWidget(widget)
            }
        })

        this.setCoords(... widget.coords())
        this.updateLayout()

        this.draggable(null, pos=>{
            widget.setCoords(pos.x, pos.y)
            if(ui.activeWidget==widget){
                ui.helper.setCoords(pos.x, pos.y)
            }
        })
    }
    setCoords(x, y) {
        super.setCoords(x,y)
        this.target.setCoords(x,y)
    }
    
    static styleActive = new lv.Style({
        "border-width": 1 ,
        "border-color": lv.palette("yellow") ,
        "border-opa": 100 ,
        "shadow-color": lv.palette("yellow") ,
        "shadow-spread": 1 ,
        "shadow-width": 1 ,
    })
}


exports.WidgetHelper = WidgetHelper
exports.WidgetShadow = WidgetShadow
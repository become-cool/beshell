const lv = require('lv')
const UITools = require('./UITools')
const {WidgetHelper, WidgetShadow} = require('./WidgetHelper')

class UI extends lv.CleanObj{

    tools = null
    activeWidget = null
    workspace = null

    constructor(workspace, ) {
        super(workspace)
        this.workspace = workspace

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: [
                {
                    class: 'Label' ,
                    center: true ,
                    text: '应用还没有UI元素' ,
                    ref: 'holdspace' ,
                    font: 'msyh' ,
                    style: {
                        "text-color": lv.rgb(200,200,200)
                    }
                } ,
                {
                    class: lv.CleanObj ,
                    width: "100%" ,
                    height: "100%" ,
                    ref: "root" ,
                    clicked: () => {
                        this.setActiveWidget(null)
                    }
                } ,
                {
                    class: lv.CleanObj ,
                    ref: "shadow",
                    width: "100%" ,
                    height: "100%" ,
                    pressed : ()=>{
                        this.setActiveWidget(null)
                    }
                } ,
                {
                    class: WidgetHelper ,
                    visible: false ,
                    ref: "helper" ,
                } ,
                 
            ] ,
        }, this)

        workspace.model.on("model.widget.new", ()=>{
            this.holdspace.hide()
        })
        workspace.model.on("model.widget.remove", ()=>{
            if(!workspace.model.widgets.length){
                this.holdspace.show()
            }
        })
    }

    createWidget(clz, json) {
        let widget = new clz(this.root)
        if(json) {
            widget.fromJson(json)
        }
        widget.name = json.ref || undefined
        this.workspace.model.addWidget(widget)
        widget.shadow = new WidgetShadow(widget, this.workspace.ui)
        return widget
    }

    setActiveWidget(widget){
        if(widget == this.activeWidget)
            return
        this.emit("ui.active.changed", widget, this.activeWidget)
        this.activeWidget = widget
        if(widget) {
            this.helper.attach(widget)
        }
        else {
            this.helper.disattach()
        }
    }
    
    viewTools(toolbar) {
        if(!this.tools) {
            this.tools = new UITools(toolbar, this.workspace)
        }
        return this.tools
    }
    
    enter() {

    }
    leave() {
        if(this.tools) {
            this.tools.widgetLib.hide()
        }
    }

    serialize() {
        let json = []
        for(let wname in this.workspace.model.widgets) {
            json.push(this.serialzeWidget(this.workspace.model.widgets[wname]))
        }
        return json
    }
    serialzeWidget(widget) {
        let json = {
            class: widget.constructor.name ,
            ref: widget.name,
        }

        if( typeof widget.text=="function") {
            json.text = widget.text()
        }

        let style = widget.localStyle()
        if(style) {
            let stylejson = {}
            let props = style.props()
            for(let propName of props) {
                if("unknow"==propName) {
                    continue
                }
                if( styleToProp.includes(propName) ) {
                    json[propName] = style.get(propName)
                }
                else {
                    stylejson[propName] = style.get(propName)
                }
            }
            if(stylejson.length) {
                json.style = stylejson
            }
        }

        return json
    }

    unserialize(json) {
        for(let wjson of json) {
            let clazz = lv[wjson.class]
            if(typeof clazz!='function') {
                throw new Error('unknow widget class: '+wjson.class)
            }
            this.createWidget(clazz, wjson)
        }
    }
}

const styleToProp = ['x', 'y', 'width', 'height']
module.exports = UI
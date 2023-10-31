const lv = require('lv')
const CardStack = require("../comm/CardStack")


const mapWidgetLib = {
    '矩形':  {
        class: lv.Obj ,
        opt: {
            width: 80 ,
            height: 80 ,
            style: {
                "radius": 2 ,
                "border-width": 1 ,
            }
        } ,
    } ,
    '文字':  {
        class: lv.Label ,
        opt: {
            text: "Text" ,
        } ,
    } ,
    '按钮':  {
        class: lv.Btn ,
        opt: {
            text: "Button" ,
        } ,
    } ,
    '直线': {
        class: lv.Line ,
        opt: {} ,
    } ,
    '圆环':  {
        class: lv.Arc ,
        opt: {
        } ,
    } ,
    '进度条':  {
        class: lv.Bar ,
        opt: {} ,
    } ,
    '选择框':  {
        class: lv.Checkbox ,
        opt: {
            text: "CheckBox" ,
        } ,
    } ,
    '下拉菜单':  {
        class: lv.Dropdown ,
        opt: {} ,
    } ,
    '图片': {
        class: lv.Img ,
        opt: {} ,
    } ,
    '滚轮': {
        class: lv.Roller ,
        opt: {} ,
    } ,
    '滑块': {
        class: lv.Slider ,
        opt: {} ,
    } ,
    '开关': {
        class: lv.Switch ,
        opt: {} ,
    } ,
    '输入框': {
        class: lv.TextArea ,
        style: {
            "text-color": lv.rgb(220,220,220)
        } ,
        opt: {
            oneLine: true ,
            placeholderText: 'Press Here' ,
            clicked() {
                lv.keyboard().popup(this)
            }
        } ,
    } ,
    '列表': {
        class: lv.List ,
        opt: {} ,
    } ,
    '日历': {
        class: lv.Obj ,
        opt: {} ,
    } ,
    '视频': {
        class: lv.Obj ,
        opt: {} ,
    } ,
}


class UITools extends lv.Column{

    widgetLib = null

    constructor(parent, workspace) {
        super(parent)

        this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-right" ,
            style: {
                "pad-row": 5 ,
                "flex-cross-place": "center" ,
            }, 
            children: [
                {
                    class: "Label" ,
                    text: lv.symbol.play ,
                    ref: "play" ,
                    font: "m12" ,
                    clicked: ()=>{
                        workspace.ui.shadow.hide()
                        workspace.toolbar.hide()
                        this.edit.setCoords(...this.play.coords())
                        this.edit.show()
                    }
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.plus ,
                    clicked: ()=>{
                        this.widgetLib.isVisible()?
                            this.widgetLib.hide() :
                            this.widgetLib.show()
                    }
                } ,
                {
                    class: "Img" ,
                    src: '/lib/icon/16/props.png' ,
                    ref: "props" ,
                    visible: false ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.trash ,
                    ref: "remove" ,
                    visible: false ,
                } ,
            ]
        }, this)

        this.edit = new lv.Label(workspace)
        this.edit.fromJson({
            font: "m12" ,
            text: lv.symbol.pause ,
            visible: false ,
            style: {
                "text-color": lv.rgb(80,80,80)
            } ,
            clicked: () => {
                workspace.ui.shadow.show()
                workspace.toolbar.show()
                this.edit.hide()
                lv.keyboard().hide()
            }
        })

        this.widgetLib = new CardStack(workspace, workspace)
        this.widgetLib.hide()
        for(let title in mapWidgetLib) {
            this.widgetLib.createCard(title, data=>{
                let widget = workspace.ui.createWidget(data.class, data.opt)
                workspace.ui.setActiveWidget(widget)
                return widget.shadow
            }, mapWidgetLib[title])
        }

        workspace.ui.on("ui.active.changed", (activePart)=>{
            if(activePart) {
                this.props.show()
                this.remove.show()
            }
            else {
                this.props.hide()
                this.remove.hide()

                this.widgetLib.hide()
            }
        })
    }
}

module.exports = UITools
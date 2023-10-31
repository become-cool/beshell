const lv = require('lv')

const CardStack = require("../comm/CardStack")
const HostPart = require('./parts/HostPart')
const BePad = require('./parts/BePad')
const {BeMotor, BeServo} = require('./parts/BeMotor')
const {BeScreen130, BeScreen096} = require('./parts/BeScreen')
const mapPartLib = {
    BePad,
    BeMotor, BeServo,
    BeScreen096, BeScreen130,
}
// this.createCard(BeMotor)
// this.createCard(BeServo)
// this.createCard(BeScreen130)
// this.createCard(BeScreen096)
// // this.createCard("LED")
// // this.createCard("RGB LED")
// // this.createCard("姿态传感器")
// // this.createCard("颜色传感器")
// // this.createCard("气压计")
// // this.createCard("红外线收发器")
// // this.createCard("超声波")
// // this.createCard("扬声器")
// // this.createCard("麦克风")
// // this.createCard("温度传感器")
// // this.createCard("湿度传感器")

class GraphTools extends lv.Column{
    
    // graph = null
    partLib = null

    constructor(parent, workspace) {
        super(parent)

        // this.workspace = workspace

        this.refs = this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-mid" ,
            style: {
                "pad-row": 5 ,
                "flex-cross-place": "center" ,
            }, 
            children: [
                {
                    class: "Label" ,
                    text: lv.symbol.plus ,
                    clicked: ()=>{
                        this.partLib.isVisible()?
                            this.partLib.hide() :
                            this.partLib.show()
                    }
                } ,
                // {
                //     class: "Img" ,
                //     src: '/lib/icon/16/tree.png' ,
                // } ,
                {
                    class: "Img" ,
                    src: '/lib/icon/16/props.png' ,
                    ref: "props" ,
                    visible: false ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.refresh ,
                    ref: "rotate" ,
                    visible: false ,
                    clicked: ()=>{
                        if(workspace.graph.activePart) {
                            workspace.graph.activePart.rotate()
                        }
                    }
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.trash ,
                    ref: "remove" ,
                    visible: false ,
                    clicked: ()=>{
                        if(!workspace.graph.activePart || !workspace.model) {
                            return
                        }
                        workspace.model.removePart(workspace.graph.activePart)
                        console.log("part ref count:", beapi.utils.varRefCnt(workspace.graph.activePart))
                        workspace.graph.activePart = null

                    }
                } ,
            ]
        })

        this.partLib = new CardStack(workspace, workspace)
        this.partLib.hide()
        for(let clzname in mapPartLib) {
            let clz = mapPartLib[clzname]
            // 忽略 HostPart
            if( HostPart.isPrototypeOf(clz) ) {
                continue
            }
            this.partLib.createCard(clz.config.title, clz=>{
                let part = workspace.model.createPart(clz)
                part.repaint(workspace.graph.zoomer.value)

                workspace.graph.setActivePart(part)
                return part
            }, clz)
        }

        
        workspace.graph.on("graph.active.changed", (activePart)=>{
            if(activePart) {
                this.refs.props.show()
                this.refs.rotate.show()
                this.refs.remove.show()
            }
            else {
                this.refs.props.hide()
                this.refs.rotate.hide()
                this.refs.remove.hide()

                this.partLib.hide()
            }
        })
    }
}

exports.GraphTools = GraphTools
exports.mapPartLib = mapPartLib
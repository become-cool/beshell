const VM = require("./program/VM")
const BePad = require("./graph/parts/BePad")
const utils = require("./comm/utils")
const HostPart = require("./graph/parts/HostPart")

const mapWidgetNameTpl = {
    TextArea:'input%i',
    List:'list%i',
    Switch:'switch%i',
    Slider:'slider%i',
    Roller:'roller%i',
    Img:'img%i',
    Dropdown:'dropdown%i',
    Checkbox:'checkbox%i',
    Arc:'arc%i',
    Bar:'bar%i',
    Line:'line%i',
    Btn:'btn%i',
    Label:'text%i',
    Obj:'rect%i',
}


class AppModel extends beapi.EventEmitter {
    host = null
    parts = {}
    widgets = {}
    vm= new VM(this)
    folderPath = null

    _widgetsLastId = {}

    constructor(workspace) {
        super()
        this.workspace = workspace
    }

    createPart(partClass) {
        let part = new partClass(this.workspace.graph)
        this.addPart(part)
        return part
    }

    addPart(part) {
        part.uuid = utils.newUUID(this.parts)
        this.parts[part.uuid] = part
        if( !this.host && part instanceof HostPart ) {
            this.host = part
        }
        this.emit("model.part.new", part)
    }

    removePart(part) {
        delete this.parts[part.uuid]
        part.del()
    }


    widgetNames(classFilter) {
        if(!classFilter){
            return Object.keys(this.widgets)
        } else {
            
        }
    }

    addWidget (widget) {
        if(!widget.name){
            this.makeWidgetName(widget)
        }
        this.widgets[widget.name] = widget
        this.emit("model.widget.new", widget)
        widget.on("*", (eventName)=>{
            this.vm.emit('ui', widget.name, eventName)
        })
    }

    makeWidgetName(widget) {
        let clzname = widget.constructor.name
        let nameTpl = mapWidgetNameTpl[clzname]
        if(!nameTpl) {
            console.log("unknow widget class for AppModel", clzname)
            nameTpl = clzname + '%i'
        }
        if(this._widgetsLastId[clzname]==undefined){
            this._widgetsLastId[clzname] = 0
        }
        widget.name = nameTpl.replace("%i", this._widgetsLastId[clzname] ++ )
    }

    serialize() {
        let json = {
            graph: this.workspace.graph.serialize() ,
            program: this.workspace.program.serialize() ,
        }

        return json
    }


    unserialize(json) {

        if(json.graph) {
            this.workspace.graph.unserialize(json.graph,this)
        }
        // if(json.ui) {
        //     this.workspace.ui.unserialize(json.ui)
        // }
        if(json.program) {
            this.workspace.program.unserialize(json.program)
        }
    }
}


module.exports = AppModel
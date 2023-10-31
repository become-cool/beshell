const lv = require('lv')
const AppModel = require("./AppModel")
const ToolBar = require("./ToolBar")
const GraphCanvas = require("./graph/GraphCanvas")
const UI = require("./ui/UI")
const Program = require("./program/Program")
const ModelLoader = require("./ModelLoader")

let _singleton = null

class Workspace extends lv.CleanObj {
    constructor() {
        super()
        this.fromJson({
            style: {
                "bg-color": lv.rgb(249,249,249) ,
            }
        })
        
        this.model = new AppModel(this)
        
        this.graph = new GraphCanvas(this, this)
        this.ui = new UI(this)
        this.program = new Program(this)

        this.toolbar = new ToolBar(this)

        this.setActiveView(this.graph)   

        this.loader = new ModelLoader(this)
    }

    model = null
    graph = null
    toolbar = null
    activeView = null
    loader = null

    start( appFolder ) {
        this.loader.load(appFolder, this.model)
        lv.loadScreen(this)
    }

    setActiveView(view) {
        if(this.activeView==view) {
            return
        }
        this.emit("ws-active-view-changed", view, this.activeView)
        if(this.activeView) {
            this.activeView.leave()
            this.activeView.hide()
        }
        if(view) {
            view.enter()
            view.show()
        }
        this.activeView = view
    }

    static singleton() {
        if(!_singleton) {
            _singleton = new Workspace()

            global.ws = _singleton
            global.vm = _singleton.model.vm
        }
        return _singleton
    }
}

module.exports = Workspace

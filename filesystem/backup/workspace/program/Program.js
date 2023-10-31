const lv = require('lv')
const ProgramTools = require("./ProgramTools")

class Program extends lv.CleanObj{

    zoom= 2
    tools = null
    workspace = null
    model = null

    constructor(workspace) {
        super(workspace)
        this.workspace = workspace
        this.model = workspace.model

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: []
        })

        this.tools = new ProgramTools(this, this.workspace)
    }

    setZoom(zoom) {
        this.zoom = zoom
    }
    viewTools(toolbar) {
        this.tools.setParent(toolbar)
        return this.tools
    }

    enter() {}
    
    leave() {
        if(this.tools) {
            this.tools.close()
        }
    }
    
    serialize() {
        let json = {
            x: this.x(),
            y: this.y(),
        }
        let cards = this.workspace.model?.vm?.serialize()
        if(cards) {
            json.cards = cards
        }
        return json
    }

    unserialize(json) {
        // json.x
        // json.y
        if(json.cards) {
            this.workspace.model?.vm?.unserialize(json.cards, this.tools.libCardClass, this.workspace)
        }
    }
}

module.exports = Program
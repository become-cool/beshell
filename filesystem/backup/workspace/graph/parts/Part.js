const lv = require('lv')
const Port = require('./Port')

class Part extends lv.CleanObj{

    ports = []

    docW = 0
    docH = 0
    docX = 0
    docY = 0
    centerX = 0
    centerY = 0
    dir = 1  //

    snap = 4

    selected = false

    static styleSelected = new lv.Style

    constructor(graph) {
        super(graph)

        this.fromJson({
            center: true ,
            style: {
                "bg-opa": 255 ,
            } ,
            children: [] ,
        })

        this.draggable(null ,
            pos=>{
                let [docX, docY] = graph.globalToDoc(pos.x, pos.y)
                docX = Math.round(docX/this.snap) * this.snap
                docY = Math.round(docY/this.snap) * this.snap
                ; ([pos.x, pos.y] = graph.docToGlobal(docX, docY))
            } ,
            ()=>{
                this.updateDocPos()
            })
        this.on("clicked", ()=>{
            this.graph().setActivePart(this)
        })
    }

    updateDocPos() {
        let zoom = this.graph().zoom
        this.docX = this.xAligned() / zoom
        this.docY = this.yAligned() / zoom
    }
    

    updateCenterCoords(){
        let [gx,gy] = this.coords()
        this.centerX = gx + this.width()/2
        this.centerY = gy + this.height()/2
    }

    local(x, y, zoom) {
        if(this.dir==1) {
            return [
                this.centerX + x * zoom ,
                this.centerY + y * zoom
            ]
        }
        else if(this.dir==2) {
            return [
                this.centerX - y * zoom ,
                this.centerY - x * zoom
            ]
        }
        else if(this.dir==3) {
            return [
                this.centerX - x * zoom ,
                this.centerY - y * zoom
            ]
        }
        else if(this.dir==4) {
            return [
                this.centerX + y * zoom ,
                this.centerY + x * zoom
            ]
        }
    }

    graph() {
        return this.parent()
    }
    
    config() {
        return this.constructor.config
    }

    loadConf(config) {
        if(config.width) {
            this.docW = config.width + 6
        }
        if(config.height) {
            this.docH = config.height + 6
        }
        if(config.ports) {
            this.loadPortConf(config.ports, !!config.onlyFemale)
        }
        if(config["bg-color"]) {
            this.setStyle('bg-color',config["bg-color"])
        }
    }

    repaint(zoom) {
        if(!zoom) {
            zoom = this.graph().zoom
        }
        if(this.dir==2||this.dir==4) {
            this.setWidth(this.docH*zoom)
            this.setHeight(this.docW*zoom)
        }
        else {
            this.setWidth(this.docW*zoom)
            this.setHeight(this.docH*zoom)
        }

        this.setX(this.docX*zoom)
        this.setY(this.docY*zoom)
        this.updateLayout()
        
        for(let port of this.ports) {
            port.repaint(zoom, this.dir)
        }
    }
    
    loadPortConf(arrConf, female) {
        for(let portConf of arrConf) {
            this.ports.push(new Port(this, portConf, female))
        }
    }

    setSelected(selected) {
        this.selected = selected
        if(selected) {
            this.addStyle(Part.styleSelected)
        }
        else {
            this.removeStyle(Part.styleSelected)
        }
    }

    rotate() {
        if( (++this.dir) > 4 ) {
            this.dir = 1
        }
        this.repaint()
    }

    serialize() {
        let json = {
            class: this.constructor.name ,
            x: this.docX ,
            y: this.docY ,
            dir: this.dir ,
        }
        return json
    }
    
    unserialize(json) {
        if(json.x!=undefined) {
            this.docX = json.x
        }
        if(json.y!=undefined) {
            this.docY = json.y
        }
        if(json.dir!=undefined) {
            this.dir = json.dir
        }
        this.repaint()
    }
}

Part.styleSelected.set("shadow-color", lv.palette("yellow"))
Part.styleSelected.set("shadow-spread", 2)
Part.styleSelected.set("shadow-width", 2)

module.exports = Part
